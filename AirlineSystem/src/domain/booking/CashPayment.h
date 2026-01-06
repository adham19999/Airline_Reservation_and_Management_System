#ifndef CASH_PAYMENT_H
#define CASH_PAYMENT_H

#include "PaymentStrategy.h"
#include <iostream>

using namespace std;

class CashPayment : public PaymentStrategy {
private:
    double amountReceived;

public:
    CashPayment() : amountReceived(0.0) {}

    void setAmountReceived(double amount) {
        amountReceived = amount;
    }

    bool processPayment(double amount) override {
        if (amount <= 0) {
            cerr << "Invalid payment amount.\n";
            return false;
        }

        if (amountReceived < amount) {
            cerr << "Insufficient cash. Required: $" << amount 
                 << ", Received: $" << amountReceived << "\n";
            return false;
        }

        double change = amountReceived - amount;
        cout << "Cash payment of $" << amount << " received.\n";
        if (change > 0) {
            cout << "Change: $" << change << "\n";
        }
        return true;
    }

    bool refund(double amount) override {
        if (amount <= 0) return false;
        cout << "Cash refund of $" << amount << " issued.\n";
        return true;
    }

    string getPaymentMethodName() const override {
        return "Cash";
    }
};

#endif