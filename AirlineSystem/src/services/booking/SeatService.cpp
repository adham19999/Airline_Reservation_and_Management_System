#include "SeatService.h"
#include <algorithm>
#include <iostream>

using namespace std;

SeatService::SeatService(FlightRepository& fRepo)
    : flightRepo(fRepo) {}

shared_ptr<Seat> SeatService::getSeatById(const string& flightId, const string& seatId) const {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) return nullptr;

    for (const auto& seat : flight->getSeats()) {
        if (seat->getSeatId() == seatId) {
            return seat;
        }
    }
    return nullptr;
}

bool SeatService::isSeatAvailable(const string& flightId, const string& seatId) const {
    auto seat = getSeatById(flightId, seatId);
    return seat && seat->isAvailable();
}

bool SeatService::reserveSeat(const string& flightId, const string& seatId) {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        cerr << "Error: Flight not found: " << flightId << "\n";
        return false;
    }

    for (const auto& seat : flight->getSeats()) {
        if (seat->getSeatId() == seatId) {
            if (!seat->isAvailable()) {
                cerr << "Error: Seat " << seatId << " is not available.\n";
                return false;
            }
            seat->reserve();
            flightRepo.updateFlight(flight);
            return true;
        }
    }

    cerr << "Error: Seat not found: " << seatId << "\n";
    return false;
}

bool SeatService::releaseSeat(const string& flightId, const string& seatId) {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) return false;

    for (const auto& seat : flight->getSeats()) {
        if (seat->getSeatId() == seatId) {
            seat->release();
            flightRepo.updateFlight(flight);
            return true;
        }
    }
    return false;
}

vector<shared_ptr<Seat>> SeatService::getAvailableSeats(const string& flightId) const {
    vector<shared_ptr<Seat>> available;
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) return available;

    for (const auto& seat : flight->getSeats()) {
        if (seat->isAvailable()) {
            available.push_back(seat);
        }
    }
    return available;
}

vector<shared_ptr<Seat>> SeatService::getAvailableSeatsByClass(const string& flightId, SeatClass seatClass) const {
    vector<shared_ptr<Seat>> available;
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) return available;

    for (const auto& seat : flight->getSeats()) {
        if (seat->isAvailable() && seat->getSeatClass() == seatClass) {
            available.push_back(seat);
        }
    }
    return available;
}

int SeatService::getAvailableSeatCount(const string& flightId) const {
    // ✅ FIXED: Added static_cast to avoid size_t to int conversion warning
    return static_cast<int>(getAvailableSeats(flightId).size());
}