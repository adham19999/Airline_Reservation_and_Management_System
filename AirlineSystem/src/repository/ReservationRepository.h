#ifndef RESERVATION_REPOSITORY_H
#define RESERVATION_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/booking/Reservation.h"

using namespace std;

class ReservationRepository {
private:
    vector<shared_ptr<Reservation>> reservations;
    string dataFilePath;
    
    // ✅ ADDED: Need references to other repos to reconstruct objects
    class FlightRepository* flightRepo;
    class UserRepository* userRepo;
    
    void loadFromJson();
    void saveToJson() const;
    void parseReservationObject(const string& reservationJson);
    string reservationStatusToString(ReservationStatus status) const;
    ReservationStatus stringToReservationStatus(const string& statusStr) const;

public:
    // ✅ UPDATED: Constructor now needs other repositories
    ReservationRepository(const string& dataPath, 
                         FlightRepository* fRepo,
                         UserRepository* uRepo);
    
    void load();
    void addReservation(shared_ptr<Reservation> reservation);
    void updateReservation(shared_ptr<Reservation> reservation);
    void deleteReservation(const string& reservationId);
    
    shared_ptr<Reservation> findByReservationId(const string& reservationId) const;
    vector<shared_ptr<Reservation>> findByPassengerId(const string& passengerId) const;
    vector<shared_ptr<Reservation>> findByFlightId(const string& flightId) const;
    const vector<shared_ptr<Reservation>>& getAllReservations() const;
};

#endif