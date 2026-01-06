#include "ReservationRepository.h"
#include "FlightRepository.h"
#include "UserRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

// Helper functions (keep as-is)
inline string extractString(const string& line, const string& key) {
    size_t pos = line.find("\"" + key + "\"");
    if (pos == string::npos) return "";
    
    pos = line.find(":", pos);
    if (pos == string::npos) return "";
    
    pos = line.find("\"", pos);
    if (pos == string::npos) return "";
    
    size_t start = pos + 1;
    size_t end = line.find("\"", start);
    if (end == string::npos) return "";
    
    return line.substr(start, end - start);
}

inline double extractDouble(const string& line, const string& key) {
    size_t pos = line.find("\"" + key + "\"");
    if (pos == string::npos) return 0.0;
    
    pos = line.find(":", pos);
    if (pos == string::npos) return 0.0;
    
    stringstream ss(line.substr(pos + 1));
    double value;
    ss >> value;
    return value;
}

// ✅ UPDATED: Constructor now accepts other repositories
ReservationRepository::ReservationRepository(const string& dataPath,
                                           FlightRepository* fRepo,
                                           UserRepository* uRepo)
    : dataFilePath(dataPath), flightRepo(fRepo), userRepo(uRepo) {
    load();
}

void ReservationRepository::load() {
    reservations.clear();
    
    ifstream file(dataFilePath);
    if (file.is_open()) {
        loadFromJson();
        file.close();
    } else {
        cout << "Warning: " << dataFilePath << " not found. Starting with empty reservations.\n";
    }
}

void ReservationRepository::loadFromJson() {
    ifstream file(dataFilePath);
    if (!file.is_open()) return;

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty() || content.find_first_not_of(" \n\r\t[]") == string::npos) {
        return;
    }

    stringstream ss(content);
    string line;
    string currentReservation = "";
    int braceCount = 0;

    while (getline(ss, line)) {
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        currentReservation += line + "\n";
        
        if (braceCount == 0 && !currentReservation.empty()) {
            if (currentReservation.find("reservationId") != string::npos) {
                parseReservationObject(currentReservation);
            }
            currentReservation = "";
        }
    }
}

// ✅ COMPLETELY REWRITTEN: Now reconstructs domain objects from IDs
void ReservationRepository::parseReservationObject(const string& reservationJson) {
    // Extract ID strings from JSON
    string reservationId = extractString(reservationJson, "reservationId");
    string passengerId = extractString(reservationJson, "passengerId");
    string flightId = extractString(reservationJson, "flightId");
    string seatId = extractString(reservationJson, "seatId");
    string bookingDate = extractString(reservationJson, "bookingDate");
    string paymentId = extractString(reservationJson, "paymentId");
    double price = extractDouble(reservationJson, "price");
    string statusStr = extractString(reservationJson, "status");

    if (reservationId.empty() || passengerId.empty() || flightId.empty()) {
        return;
    }

    // ✅ Reconstruct domain objects from IDs
    auto flight = flightRepo ? flightRepo->findByFlightId(flightId) : nullptr;
    if (!flight) {
        cerr << "Warning: Flight " << flightId << " not found for reservation " << reservationId << "\n";
        return;
    }

    auto passenger = userRepo ? dynamic_pointer_cast<Passenger>(userRepo->findByUserId(passengerId)) : nullptr;
    if (!passenger) {
        cerr << "Warning: Passenger " << passengerId << " not found for reservation " << reservationId << "\n";
        return;
    }

    // Find seat in flight's seat collection
    shared_ptr<Seat> seat = nullptr;
    for (const auto& s : flight->getSeats()) {
        if (s->getSeatId() == seatId) {
            seat = s;
            break;
        }
    }
    
    if (!seat) {
        cerr << "Warning: Seat " << seatId << " not found for reservation " << reservationId << "\n";
        return;
    }

    // Create dummy payment (since we don't persist payment details separately yet)
    auto payment = make_shared<Payment>(paymentId, price, nullptr);
    payment->setStatus(PaymentStatus::Completed);

    ReservationStatus status = stringToReservationStatus(statusStr);

    // ✅ NOW uses correct Domain constructor
    auto reservation = make_shared<Reservation>(
        reservationId,
        flight,
        passenger,
        seat,
        payment,
        bookingDate
    );
    
    reservation->setStatus(status);
    reservations.push_back(reservation);
}

string ReservationRepository::reservationStatusToString(ReservationStatus status) const {
    switch (status) {
        case ReservationStatus::Confirmed: return "Confirmed";
        case ReservationStatus::Cancelled: return "Cancelled";
        case ReservationStatus::CheckedIn: return "CheckedIn";
        case ReservationStatus::Completed: return "Completed";
        default: return "Confirmed";
    }
}

ReservationStatus ReservationRepository::stringToReservationStatus(const string& statusStr) const {
    if (statusStr == "Cancelled") return ReservationStatus::Cancelled;
    if (statusStr == "CheckedIn") return ReservationStatus::CheckedIn;
    if (statusStr == "Completed") return ReservationStatus::Completed;
    return ReservationStatus::Confirmed;
}

// ✅ REWRITTEN: Saves IDs instead of objects
void ReservationRepository::saveToJson() const {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cout << "Error: Could not open " << dataFilePath << " for writing.\n";
        return;
    }

    file << "[\n";
    
    for (size_t i = 0; i < reservations.size(); ++i) {
        const auto& res = reservations[i];
        
        file << "  {\n";
        file << "    \"reservationId\": \"" << res->getReservationId() << "\",\n";
        
        // ✅ Extract IDs from objects
        file << "    \"passengerId\": \"" 
             << (res->getPassenger() ? res->getPassenger()->getUserId() : "") << "\",\n";
        file << "    \"flightId\": \"" 
             << (res->getFlight() ? res->getFlight()->getFlightId() : "") << "\",\n";
        file << "    \"seatId\": \"" 
             << (res->getSeat() ? res->getSeat()->getSeatId() : "") << "\",\n";
        file << "    \"bookingDate\": \"" << res->getBookingDate() << "\",\n";
        
        // ✅ Extract payment info
        file << "    \"paymentId\": \"" 
             << (res->getPayment() ? res->getPayment()->getPaymentId() : "") << "\",\n";
        file << "    \"price\": " 
             << (res->getPayment() ? res->getPayment()->getAmount() : 0.0) << ",\n";
        
        file << "    \"status\": \"" << reservationStatusToString(res->getStatus()) << "\"\n";
        file << "  }";
        
        if (i < reservations.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    
    file << "]\n";
    file.close();
}

void ReservationRepository::addReservation(shared_ptr<Reservation> reservation) {
    if (reservation && !findByReservationId(reservation->getReservationId())) {
        reservations.push_back(reservation);
        saveToJson();
    }
}

void ReservationRepository::updateReservation(shared_ptr<Reservation> reservation) {
    for (auto& res : reservations) {
        if (res->getReservationId() == reservation->getReservationId()) {
            res = reservation;
            saveToJson();
            return;
        }
    }
}

void ReservationRepository::deleteReservation(const string& reservationId) {
    reservations.erase(
        remove_if(reservations.begin(), reservations.end(),
            [reservationId](const shared_ptr<Reservation>& res) {
                return res->getReservationId() == reservationId;
            }),
        reservations.end()
    );
    saveToJson();
}

shared_ptr<Reservation> ReservationRepository::findByReservationId(const string& reservationId) const {
    for (const auto& res : reservations) {
        if (res->getReservationId() == reservationId) {
            return res;
        }
    }
    return nullptr;
}

vector<shared_ptr<Reservation>> ReservationRepository::findByPassengerId(const string& passengerId) const {
    vector<shared_ptr<Reservation>> result;
    for (const auto& res : reservations) {
        if (res->getPassenger() && res->getPassenger()->getUserId() == passengerId) {
            result.push_back(res);
        }
    }
    return result;
}

vector<shared_ptr<Reservation>> ReservationRepository::findByFlightId(const string& flightId) const {
    vector<shared_ptr<Reservation>> result;
    for (const auto& res : reservations) {
        if (res->getFlight() && res->getFlight()->getFlightId() == flightId) {
            result.push_back(res);
        }
    }
    return result;
}

const vector<shared_ptr<Reservation>>& ReservationRepository::getAllReservations() const {
    return reservations;
}