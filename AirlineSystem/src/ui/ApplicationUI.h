#ifndef APPLICATION_UI_H
#define APPLICATION_UI_H

#include <memory>
#include "../repository/UserRepository.h"
#include "../services/auth/AuthService.h"
#include "../services/flight/FlightService.h"
#include "../services/flight/AircraftService.h"
#include "../services/flight/CrewService.h"
#include "../services/booking/BookingService.h"
#include "../services/booking/SeatService.h"  // ✅ ADD THIS

using namespace std;

class Menu;

class ApplicationUI {
private:
    UserRepository& userRepository;
    AuthService& authService;
    FlightService& flightService;
    AircraftService& aircraftService;
    CrewService& crewService;
    BookingService& bookingService;
    SeatService& seatService;  // ✅ ADD THIS

    void displayRoleSelectionMenu() const;
    int getRoleChoice();
    unique_ptr<Menu> createMenuForRole(shared_ptr<User> user);

public:
    ApplicationUI(UserRepository& repo,
                  AuthService& authSvc,
                  FlightService& flightSvc,
                  AircraftService& aircraftSvc,
                  CrewService& crewSvc,
                  BookingService& bookingSvc,
                  SeatService& seatSvc);  // ✅ ADD THIS PARAMETER

    void run();
};

#endif