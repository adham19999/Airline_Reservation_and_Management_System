#ifndef CREDIT_CARD_PAYMENT_H
#define CREDIT_CARD_PAYMENT_H

#include "PaymentStrategy.h"
#include <iostream>

using namespace std;

class CreditCardPayment : public PaymentStrategy {
private:
    string cardNumber;
    string cardHolderName;
    string expiryDate;

public:
    CreditCardPayment(const string& cardNumber, 
                      const string& cardHolderName,
                      const string& expiryDate)
        : cardNumber(cardNumber),
          cardHolderName(cardHolderName),
          expiryDate(expiryDate) {}

    bool processPayment(double amount) override {
        if (cardNumber.length() < 13 || cardNumber.length() > 19) {
            cerr << "Invalid card number.\n";
            return false;
        }
        
        if (amount <= 0) {
            cerr << "Invalid payment amount.\n";
            return false;
        }

        cout << "Processing credit card payment of $" << amount << "...\n";
        cout << "Card ending in: " << cardNumber.substr(cardNumber.length() - 4) << "\n";
        cout << "Payment successful!\n";
        return true;
    }

    bool refund(double amount) override {
        if (amount <= 0) return false;
        cout << "Refunding $" << amount << " to card ending in " 
             << cardNumber.substr(cardNumber.length() - 4) << "...\n";
        return true;
    }

    string getPaymentMethodName() const override {
        return "Credit Card";
    }
};

#endif