#include "AuthService.h"
#include <iostream>

using namespace std;

AuthService::AuthService(UserRepository& repo)
    : userRepo(repo) {}

bool AuthService::validatePassword(const string& inputPassword,
                                   const string& storedPassword) const {
    return (inputPassword == storedPassword);
}

shared_ptr<User> AuthService::login(const string& username,
                                    const string& password) const {
    auto user = userRepo.findByUsername(username);

    if (!user) {
        cout << "Error: User '" << username << "' not found.\n";
        return nullptr;
    }

    if (!validatePassword(password, user->getPasswordHash())) {
        cout << "Error: Invalid password.\n";
        return nullptr;
    }

    if (!user->isActive()) {
        cout << "Error: User account is inactive.\n";
        return nullptr;
    }

    cout << "Login successful! Welcome, " << user->getFullName() << ".\n";
    return user;
}

void AuthService::logout() {
    cout << "You have been logged out.\n";
}