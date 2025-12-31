#ifndef PASSENGER_H
#define PASSENGER_H

#include "User.h"

class Passenger : public User {
private:
    string passportNumber;
    string contactInfo;
    string loyaltyAccountId;

public:
    Passenger(const string& userId,
              const string& username,
              const string& passwordHash,
              const string& fullName,
              const string& passportNumber,
              const string& contactInfo,
              const string& loyaltyAccountId);

    string getPassportNumber() const;
    string getContactInfo() const;
    string getLoyaltyAccountId() const;
};

#endif
