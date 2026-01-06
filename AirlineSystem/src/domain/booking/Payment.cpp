#include "Payment.h"

Payment::Payment(const string& paymentId, double amount, shared_ptr<PaymentStrategy> strategy)
    : paymentId(paymentId), amount(amount), status(PaymentStatus::Pending), strategy(strategy) {}

string Payment::getPaymentId() const { return paymentId; }
double Payment::getAmount() const { return amount; }
PaymentStatus Payment::getStatus() const { return status; }

string Payment::getPaymentMethodName() const {
    if (strategy) {
        return strategy->getPaymentMethodName();
    }
    return "Unknown";
}

bool Payment::process() {
    if (strategy && strategy->processPayment(amount)) {
        status = PaymentStatus::Completed;
        return true;
    }
    status = PaymentStatus::Failed;
    return false;
}

bool Payment::refund() {
    if (status == PaymentStatus::Completed && strategy && strategy->refund(amount)) {
        status = PaymentStatus::Refunded;
        return true;
    }
    return false;
}

void Payment::setStatus(PaymentStatus newStatus) {
    status = newStatus;
}