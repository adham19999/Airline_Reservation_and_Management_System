#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/user/User.h"

using namespace std;

class UserRepository {
private:
    vector<shared_ptr<User>> users;
    string dataFilePath;

    void loadFromJson();
    void saveToJson() const;
    void parseUserObject(const string& userJson);

public:
    UserRepository(const string& dataPath = "data/users.json");

    void load();
    
    void addUser(shared_ptr<User> user);
    void updateUser(shared_ptr<User> user);
    void deleteUser(const string& userId);
    
    shared_ptr<User> findByUsername(const string& username) const;
    shared_ptr<User> findByUserId(const string& userId) const;
    const vector<shared_ptr<User>>& getAllUsers() const;
};

#endif