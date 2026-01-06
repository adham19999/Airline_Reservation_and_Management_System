#include "PaymentService.h"
#include <iostream>
#include <sstream>

using namespace std;

shared_ptr<Payment> PaymentService::createPayment(double amount, shared_ptr<PaymentStrategy> strategy) {
    static int paymentCounter = 1;
    stringstream ss;
    ss << "PAY" << paymentCounter++;
    
    return make_shared<Payment>(ss.str(), amount, strategy);
}

bool PaymentService::processPayment(shared_ptr<Payment> payment) {
    if (!payment) return false;
    return payment->process();
}

bool PaymentService::refundPayment(shared_ptr<Payment> payment) {
    if (!payment) return false;
    return payment->refund();
}