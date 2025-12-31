#include "FlightRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;


FlightRepository::FlightRepository(const string& dataPath)
    : dataFilePath(dataPath) {
    load();
}

void FlightRepository::load() {
    flights.clear();
    
    ifstream file(dataFilePath);
    if (file.is_open()) {
        loadFromJson();
        file.close();
    } else {
        cout << "Warning: " << dataFilePath << " not found. Starting with empty flights.\n";
    }
}

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

void FlightRepository::loadFromJson() {
    ifstream file(dataFilePath);
    if (!file.is_open()) return;

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty() || content.find_first_not_of(" \n\r\t[]") == string::npos) {
        cout << "Info: flights.json is empty. No flights loaded.\n";
        return;
    }

    stringstream ss(content);
    string line;
    string currentFlight = "";
    int braceCount = 0;

    while (getline(ss, line)) {
        if (line.find_first_not_of(" \t\n\r") == string::npos) continue;
        
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        currentFlight += line + "\n";

        if (braceCount == 0 && currentFlight.find('{') != string::npos && currentFlight.find('}') != string::npos) {
            parseFlightObject(currentFlight);
            currentFlight = "";
        }
    }
}

void FlightRepository::parseFlightObject(const string& flightJson) {
    string flightId = extractString(flightJson, "flightId");
    string flightNumber = extractString(flightJson, "flightNumber");
    string departureCity = extractString(flightJson, "departureCity");
    string arrivalCity = extractString(flightJson, "arrivalCity");
    string departureTime = extractString(flightJson, "departureTime");
    string arrivalTime = extractString(flightJson, "arrivalTime");
    string aircraftId = extractString(flightJson, "aircraftId");
    int totalSeats = extractInt(flightJson, "totalSeats");
    string statusStr = extractString(flightJson, "status");

    if (flightId.empty() || flightNumber.empty()) return;

    FlightStatus status = FlightStatus::Scheduled;
    if (statusStr == "InFlight") status = FlightStatus::InFlight;
    else if (statusStr == "Completed") status = FlightStatus::Completed;
    else if (statusStr == "Cancelled") status = FlightStatus::Cancelled;
    else if (statusStr == "Delayed") status = FlightStatus::Delayed;

    auto flight = make_shared<Flight>(flightId, flightNumber, departureCity, 
                                      arrivalCity, departureTime, arrivalTime, 
                                      aircraftId, totalSeats, status);
    flights.push_back(flight);
}

shared_ptr<Flight> FlightRepository::findByFlightNumber(const string& flightNumber) const {
    for (const auto& flight : flights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

shared_ptr<Flight> FlightRepository::findByFlightId(const string& flightId) const {
    for (const auto& flight : flights) {
        if (flight->getFlightId() == flightId) {
            return flight;
        }
    }
    return nullptr;
}

const vector<shared_ptr<Flight>>& FlightRepository::getAllFlights() const {
    return flights;
}

string FlightRepository::flightStatusToString(FlightStatus status) const {
    switch (status) {
        case FlightStatus::Scheduled: return "Scheduled";
        case FlightStatus::InFlight: return "InFlight";
        case FlightStatus::Completed: return "Completed";
        case FlightStatus::Cancelled: return "Cancelled";
        case FlightStatus::Delayed: return "Delayed";
        default: return "Scheduled";
    }
}

FlightStatus FlightRepository::stringToFlightStatus(const string& statusStr) const {
    if (statusStr == "InFlight") return FlightStatus::InFlight;
    if (statusStr == "Completed") return FlightStatus::Completed;
    if (statusStr == "Cancelled") return FlightStatus::Cancelled;
    if (statusStr == "Delayed") return FlightStatus::Delayed;
    return FlightStatus::Scheduled;
}

void FlightRepository::saveToJson() const {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << dataFilePath << " for writing.\n";
        return;
    }

    file << "[\n";
    
    for (size_t i = 0; i < flights.size(); ++i) {
        const auto& flight = flights[i];
        file << "  {\n";
        file << "    \"flightId\": \"" << flight->getFlightId() << "\",\n";
        file << "    \"flightNumber\": \"" << flight->getFlightNumber() << "\",\n";
        file << "    \"departureCity\": \"" << flight->getDepartureCity() << "\",\n";
        file << "    \"arrivalCity\": \"" << flight->getArrivalCity() << "\",\n";
        file << "    \"departureTime\": \"" << flight->getDepartureTime() << "\",\n";
        file << "    \"arrivalTime\": \"" << flight->getArrivalTime() << "\",\n";
        file << "    \"status\": \"" << flightStatusToString(flight->getStatus()) << "\",\n";
        file << "    \"aircraftId\": \"" << flight->getAircraftId() << "\"\n";
        file << "  }";
        if (i < flights.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void FlightRepository::addFlight(shared_ptr<Flight> flight) {
    flights.push_back(flight);
    saveToJson();  // PERSIST CHANGES
}

void FlightRepository::updateFlight(shared_ptr<Flight> flight) {
    auto it = find_if(flights.begin(), flights.end(),
        [&flight](const shared_ptr<Flight>& f) {
            return f->getFlightId() == flight->getFlightId();
        });
    
    if (it != flights.end()) {
        *it = flight;
        saveToJson();  // PERSIST CHANGES
    }
}

void FlightRepository::deleteFlight(const string& flightId) {
    flights.erase(
        remove_if(flights.begin(), flights.end(),
            [&flightId](const shared_ptr<Flight>& f) {
                return f->getFlightId() == flightId;
            }),
        flights.end()
    );
    saveToJson();  // PERSIST CHANGES
}