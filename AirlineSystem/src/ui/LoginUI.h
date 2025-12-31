#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include <memory>
#include <string>
#include "../services/auth/AuthService.h"
#include "../domain/user/User.h"

using namespace std;

class LoginUI {
private:
    AuthService& authService;

    void displayLoginPrompt();
    pair<string, string> getCredentials();

public:
    LoginUI(AuthService& service);

    shared_ptr<User> performLogin();
};

#endif
