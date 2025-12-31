#include <iostream>
#include <memory>
#include "repository/UserRepository.h"
#include "repository/FlightRepository.h"
#include "repository/AircraftRepository.h"
#include "repository/CrewRepository.h"
#include "services/auth/AuthService.h"
#include "services/flight/FlightService.h"
#include "services/flight/AircraftService.h"
#include "services/flight/CrewService.h"
#include "ui/ApplicationUI.h"

using namespace std;

int main() {
    // Initialize repositories
    UserRepository userRepo("data/users.json");
    FlightRepository flightRepo("data/flights.json");
    AircraftRepository aircraftRepo("data/aircraft.json");
    CrewRepository crewRepo("data/crew.json");
    
    // Initialize services
    AuthService authService(userRepo);
    FlightService flightService(flightRepo, aircraftRepo);
    AircraftService aircraftService(aircraftRepo);
    CrewService crewService(crewRepo);

    // Create and run application UI
    ApplicationUI app(userRepo, authService, flightService, 
                     aircraftService, crewService);
    app.run();

    return 0;
}