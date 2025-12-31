#include "Administrator.h"

Administrator::Administrator(const string& userId,
                             const string& username,
                             const string& passwordHash,
                             const string& fullName,
                             int adminLevel)
    : User(userId, username, passwordHash, fullName, UserRole::Administrator),
      adminLevel(adminLevel) {}

int Administrator::getAdminLevel() const {
    return adminLevel;
}
