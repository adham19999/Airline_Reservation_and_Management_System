#ifndef SEARCH_FLIGHTS_UI_H
#define SEARCH_FLIGHTS_UI_H

#include <memory>
#include "../../services/flight/FlightService.h"
#include "../../services/flight/AircraftService.h"
#include "../../domain/flight/Flight.h"

using namespace std;

class SearchFlightsUI {
private:
    FlightService& flightService;
    AircraftService& aircraftService;

    void displayFlightResults(const vector<shared_ptr<Flight>>& flights) const;

public:
    SearchFlightsUI(FlightService& fs, AircraftService& as);

    void display();
};

#endif