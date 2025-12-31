#ifndef USER_H
#define USER_H

#include <string>
using namespace std;
enum class UserRole {
    Administrator,
    BookingAgent,
    Passenger
};

class User {
protected:
    string userId;
    string username;
    string passwordHash;
    string fullName;
    UserRole role;
    bool active;

public:
    User(const string& userId,
         const string& username,
         const string& passwordHash,
         const string& fullName,
         UserRole role);

    virtual ~User() = default;

    // Getters
    string getUserId() const;
    string getUsername() const;
    string getFullName() const;
    UserRole getRole() const;
    string getPasswordHash() const;
    bool isActive() const;

    // Setters
    void setActive(bool active);
};

#endif
