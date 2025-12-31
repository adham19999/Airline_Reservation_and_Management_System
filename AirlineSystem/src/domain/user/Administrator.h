#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"

class Administrator : public User {
private:
    int adminLevel;   // Optional future use

public:
    Administrator(const string& userId,
                  const string& username,
                  const string& passwordHash,
                  const string& fullName,
                  int adminLevel = 1);

    int getAdminLevel() const;
};

#endif
