#include "UserRepository.h"
#include "../domain/user/Administrator.h"
#include "../domain/user/BookingAgent.h"
#include "../domain/user/Passenger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

UserRepository::UserRepository(const string& dataPath)
    : dataFilePath(dataPath) {
    load();
}

void UserRepository::load() {
    users.clear();
    
    // Try to load from JSON file
    ifstream file(dataFilePath);
    if (file.is_open()) {
        loadFromJson();
        file.close();
    } else {
        // Fallback: hardcoded sample users if file doesn't exist
        cout << "Warning: " << dataFilePath << " not found. Using sample data.\n";
        
        users.push_back(make_shared<Administrator>(
            "U1", "adminUser", "hashed_admin123", "System Admin", 1));

        users.push_back(make_shared<BookingAgent>(
            "U2", "agentJane", "hashed_agent123", "Jane Agent", "AG001", "Cairo Office"));

        users.push_back(make_shared<Passenger>(
            "U3", "passengerMike", "hashed_pass123", "Mike Smith",
            "P123456", "mike@email.com", "L001"));
    }
}

// Helper function to extract quoted string value
inline string extractString(const string& line, const string& key) {
    size_t pos = line.find("\"" + key + "\"");
    if (pos == string::npos) return "";
    
    pos = line.find(":", pos);
    if (pos == string::npos) return "";
    
    pos = line.find("\"", pos);
    if (pos == string::npos) return "";
    
    size_t start = pos + 1;
    size_t end = line.find("\"", start);
    if (end == string::npos) return "";
    
    return line.substr(start, end - start);
}

// Helper function to extract integer value
inline int extractInt(const string& line, const string& key) {
    size_t pos = line.find("\"" + key + "\"");
    if (pos == string::npos) return 0;
    
    pos = line.find(":", pos);
    if (pos == string::npos) return 0;
    
    stringstream ss(line.substr(pos + 1));
    int value;
    ss >> value;
    return value;
}

// Helper function to extract boolean value
bool extractBool(const string& line, const string& key) {
    size_t pos = line.find("\"" + key + "\"");
    if (pos == string::npos) return false;
    
    pos = line.find(":", pos);
    if (pos == string::npos) return false;
    
    string remaining = line.substr(pos + 1);
    // Remove whitespace
    remaining.erase(0, remaining.find_first_not_of(" \t"));
    
    return remaining.find("true") == 0;
}

void UserRepository::loadFromJson() {
    ifstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << dataFilePath << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    // Check if file is empty
    if (content.empty() || content.find_first_not_of(" \n\r\t") == string::npos) {
        cout << "Info: users.json is empty. Loading sample users.\n";
        
        users.push_back(make_shared<Administrator>(
            "U1", "adminUser", "hashed_admin123", "System Admin", 1));

        users.push_back(make_shared<BookingAgent>(
            "U2", "agentJane", "hashed_agent123", "Jane Agent", "AG001", "Cairo Office"));

        users.push_back(make_shared<Passenger>(
            "U3", "passengerMike", "hashed_pass123", "Mike Smith",
            "P123456", "mike@email.com", "L001"));
        return;
    }

    // Parse JSON manually - extract user objects
    stringstream ss(content);
    string line;
    string currentUser = "";
    int braceCount = 0;

    while (getline(ss, line)) {
        // Skip empty lines and array markers
        if (line.find_first_not_of(" \t\n\r") == string::npos) continue;
        
        // Count braces to identify complete user objects
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        currentUser += line + "\n";

        // When we complete a user object (closing brace), parse it
        if (braceCount == 0 && currentUser.find('{') != string::npos && currentUser.find('}') != string::npos) {
            parseUserObject(currentUser);
            currentUser = "";
        }
    }
}

void UserRepository::parseUserObject(const string& userJson) {
    // Extract common fields
    string userId = extractString(userJson, "userId");
    string username = extractString(userJson, "username");
    string passwordHash = extractString(userJson, "passwordHash");
    string fullName = extractString(userJson, "fullName");
    string roleStr = extractString(userJson, "role");
    bool active = extractBool(userJson, "active");

    if (userId.empty() || username.empty() || roleStr.empty()) {
        return; // Invalid user object
    }

    // Create user based on role
    if (roleStr == "Administrator") {
        int adminLevel = extractInt(userJson, "adminLevel");
        users.push_back(make_shared<Administrator>(userId, username, passwordHash, fullName, adminLevel));
    }
    else if (roleStr == "BookingAgent") {
        string agentId = extractString(userJson, "agentId");
        string officeLocation = extractString(userJson, "officeLocation");
        users.push_back(make_shared<BookingAgent>(userId, username, passwordHash, fullName, agentId, officeLocation));
    }
    else if (roleStr == "Passenger") {
        string passportNumber = extractString(userJson, "passportNumber");
        string email = extractString(userJson, "email");
        string loyaltyId = extractString(userJson, "loyaltyId");
        users.push_back(make_shared<Passenger>(userId, username, passwordHash, fullName, passportNumber, email, loyaltyId));
    }
}

// ...existing code...
shared_ptr<User> UserRepository::findByUsername(const string& username) const {
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

shared_ptr<User> UserRepository::findByUserId(const string& userId) const {
    for (const auto& user : users) {
        if (user->getUserId() == userId) {
            return user;
        }
    }
    return nullptr;
}

const vector<shared_ptr<User>>& UserRepository::getAllUsers() const {
    return users;
}

void UserRepository::saveToJson() const {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << dataFilePath << " for writing.\n";
        return;
    }

    file << "[\n";
    
    for (size_t i = 0; i < users.size(); ++i) {
        const auto& user = users[i];
        file << "  {\n";
        file << "    \"userId\": \"" << user->getUserId() << "\",\n";
        file << "    \"username\": \"" << user->getUsername() << "\",\n";
        file << "    \"passwordHash\": \"" << user->getPasswordHash() << "\",\n";
        file << "    \"fullName\": \"" << user->getFullName() << "\",\n";
        file << "    \"active\": " << (user->isActive() ? "true" : "false") << ",\n";
        
        // Role-specific fields
        if (user->getRole() == UserRole::Administrator) {
            file << "    \"role\": \"Administrator\",\n";
            auto admin = dynamic_pointer_cast<Administrator>(user);
            if (admin) {
                file << "    \"adminLevel\": " << admin->getAdminLevel() << "\n";
            }
        } else if (user->getRole() == UserRole::BookingAgent) {
            file << "    \"role\": \"BookingAgent\",\n";
            auto agent = dynamic_pointer_cast<BookingAgent>(user);
            if (agent) {
                file << "    \"officeLocation\": \"" << agent->getOfficeLocation() << "\"\n";
            }
        } else if (user->getRole() == UserRole::Passenger) {
            file << "    \"role\": \"Passenger\",\n";
            auto passenger = dynamic_pointer_cast<Passenger>(user);
            if (passenger) {
                file << "    \"passportNumber\": \"" << passenger->getPassportNumber() << "\",\n";
                // file << "    \"loyaltyPoints\": " << passenger->getLoyaltyPoints() << "\n";
            }
        }
        
        file << "  }";
        if (i < users.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void UserRepository::addUser(shared_ptr<User> user) {
    users.push_back(user);
    saveToJson();  // PERSIST CHANGES
}

void UserRepository::updateUser(shared_ptr<User> user) {
    auto it = find_if(users.begin(), users.end(),
        [&user](const shared_ptr<User>& u) {
            return u->getUserId() == user->getUserId();
        });
    
    if (it != users.end()) {
        *it = user;
        saveToJson();  // PERSIST CHANGES
    }
}

void UserRepository::deleteUser(const string& userId) {
    users.erase(
        remove_if(users.begin(), users.end(),
            [&userId](const shared_ptr<User>& u) {
                return u->getUserId() == userId;
            }),
        users.end()
    );
    saveToJson();  // PERSIST CHANGES
}