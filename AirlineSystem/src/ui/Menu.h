#ifndef MENU_H
#define MENU_H

#include <memory>
#include "../domain/user/User.h"
using namespace std;

class Menu {
protected:
    shared_ptr<User> currentUser;

public:
    Menu(shared_ptr<User> user) : currentUser(user) {}
    virtual ~Menu() = default;

    virtual void display() = 0;
    virtual bool handleInput(int choice) = 0;
};

#endif