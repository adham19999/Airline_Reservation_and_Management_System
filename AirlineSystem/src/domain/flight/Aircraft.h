#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
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
    int getBusinessClassCapacity() const;
    int getEconomyClassCapacity() const;
};

#endif