#include "CrewRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

CrewRepository::CrewRepository(const string& dataPath)
    : dataFilePath(dataPath) {
    load();
}

void CrewRepository::load() {
    ifstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Warning: Could not open " << dataFilePath << ". Creating sample crew data.\n";
        
        auto pilot1 = make_shared<Pilot>("P1", "John Smith", "ATP-12345", 5000.0);
        crewMembers.push_back(pilot1);
        
        auto attendant1 = make_shared<FlightAttendant>("FA1", "Sarah Johnson", "Senior");
        crewMembers.push_back(attendant1);
        
        return;
    }
    
    loadFromJson();
    file.close();
}

void CrewRepository::loadFromJson() {
    ifstream file(dataFilePath);
    if (!file.is_open()) return;

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    size_t pos = 0;
    while ((pos = content.find("{", pos)) != string::npos) {
        size_t end = content.find("}", pos);
        if (end == string::npos) break;
        
        string crewJson = content.substr(pos, end - pos + 1);
        parseCrewObject(crewJson);
        pos = end + 1;
    }
}

void CrewRepository::parseCrewObject(const string& crewJson) {
    auto extractString = [](const string& line, const string& key) -> string {
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
    };

    auto extractDouble = [](const string& line, const string& key) -> double {
        size_t pos = line.find("\"" + key + "\"");
        if (pos == string::npos) return 0.0;
        pos = line.find(":", pos);
        if (pos == string::npos) return 0.0;
        stringstream ss(line.substr(pos + 1));
        double value;
        ss >> value;
        return value;
    };

    string crewId = extractString(crewJson, "crewId");
    string name = extractString(crewJson, "name");
    string crewType = extractString(crewJson, "crewType");

    if (crewType == "Pilot") {
        string licenseNumber = extractString(crewJson, "licenseNumber");
        double flightHours = extractDouble(crewJson, "flightHours");
        auto pilot = make_shared<Pilot>(crewId, name, licenseNumber, flightHours);
        crewMembers.push_back(pilot);
    } else if (crewType == "FlightAttendant") {
        string certificationLevel = extractString(crewJson, "certificationLevel");
        auto attendant = make_shared<FlightAttendant>(crewId, name, certificationLevel);
        crewMembers.push_back(attendant);
    }
}

void CrewRepository::saveToJson() const {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << dataFilePath << " for writing.\n";
        return;
    }

    file << "[\n";
    
    for (size_t i = 0; i < crewMembers.size(); ++i) {
        const auto& crew = crewMembers[i];
        file << "  {\n";
        file << "    \"crewId\": \"" << crew->getCrewId() << "\",\n";
        file << "    \"name\": \"" << crew->getName() << "\",\n";
        file << "    \"crewType\": \"" << crew->getCrewType() << "\"";
        
        auto pilot = dynamic_pointer_cast<Pilot>(crew);
        if (pilot) {
            file << ",\n";
            file << "    \"licenseNumber\": \"" << pilot->getLicenseNumber() << "\",\n";
            file << "    \"flightHours\": " << pilot->getTotalFlightHours() << "\n";
        }
        
        auto attendant = dynamic_pointer_cast<FlightAttendant>(crew);
        if (attendant) {
            file << ",\n";
            file << "    \"certificationLevel\": \"" << attendant->getCertificationLevel() << "\"\n";
        } else if (!pilot) {
            file << "\n";
        }
        
        file << "  }";
        if (i < crewMembers.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void CrewRepository::addCrewMember(shared_ptr<CrewMember> crew) {
    crewMembers.push_back(crew);
    saveToJson();  // PERSIST CHANGES
}

void CrewRepository::updateCrewMember(shared_ptr<CrewMember> crew) {
    auto it = find_if(crewMembers.begin(), crewMembers.end(),
        [&crew](const shared_ptr<CrewMember>& c) {
            return c->getCrewId() == crew->getCrewId();
        });
    
    if (it != crewMembers.end()) {
        *it = crew;
        saveToJson();  // PERSIST CHANGES
    }
}

void CrewRepository::deleteCrewMember(const string& crewId) {
    crewMembers.erase(
        remove_if(crewMembers.begin(), crewMembers.end(),
            [&crewId](const shared_ptr<CrewMember>& c) {
                return c->getCrewId() == crewId;
            }),
        crewMembers.end()
    );
    saveToJson();  // PERSIST CHANGES
}

shared_ptr<CrewMember> CrewRepository::findByCrewId(const string& crewId) const {
    for (const auto& crew : crewMembers) {
        if (crew->getCrewId() == crewId) {
            return crew;
        }
    }
    return nullptr;
}

vector<shared_ptr<CrewMember>> CrewRepository::getAllCrew() const {
    return crewMembers;
}

vector<shared_ptr<Pilot>> CrewRepository::getAllPilots() const {
    vector<shared_ptr<Pilot>> pilots;
    for (const auto& crew : crewMembers) {
        auto pilot = dynamic_pointer_cast<Pilot>(crew);
        if (pilot) {
            pilots.push_back(pilot);
        }
    }
    return pilots;
}

vector<shared_ptr<FlightAttendant>> CrewRepository::getAllAttendants() const {
    vector<shared_ptr<FlightAttendant>> attendants;
    for (const auto& crew : crewMembers) {
        auto attendant = dynamic_pointer_cast<FlightAttendant>(crew);
        if (attendant) {
            attendants.push_back(attendant);
        }
    }
    return attendants;
}