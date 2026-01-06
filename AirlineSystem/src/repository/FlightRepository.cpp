#include "FlightRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

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

FlightRepository::FlightRepository(const string& dataPath)
    : dataFilePath(dataPath) {}

void FlightRepository::load(AircraftRepository& aircraftRepo) {
    flights.clear();
    
    ifstream file(dataFilePath);
    if (file.is_open()) {
        loadFromJson(aircraftRepo);
        file.close();
    } else {
        cout << "Warning: " << dataFilePath << " not found. Starting with empty flights.\n";
    }
}

void FlightRepository::loadFromJson(AircraftRepository& aircraftRepo) {
    ifstream file(dataFilePath);
    if (!file.is_open()) return;

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty() || content.find_first_not_of(" \n\r\t[]") == string::npos) {
        return;
    }

    stringstream ss(content);
    string line;
    string currentFlight = "";
    int braceCount = 0;

    while (getline(ss, line)) {
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        currentFlight += line + "\n";
        
        if (braceCount == 0 && !currentFlight.empty()) {
            if (currentFlight.find("flightId") != string::npos) {
                parseFlightObject(currentFlight, aircraftRepo);
            }
            currentFlight = "";
        }
    }
}

void FlightRepository::parseFlightObject(const string& flightJson, AircraftRepository& aircraftRepo) {
    string flightId = extractString(flightJson, "flightId");
    string flightNumber = extractString(flightJson, "flightNumber");
    string departureCity = extractString(flightJson, "departureCity");
    string arrivalCity = extractString(flightJson, "arrivalCity");
    string departureTime = extractString(flightJson, "departureTime");
    string arrivalTime = extractString(flightJson, "arrivalTime");
    string aircraftId = extractString(flightJson, "aircraftId");
    string statusStr = extractString(flightJson, "status");

    if (flightId.empty() || flightNumber.empty()) {
        return;
    }

    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        cerr << "Warning: Aircraft " << aircraftId << " not found for flight " << flightId << "\n";
        return;
    }

    FlightStatus status = stringToFlightStatus(statusStr);

    auto flight = make_shared<Flight>(flightId, flightNumber, aircraft, departureCity,
                                     arrivalCity, departureTime, arrivalTime, status);

    flights.push_back(flight);
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
        auto aircraft = flight->getAircraft();
        
        file << "  {\n";
        file << "    \"flightId\": \"" << flight->getFlightId() << "\",\n";
        file << "    \"flightNumber\": \"" << flight->getFlightNumber() << "\",\n";
        file << "    \"departureCity\": \"" << flight->getDepartureCity() << "\",\n";
        file << "    \"arrivalCity\": \"" << flight->getArrivalCity() << "\",\n";
        file << "    \"departureTime\": \"" << flight->getDepartureTime() << "\",\n";
        file << "    \"arrivalTime\": \"" << flight->getArrivalTime() << "\",\n";
        file << "    \"aircraftId\": \"" << (aircraft ? aircraft->getAircraftId() : "") << "\",\n";
        file << "    \"status\": \"" << flightStatusToString(flight->getStatus()) << "\",\n";
        
        file << "    \"seats\": [\n";
        const auto& seats = flight->getSeats();
        for (size_t j = 0; j < seats.size(); ++j) {
            const auto& seat = seats[j];
            
            string statusStr;
            switch (seat->getStatus()) {
                case SeatStatus::Available: statusStr = "Available"; break;
                case SeatStatus::Reserved: statusStr = "Reserved"; break;
                case SeatStatus::Occupied: statusStr = "Occupied"; break;
            }
            
            file << "      {\n";
            file << "        \"seatId\": \"" << seat->getSeatId() << "\",\n";
            file << "        \"seatNumber\": \"" << seat->getSeatNumber() << "\",\n";
            file << "        \"seatClass\": \"" << (seat->getSeatClass() == SeatClass::Business ? "Business" : "Economy") << "\",\n";
            file << "        \"status\": \"" << statusStr << "\"\n";
            file << "      }";
            if (j < seats.size() - 1) file << ",";
            file << "\n";
        }
        file << "    ]\n";
        
        file << "  }";
        if (i < flights.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void FlightRepository::addFlight(shared_ptr<Flight> flight) {
    if (flight && !findByFlightId(flight->getFlightId())) {
        flights.push_back(flight);
        saveToJson();
    }
}

void FlightRepository::updateFlight(shared_ptr<Flight> flight) {
    for (auto& f : flights) {
        if (f->getFlightId() == flight->getFlightId()) {
            f = flight;
            saveToJson();
            return;
        }
    }
}

void FlightRepository::deleteFlight(const string& flightId) {
    flights.erase(
        remove_if(flights.begin(), flights.end(),
            [flightId](const shared_ptr<Flight>& f) {
                return f->getFlightId() == flightId;
            }),
        flights.end()
    );
    saveToJson();
}

shared_ptr<Flight> FlightRepository::findByFlightId(const string& flightId) const {
    for (const auto& flight : flights) {
        if (flight->getFlightId() == flightId) {
            return flight;
        }
    }
    return nullptr;
}

shared_ptr<Flight> FlightRepository::findByFlightNumber(const string& flightNumber) const {
    for (const auto& flight : flights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

vector<shared_ptr<Flight>> FlightRepository::searchFlights(const string& origin, const string& destination, const string& date) const {
    vector<shared_ptr<Flight>> result;
    
    for (const auto& flight : flights) {
        if (flight->getDepartureCity() == origin && 
            flight->getArrivalCity() == destination &&
            flight->getDepartureTime().find(date) == 0) {
            result.push_back(flight);
        }
    }
    
    return result;
}

const vector<shared_ptr<Flight>>& FlightRepository::getAllFlights() const {
    return flights;
}