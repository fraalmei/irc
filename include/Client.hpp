#ifndef USER_HPP
#define USER_HPP

#include <string>

class Client
{

private:

    std::string _username;
    std::string _nickname;
    bool _isAuthenticated;

public:

    // Constructors
    Client();
    Client(const std::string& nickname, const std::string& username);

    // Destructor
    ~Client();

    // Getters
    std::string getNickname() const;
    std::string getUsername() const;
    bool isAuthenticated() const;

    // Setters
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setAuthenticated(bool isAuthenticated);
};

#endif // USER_HPP