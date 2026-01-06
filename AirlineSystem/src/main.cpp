#include <iostream>
#include <memory>
#include "repository/UserRepository.h"
#include "repository/FlightRepository.h"
#include "repository/AircraftRepository.h"
#include "repository/CrewRepository.h"
#include "repository/ReservationRepository.h"
#include "services/auth/AuthService.h"
#include "services/flight/FlightService.h"
#include "services/flight/AircraftService.h"
#include "services/flight/CrewService.h"
#include "services/booking/SeatService.h"
#include "services/booking/BookingService.h"
#include "ui/ApplicationUI.h"

using namespace std;

int main() {
    cout << "========================================\n";
    cout << "Initializing Airline Reservation System\n";
    cout << "========================================\n";

    // Step 1: Load aircraft (flights depend on this)
    AircraftRepository aircraftRepo("data/aircraft.json");
    cout << "✓ Aircraft repository loaded.\n";

    // Step 2: Load flights (raw data only - no seat generation yet)
    FlightRepository flightRepo("data/flights.json");
    flightRepo.load(aircraftRepo);
    cout << "✓ Flight repository loaded.\n";

    // Step 3: Create FlightService (business logic layer)
    FlightService flightService(flightRepo, aircraftRepo);
    
    // ✅ NEW: Service ensures all flights have seats (business invariant)
    cout << "Ensuring flight seat availability...\n";
    flightService.ensureAllFlightsHaveSeats();
    cout << "✓ Flight initialization complete.\n";

    // Step 4: Load other repositories
    UserRepository userRepo("data/users.json");
    CrewRepository crewRepo("data/crew.json");
    ReservationRepository reservationRepo("data/reservations.json", &flightRepo, &userRepo);

    // Step 5: Initialize services
    AuthService authService(userRepo);
    AircraftService aircraftService(aircraftRepo);
    CrewService crewService(crewRepo);
    SeatService seatService(flightRepo);
    BookingService bookingService(flightRepo, reservationRepo, seatService);

    // Step 6: Run application
    ApplicationUI app(userRepo, authService, flightService, 
                     aircraftService, crewService, bookingService, seatService);
    
    app.run();

    return 0;
}