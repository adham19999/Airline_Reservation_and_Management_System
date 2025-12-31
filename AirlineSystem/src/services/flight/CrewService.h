#ifndef CREW_SERVICE_H
#define CREW_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "../../repository/CrewRepository.h"
#include "../../domain/flight/CrewMember.h"
#include "../../domain/flight/Pilot.h"
#include "../../domain/flight/FlightAttendant.h"

using namespace std;

struct CrewAssignment {
    string flightId;
    string crewId;
    string assignmentDate;
    double flightDuration;
};

class CrewService {
private:
    CrewRepository& crewRepo;
    vector<CrewAssignment> assignments;

    static constexpr double MAX_FLIGHT_HOURS_PER_DAY = 8.0;
    static constexpr double MAX_FLIGHT_HOURS_PER_WEEK = 30.0;
    static constexpr double MAX_FLIGHT_HOURS_PER_MONTH = 100.0;
    static constexpr double MAX_FLIGHT_HOURS_PER_YEAR = 1000.0;
    static constexpr double MIN_REST_HOURS = 10.0;

    double calculateFlightHours(const string& crewId, int days) const;

public:
    CrewService(CrewRepository& repo);

    shared_ptr<Pilot> addPilot(const string& name,
                              const string& licenseNumber,
                              double initialFlightHours);
    
    shared_ptr<FlightAttendant> addFlightAttendant(const string& name,
                                                   const string& certificationLevel);
    
    shared_ptr<CrewMember> getCrewById(const string& crewId) const;
    vector<shared_ptr<CrewMember>> getAllCrew() const;
    vector<shared_ptr<Pilot>> getAllPilots() const;
    vector<shared_ptr<FlightAttendant>> getAllAttendants() const;
    
    void deleteCrewMember(const string& crewId);

    bool assignCrewToFlight(const string& crewId, const string& flightId,
                           const string& flightDate, double flightDuration);
    
    void unassignCrewFromFlight(const string& crewId, const string& flightId);
    
    vector<CrewAssignment> getCrewAssignments(const string& crewId) const;
    vector<CrewAssignment> getFlightCrew(const string& flightId) const;

    bool validateFlightHours(const string& crewId, double additionalHours) const;
    bool checkRestPeriod(const string& crewId, const string& proposedFlightDate) const;
    
    double getTotalFlightHours(const string& crewId, int days) const;
    double getDailyFlightHours(const string& crewId) const;
    double getWeeklyFlightHours(const string& crewId) const;
    double getMonthlyFlightHours(const string& crewId) const;
    double getYearlyFlightHours(const string& crewId) const;

    vector<shared_ptr<Pilot>> getAvailablePilots(const string& flightDate, 
                                                  double flightDuration) const;
    vector<shared_ptr<FlightAttendant>> getAvailableAttendants(const string& flightDate,
                                                                double flightDuration) const;
};

#endif