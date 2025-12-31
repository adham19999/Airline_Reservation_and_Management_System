#include "Aircraft.h"

Aircraft::Aircraft(const string& aircraftId,
                   const string& registration,
                   const string& model,
                   const string& manufacturer,
                   int economySeats,
                   int businessSeats,
                   int totalSeats)
    : aircraftId(aircraftId),
      registration(registration),
      model(model),
      manufacturer(manufacturer),
      economySeats(economySeats),
      businessSeats(businessSeats),
      totalSeats(totalSeats) {}

string Aircraft::getAircraftId() const { return aircraftId; }
string Aircraft::getRegistration() const { return registration; }
string Aircraft::getModel() const { return model; }
string Aircraft::getManufacturer() const { return manufacturer; }
int Aircraft::getEconomySeats() const { return economySeats; }
int Aircraft::getBusinessSeats() const { return businessSeats; }
int Aircraft::getTotalSeats() const { return totalSeats; }
const vector<Seat>& Aircraft::getSeats() const { return seats; }