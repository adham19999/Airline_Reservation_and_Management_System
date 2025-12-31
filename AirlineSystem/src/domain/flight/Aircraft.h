#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include "Seat.h"
using namespace std;

class Aircraft {
private:
    string aircraftId;
    string registration;
    string model;
    string manufacturer;
    int economySeats;
    int businessSeats;
    int totalSeats;
    vector<Seat> seats;

public:
    Aircraft(const string& aircraftId,
             const string& registration,
             const string& model,
             const string& manufacturer,
             int economySeats,
             int businessSeats,
             int totalSeats);

    string getAircraftId() const;
    string getRegistration() const;
    string getModel() const;
    string getManufacturer() const;
    int getEconomySeats() const;
    int getBusinessSeats() const;
    int getTotalSeats() const;

    const vector<Seat>& getSeats() const;
};

#endif