cmake_minimum_required(VERSION 3.10)
project(AirlineSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    
    # Domain - User
    src/domain/user/User.cpp
    src/domain/user/Administrator.cpp
    src/domain/user/BookingAgent.cpp
    src/domain/user/Passenger.cpp
    
    # Domain - Flight
    src/domain/flight/Aircraft.cpp
    src/domain/flight/Seat.cpp
    src/domain/flight/CrewMember.cpp
    src/domain/flight/Pilot.cpp
    src/domain/flight/FlightAttendant.cpp
    src/domain/flight/Flight.cpp
    
    # Domain - Booking
    src/domain/booking/Payment.cpp
    src/domain/booking/Reservation.cpp
    
    # Domain - Maintenance
    src/domain/maintenance/MaintenanceRecord.cpp
    
    # Services - Auth
    src/services/auth/AuthService.cpp
    
    # Services - Flight
    src/services/flight/FlightService.cpp
    src/services/flight/AircraftService.cpp
    src/services/flight/CrewService.cpp
    
    # Services - Booking
    src/services/booking/BookingService.cpp
    src/services/booking/SeatService.cpp
    src/services/booking/PaymentService.cpp
    
    # Repository
    src/repository/UserRepository.cpp
    
    # UI - Base
    src/ui/ApplicationUI.cpp
    src/ui/LoginUI.cpp
    
  # UI - Admin
    src/ui/admin/AdminMenu.cpp
    src/ui/admin/ManageFlightsUI.cpp
    src/ui/admin/ManageAircraftUI.cpp
    src/ui/admin/ManageCrewUI.cpp
    src/ui/admin/ManageUserUI.cpp
    
    # UI - Agent
    src/ui/agent/AgentMenu.cpp

    
    # UI - Passenger
    src/ui/passenger/PassengerMenu.cpp
    src/ui/passenger/SearchFlightsUI.cpp
    # Repository
    src/repository/UserRepository.cpp
    src/repository/FlightRepository.cpp
    src/repository/AircraftRepository.cpp
    src/repository/CrewRepository.cpp
    
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)


# Create executable
add_executable(AirlineSystem ${SOURCES})

# Output directory
set_target_properties(AirlineSystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
