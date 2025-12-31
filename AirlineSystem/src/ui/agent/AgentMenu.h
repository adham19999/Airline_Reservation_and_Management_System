#ifndef AGENT_MENU_H
#define AGENT_MENU_H

#include "../Menu.h"
using namespace std;

class AgentMenu : public Menu {
private:
    bool running = true;

public:
    AgentMenu(shared_ptr<User> user);

    void display() override;
    bool handleInput(int choice) override;

private:
    void searchFlights();
    void bookFlight();
    void modifyReservation();
    void cancelReservation();
};

#endif