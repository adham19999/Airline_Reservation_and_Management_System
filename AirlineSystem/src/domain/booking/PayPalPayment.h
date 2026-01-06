#ifndef PAYPAL_PAYMENT_H
#define PAYPAL_PAYMENT_H

#include "PaymentStrategy.h"
#include <iostream>

using namespace std;

class PayPalPayment : public PaymentStrategy {
private:
    string email;

public:
    PayPalPayment(const string& email) : email(email) {}

    bool processPayment(double amount) override {
        if (email.find("@") == string::npos) {
            cerr << "Invalid PayPal email.\n";
            return false;
        }
        
        if (amount <= 0) {
            cerr << "Invalid payment amount.\n";
            return false;
        }

        cout << "Processing PayPal payment of $" << amount << "...\n";
        cout << "PayPal Account: " << email << "\n";
        cout << "Payment successful!\n";
        return true;
    }

    bool refund(double amount) override {
        if (amount <= 0) return false;
        cout << "Refunding $" << amount << " to PayPal: " << email << "\n";
        return true;
    }

    string getPaymentMethodName() const override {
        return "PayPal";
    }
};

#endif