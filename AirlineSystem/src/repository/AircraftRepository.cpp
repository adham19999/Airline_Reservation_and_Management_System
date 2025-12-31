#include "AircraftRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;


AircraftRepository::AircraftRepository(const string& dataPath)
    : dataFilePath(dataPath) {
    load();
}

void AircraftRepository::load() {
    aircraft.clear();
    
    ifstream file(dataFilePath);
    if (file.is_open()) {
        loadFromJson();
        file.close();
    } else {
        cout << "Warning: " << dataFilePath << " not found. Starting with empty aircraft.\n";
    }
}

inline  string extractString(const string& line, const string& key) {
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

void AircraftRepository::loadFromJson() {
    ifstream file(dataFilePath);
    if (!file.is_open()) return;

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty() || content.find_first_not_of(" \n\r\t[]") == string::npos) {
        cout << "Info: aircraft.json is empty. No aircraft loaded.\n";
        return;
    }

    stringstream ss(content);
    string line;
    string currentAircraft = "";
    int braceCount = 0;

    while (getline(ss, line)) {
        if (line.find_first_not_of(" \t\n\r") == string::npos) continue;
        
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        currentAircraft += line + "\n";

        if (braceCount == 0 && currentAircraft.find('{') != string::npos && currentAircraft.find('}') != string::npos) {
            parseAircraftObject(currentAircraft);
            currentAircraft = "";
        }
    }
}

void AircraftRepository::parseAircraftObject(const string& aircraftJson) {
    string aircraftId = extractString(aircraftJson, "aircraftId");
    string registration = extractString(aircraftJson, "registration");
    string model = extractString(aircraftJson, "model");
    string manufacturer = extractString(aircraftJson, "manufacturer");
    int economySeats = extractInt(aircraftJson, "economySeats");
    int businessSeats = extractInt(aircraftJson, "businessSeats");
    int totalSeats = extractInt(aircraftJson, "totalSeats");

    if (aircraftId.empty() || registration.empty()) return;

    auto ac = make_shared<Aircraft>(aircraftId, registration, model, manufacturer, 
                                    economySeats, businessSeats, totalSeats);
    aircraft.push_back(ac);
}

shared_ptr<Aircraft> AircraftRepository::findByAircraftId(const string& aircraftId) const {
    for (const auto& ac : aircraft) {
        if (ac->getAircraftId() == aircraftId) {
            return ac;
        }
    }
    return nullptr;
}

shared_ptr<Aircraft> AircraftRepository::findByRegistration(const string& registration) const {
    for (const auto& ac : aircraft) {
        if (ac->getRegistration() == registration) {
            return ac;
        }
    }
    return nullptr;
}

const vector<shared_ptr<Aircraft>>& AircraftRepository::getAllAircraft() const {
    return aircraft;
}

void AircraftRepository::saveToJson() const {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << dataFilePath << " for writing.\n";
        return;
    }

    file << "[\n";
    
    for (size_t i = 0; i < aircraft.size(); ++i) {
        const auto& ac = aircraft[i];
        file << "  {\n";
        file << "    \"aircraftId\": \"" << ac->getAircraftId() << "\",\n";
        file << "    \"registration\": \"" << ac->getRegistration() << "\",\n";
        file << "    \"model\": \"" << ac->getModel() << "\",\n";
        file << "    \"manufacturer\": \"" << ac->getManufacturer() << "\",\n";
        file << "    \"economySeats\": " << ac->getEconomySeats() << ",\n";
        file << "    \"businessSeats\": " << ac->getBusinessSeats() << ",\n";
        file << "    \"totalSeats\": " << ac->getTotalSeats() << "\n";
        file << "  }";
        if (i < aircraft.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void AircraftRepository::addAircraft(shared_ptr<Aircraft> ac) {
    if (ac && !findByAircraftId(ac->getAircraftId())) {
        aircraft.push_back(ac);
        saveToJson();  // PERSIST CHANGES
    }
}

void AircraftRepository::updateAircraft(shared_ptr<Aircraft> ac) {
    for (auto& a : aircraft) {
        if (a->getAircraftId() == ac->getAircraftId()) {
            a = ac;
            saveToJson();  // PERSIST CHANGES
            return;
        }
    }
}

void AircraftRepository::deleteAircraft(const string& aircraftId) {
    aircraft.erase(remove_if(aircraft.begin(), aircraft.end(),
        [aircraftId](const shared_ptr<Aircraft>& a) { return a->getAircraftId() == aircraftId; }),
        aircraft.end());
    saveToJson();  // PERSIST CHANGES
}