#include "Passenger.h"

Passenger::Passenger(const string& userId,
                     const string& username,
                     const string& passwordHash,
                     const string& fullName,
                     const string& passportNumber,
                     const string& contactInfo,
                     const string& loyaltyAccountId)
    : User(userId, username, passwordHash, fullName, UserRole::Passenger),
      passportNumber(passportNumber),
      contactInfo(contactInfo),
      loyaltyAccountId(loyaltyAccountId) {}

string Passenger::getPassportNumber() const {
    return passportNumber;
}

string Passenger::getContactInfo() const {
    return contactInfo;
}

string Passenger::getLoyaltyAccountId() const {
    return loyaltyAccountId;
}
