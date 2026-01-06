#ifndef PAYMENT_SERVICE_H
#define PAYMENT_SERVICE_H

#include <memory>
#include <string>
#include "../../domain/booking/Payment.h"
#include "../../domain/booking/PaymentStrategy.h"

using namespace std;

class PaymentService {
public:
    shared_ptr<Payment> createPayment(double amount, shared_ptr<PaymentStrategy> strategy);
    bool processPayment(shared_ptr<Payment> payment);
    bool refundPayment(shared_ptr<Payment> payment);
};

#endif