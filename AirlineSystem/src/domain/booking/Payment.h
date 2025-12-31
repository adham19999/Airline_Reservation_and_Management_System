#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
using namespace std;

enum class PaymentMethod {
    CreditCard,
    Cash,
    PayPal
};

enum class PaymentStatus {
    Pending,
    Completed,
    Refunded,
    Failed
};

class Payment {
private:
    string paymentId;
    PaymentMethod method;
    double amount;
    PaymentStatus status;

public:
    Payment(const string& paymentId,
            PaymentMethod method,
            double amount);

    string getPaymentId() const;
    PaymentMethod getMethod() const;
    double getAmount() const;
    PaymentStatus getStatus() const;

    void setStatus(PaymentStatus status);
};

#endif
