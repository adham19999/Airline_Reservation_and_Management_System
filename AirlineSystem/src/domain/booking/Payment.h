#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <memory>
#include "PaymentStrategy.h"
using namespace std;

enum class PaymentStatus {
    Pending,
    Completed,
    Failed,
    Refunded
};

class Payment {
private:
    string paymentId;
    double amount;
    PaymentStatus status;
    shared_ptr<PaymentStrategy> strategy;

public:
    Payment(const string& paymentId, double amount, shared_ptr<PaymentStrategy> strategy);

    string getPaymentId() const;
    double getAmount() const;
    PaymentStatus getStatus() const;
    string getPaymentMethodName() const;

    bool process();
    bool refund();
    void setStatus(PaymentStatus newStatus);
};

#endif