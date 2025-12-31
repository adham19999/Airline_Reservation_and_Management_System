#include "BookingAgent.h"

BookingAgent::BookingAgent(const string& userId,
                           const string& username,
                           const string& passwordHash,
                           const string& fullName,
                           const string& agentId,
                           const string& officeLocation)
    : User(userId, username, passwordHash, fullName, UserRole::BookingAgent),
      agentId(agentId),
      officeLocation(officeLocation) {}

string BookingAgent::getAgentId() const {
    return agentId;
}

string BookingAgent::getOfficeLocation() const {
    return officeLocation;
}
