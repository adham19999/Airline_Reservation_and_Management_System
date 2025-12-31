#ifndef BOOKING_AGENT_H
#define BOOKING_AGENT_H

#include "User.h"

class BookingAgent : public User {
private:
    string agentId;
    string officeLocation;

public:
    BookingAgent(const string& userId,
                 const string& username,
                 const string& passwordHash,
                 const string& fullName,
                 const string& agentId,
                 const string& officeLocation);

    string getAgentId() const;
    string getOfficeLocation() const;
};

#endif
