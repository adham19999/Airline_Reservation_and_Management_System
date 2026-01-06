#ifndef PAYMENT_STRATEGY_H
#define PAYMENT_STRATEGY_H

#include <string>

using namespace std;

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
    virtual bool refund(double amount) = 0;
    virtual string getPaymentMethodName() const = 0;
};

#endif