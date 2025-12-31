#include "Payment.h"

Payment::Payment(const string& paymentId,
                 PaymentMethod method,
                 double amount)
    : paymentId(paymentId),
      method(method),
      amount(amount),
      status(PaymentStatus::Pending) {}

string Payment::getPaymentId() const {
    return paymentId;
}

PaymentMethod Payment::getMethod() const {
    return method;
}

double Payment::getAmount() const {
    return amount;
}

PaymentStatus Payment::getStatus() const {
    return status;
}

void Payment::setStatus(PaymentStatus status) {
    this->status = status;
}
