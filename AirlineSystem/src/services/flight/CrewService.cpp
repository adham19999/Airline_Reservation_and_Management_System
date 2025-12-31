#include "CrewService.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

CrewService::CrewService(CrewRepository& repo)
    : crewRepo(repo) {}

shared_ptr<Pilot> CrewService::addPilot(const string& name,
                                       const string& licenseNumber,
                                       double initialFlightHours) {
    static int pilotCounter = 1;
    stringstream ss;
    ss << "P" << pilotCounter++;
    string crewId = ss.str();

    auto pilot = make_shared<Pilot>(crewId, name, licenseNumber, initialFlightHours);
    crewRepo.addCrewMember(pilot);
    
    cout << "Pilot added successfully with ID: " << crewId << "\n";
    return pilot;
}

shared_ptr<FlightAttendant> CrewService::addFlightAttendant(const string& name,
                                                            const string& certificationLevel) {
    static int attendantCounter = 1;
    stringstream ss;
    ss << "FA" << attendantCounter++;
    string crewId = ss.str();

    auto attendant = make_shared<FlightAttendant>(crewId, name, certificationLevel);
    crewRepo.addCrewMember(attendant);
    
    cout << "Flight Attendant added successfully with ID: " << crewId << "\n";
    return attendant;
}

shared_ptr<CrewMember> CrewService::getCrewById(const string& crewId) const {
    return crewRepo.findByCrewId(crewId);
}

vector<shared_ptr<CrewMember>> CrewService::getAllCrew() const {
    return crewRepo.getAllCrew();
}

vector<shared_ptr<Pilot>> CrewService::getAllPilots() const {
    return crewRepo.getAllPilots();
}

vector<shared_ptr<FlightAttendant>> CrewService::getAllAttendants() const {
    return crewRepo.getAllAttendants();
}

void CrewService::deleteCrewMember(const string& crewId) {
    // Check if crew has active assignments
    auto crewAssignments = getCrewAssignments(crewId);
    if (!crewAssignments.empty()) {
        cerr << "Error: Cannot delete crew member with active assignments.\n";
        return;
    }
    
    crewRepo.deleteCrewMember(crewId);
    cout << "Crew member deleted successfully.\n";
}

bool CrewService::assignCrewToFlight(const string& crewId, const string& flightId,
                                    const string& flightDate, double flightDuration) {
    auto crew = crewRepo.findByCrewId(crewId);
    if (!crew) {
        cerr << "Error: Crew member '" << crewId << "' not found.\n";
        return false;
    }

    // Validate flight hours compliance
    if (!validateFlightHours(crewId, flightDuration)) {
        cerr << "Error: Assignment would exceed regulatory flight hour limits.\n";
        return false;
    }

    // Check rest period
    if (!checkRestPeriod(crewId, flightDate)) {
        cerr << "Error: Insufficient rest period for crew member.\n";
        return false;
    }

    // Create assignment
    CrewAssignment assignment;
    assignment.flightId = flightId;
    assignment.crewId = crewId;
    assignment.assignmentDate = flightDate;
    assignment.flightDuration = flightDuration;
    
    assignments.push_back(assignment);

    // Update pilot flight hours if applicable
    auto pilot = dynamic_pointer_cast<Pilot>(crew);
    if (pilot) {
        pilot->addFlightHours(flightDuration);
    }

    cout << "Crew member assigned to flight successfully.\n";
    return true;
}

void CrewService::unassignCrewFromFlight(const string& crewId, const string& flightId) {
    assignments.erase(
        remove_if(assignments.begin(), assignments.end(),
            [&](const CrewAssignment& a) {
                return a.crewId == crewId && a.flightId == flightId;
            }),
        assignments.end()
    );
    cout << "Crew member unassigned from flight.\n";
}

vector<CrewAssignment> CrewService::getCrewAssignments(const string& crewId) const {
    vector<CrewAssignment> crewAssignments;
    for (const auto& assignment : assignments) {
        if (assignment.crewId == crewId) {
            crewAssignments.push_back(assignment);
        }
    }
    return crewAssignments;
}

vector<CrewAssignment> CrewService::getFlightCrew(const string& flightId) const {
    vector<CrewAssignment> flightCrew;
    for (const auto& assignment : assignments) {
        if (assignment.flightId == flightId) {
            flightCrew.push_back(assignment);
        }
    }
    return flightCrew;
}

bool CrewService::validateFlightHours(const string& crewId, double additionalHours) const {
    double dailyHours = getDailyFlightHours(crewId) + additionalHours;
    double weeklyHours = getWeeklyFlightHours(crewId) + additionalHours;
    double monthlyHours = getMonthlyFlightHours(crewId) + additionalHours;
    double yearlyHours = getYearlyFlightHours(crewId) + additionalHours;

    return dailyHours <= MAX_FLIGHT_HOURS_PER_DAY &&
           weeklyHours <= MAX_FLIGHT_HOURS_PER_WEEK &&
           monthlyHours <= MAX_FLIGHT_HOURS_PER_MONTH &&
           yearlyHours <= MAX_FLIGHT_HOURS_PER_YEAR;
}

bool CrewService::checkRestPeriod(const string& crewId, const string& proposedFlightDate) const {
    // Simplified: In production, would parse dates and check MIN_REST_HOURS between flights
    auto crewAssignments = getCrewAssignments(crewId);
    
    // For now, just check if crew has assignments on same date
    for (const auto& assignment : crewAssignments) {
        if (assignment.assignmentDate == proposedFlightDate) {
            // Already has flight on this date - would need time comparison
            return false;
        }
    }
    
    return true;
}

double CrewService::getTotalFlightHours(const string& crewId, int days) const {
    return calculateFlightHours(crewId, days);
}

double CrewService::getDailyFlightHours(const string& crewId) const {
    return calculateFlightHours(crewId, 1);
}

double CrewService::getWeeklyFlightHours(const string& crewId) const {
    return calculateFlightHours(crewId, 7);
}

double CrewService::getMonthlyFlightHours(const string& crewId) const {
    return calculateFlightHours(crewId, 30);
}

double CrewService::getYearlyFlightHours(const string& crewId) const {
    return calculateFlightHours(crewId, 365);
}

double CrewService::calculateFlightHours(const string& crewId, int days) const {
    // Simplified: Would use actual date calculation in production
    double totalHours = 0.0;
    auto crewAssignments = getCrewAssignments(crewId);
    
    for (const auto& assignment : crewAssignments) {
        totalHours += assignment.flightDuration;
    }
    
    return totalHours;
}

vector<shared_ptr<Pilot>> CrewService::getAvailablePilots(const string& flightDate,
                                                          double flightDuration) const {
    vector<shared_ptr<Pilot>> availablePilots;
    auto allPilots = crewRepo.getAllPilots();
    
    for (const auto& pilot : allPilots) {
        if (validateFlightHours(pilot->getCrewId(), flightDuration) &&
            checkRestPeriod(pilot->getCrewId(), flightDate)) {
            availablePilots.push_back(pilot);
        }
    }
    
    return availablePilots;
}

vector<shared_ptr<FlightAttendant>> CrewService::getAvailableAttendants(const string& flightDate,
                                                                        double flightDuration) const {
    vector<shared_ptr<FlightAttendant>> availableAttendants;
    auto allAttendants = crewRepo.getAllAttendants();
    
    for (const auto& attendant : allAttendants) {
        if (validateFlightHours(attendant->getCrewId(), flightDuration) &&
            checkRestPeriod(attendant->getCrewId(), flightDate)) {
            availableAttendants.push_back(attendant);
        }
    }
    
    return availableAttendants;
}