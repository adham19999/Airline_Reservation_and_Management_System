#include "User.h"

User::User(const string& userId,
           const string& username,
           const string& passwordHash,
           const string& fullName,
           UserRole role)
    : userId(userId),
      username(username),
      passwordHash(passwordHash),
      fullName(fullName),
      role(role),
      active(true) {}

string User::getUserId() const {
    return userId;
}

string User::getUsername() const {
    return username;
}

string User::getFullName() const {
    return fullName;
}

UserRole User::getRole() const {
    return role;
}
string User::getPasswordHash() const { return passwordHash; } 

bool User::isActive() const {
    return active;
}

void User::setActive(bool active) {
    this->active = active;
}
