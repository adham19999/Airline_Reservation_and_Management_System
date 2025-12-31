#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <memory>
#include <string>
#include "../../repository/UserRepository.h"
#include "../../domain/user/User.h"

using namespace std;

class AuthService {
private:
    UserRepository& userRepo;

    bool validatePassword(const string& inputPassword,
                          const string& storedPassword) const;

public:
    AuthService(UserRepository& repo);

    shared_ptr<User> login(const string& username,
                          const string& password) const;

    void logout();
};

#endif