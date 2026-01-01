#ifndef MANAGE_USERS_UI_H
#define MANAGE_USERS_UI_H

#include <memory>
#include "../../repository/UserRepository.h"
#include "../../domain/user/User.h"

using namespace std;

class ManageUsersUI {
private:
    UserRepository& userRepository;

    void displayAllUsers() const;
    void displayUserDetails(shared_ptr<User> user) const;
    void addNewUser();
    void updateUserStatus();
    void deleteUser();
    void searchUser();
    UserRole getUserRoleChoice() const;
    string getRoleString(UserRole role) const;

public:
    ManageUsersUI(UserRepository& repo);

    void display();
};

#endif