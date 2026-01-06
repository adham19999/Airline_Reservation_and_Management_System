#include "BookFlightUI.h"
#include "../../domain/booking/CreditCardPayment.h"
#include "../../domain/booking/CashPayment.h"
#include "../../domain/booking/PayPalPayment.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

BookFlightUI::BookFlightUI(BookingService& bs, FlightService& fs, SeatService& ss, shared_ptr<Passenger> passenger)
    : bookingService(bs), flightService(fs), seatService(ss), currentPassenger(passenger) {}

void BookFlightUI::display() {
    cout << "\n========================================\n";
    cout << "--- Book a Flight ---\n";
    cout << "========================================\n";

    string flightNumber;
    cout << "Enter Flight Number: ";
    getline(cin, flightNumber);

    auto flight = flightService.getFlightByNumber(flightNumber);
    if (!flight) {
        cout << "Error: Flight not found.\n";
        return;
    }

    // Display flight details
    cout << "\nFlight Details:\n";
    cout << "Flight: " << flight->getFlightNumber() << "\n";
    cout << "From: " << flight->getDepartureCity() << "\n";
    cout << "To: " << flight->getArrivalCity() << "\n";
    cout << "Departure: " << flight->getDepartureTime() << "\n";
    cout << "Arrival: " << flight->getArrivalTime() << "\n";

    // Show available seats
    int availableCount = seatService.getAvailableSeatCount(flight->getFlightId());
    if (availableCount == 0) {
        cout << "\nError: No available seats on this flight.\n";
        return;
    }

    cout << "\nAvailable Seats: " << availableCount << "\n";
    
    // Seat selection
    string seatId = selectSeat(flight);
    if (seatId.empty()) {
        cout << "Booking cancelled.\n";
        return;
    }

    // Payment method selection
    auto paymentStrategy = selectPaymentMethod();
    if (!paymentStrategy) {
        cout << "Booking cancelled.\n";
        return;
    }

    // Book flight
    auto reservation = bookingService.bookFlight(
        currentPassenger,
        flight->getFlightId(),
        seatId,
        paymentStrategy
    );

    if (reservation) {
        cout << "\n✓ Booking confirmed! Reservation ID: " << reservation->getReservationId() << "\n";
    } else {
        cout << "\nError: Booking failed. Please try again.\n";
    }
}

void BookFlightUI::displayAvailableSeats(shared_ptr<Flight> flight) const {
    auto availableSeats = seatService.getAvailableSeats(flight->getFlightId());
    
    if (availableSeats.empty()) {
        cout << "\nNo available seats.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "Available Seats:\n";
    cout << "========================================\n";
    cout << left << setw(10) << "Seat" << setw(15) << "Class" << setw(10) << "Status" << "\n";
    cout << "----------------------------------------\n";

    for (const auto& seat : availableSeats) {
        string seatClass = (seat->getSeatClass() == SeatClass::Business) ? "Business" : "Economy";
        
        cout << left << setw(10) << seat->getSeatNumber() 
             << setw(15) << seatClass 
             << setw(10) << "Available" << "\n";
    }
}

string BookFlightUI::selectSeat(shared_ptr<Flight> flight) {
    displayAvailableSeats(flight);

    cout << "\nEnter Seat Number (e.g., 12A) or '0' to cancel: ";
    string seatNumber;
    getline(cin, seatNumber);

    if (seatNumber == "0") {
        return "";
    }

    // Find seat by number
    for (const auto& seat : flight->getSeats()) {
        if (seat->getSeatNumber() == seatNumber) {
            if (seat->isAvailable()) {
                return seat->getSeatId();
            } else {
                cout << "Error: Seat " << seatNumber << " is already occupied. Please choose a different seat.\n";
                return "";
            }
        }
    }

    cout << "Error: Seat number " << seatNumber << " is invalid. Please enter a valid seat number (e.g., 12A).\n";
    return "";
}

shared_ptr<PaymentStrategy> BookFlightUI::selectPaymentMethod() {
    cout << "\n========================================\n";
    cout << "Payment Method:\n";
    cout << "========================================\n";
    cout << "1. Credit Card\n";
    cout << "2. Cash\n";
    cout << "3. PayPal\n";
    cout << "4. Cancel\n";
    cout << "Enter choice: ";
    
    int paymentChoice;
    cin >> paymentChoice;
    cin.ignore();

    shared_ptr<PaymentStrategy> paymentStrategy;

    switch (paymentChoice) {
        case 1: {
            string cardNumber, cardHolder, expiry;
            cout << "\nEnter Payment Details:\n";
            cout << "Card Number: ";
            getline(cin, cardNumber);
            cout << "Card Holder Name: ";
            getline(cin, cardHolder);
            cout << "Expiry (MM/YY): ";
            getline(cin, expiry);
            
            paymentStrategy = make_shared<CreditCardPayment>(cardNumber, cardHolder, expiry);
            break;
        }
        case 2: {
            double amount;
            cout << "\nCash Amount Received: $";
            cin >> amount;
            cin.ignore();
            
            auto cashPayment = make_shared<CashPayment>();
            cashPayment->setAmountReceived(amount);
            paymentStrategy = cashPayment;
            break;
        }
        case 3: {
            string email;
            cout << "\nPayPal Email: ";
            getline(cin, email);
            
            paymentStrategy = make_shared<PayPalPayment>(email);
            break;
        }
        case 4:
            return nullptr;
        default:
            cout << "Invalid choice.\n";
            return nullptr;
    }

    return paymentStrategy;
}