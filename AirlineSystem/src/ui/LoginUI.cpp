#include "LoginUI.h"
#include <iostream>

using namespace std;

LoginUI::LoginUI(AuthService& service)
    : authService(service) {}

void LoginUI::displayLoginPrompt() {
    cout << "\n--- Login ---\n";
}

pair<string, string> LoginUI::getCredentials() {
    cout << "Username: ";
    string username;
    getline(cin, username);

    cout << "Password: ";
    string password;
    getline(cin, password);

    return make_pair(username, password);
}

shared_ptr<User> LoginUI::performLogin() {
    displayLoginPrompt();
    auto credentials = getCredentials();

    return authService.login(credentials.first, credentials.second);
}
