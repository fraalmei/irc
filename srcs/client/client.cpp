// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/srcs/Client/client.cpp
#include <client.hpp>

// Constructors
Client::Client() : _username(""), _nickname(""), _isAuthenticated(false) 
{
    // Default constructor implementation
    // Initialize members or any other necessary setup
}

Client::Client(const std::string& nickname, const std::string& username)
    : _username(username), _nickname(nickname), _isAuthenticated(false)
{
    // Parameterized constructor implementation
    // Initialize members or any other necessary setup
}

// Destructor
Client::~Client() {}

// Getters
const std::string& Client::getNickname() const
{
    return _nickname;
}

const std::string& Client::getUsername() const
{
    return _username;
}

bool Client::isAuthenticated() const
{
    return _isAuthenticated;
}

// Setters
void Client::setNickname(const std::string& nickname)
{
    _nickname = nickname;
}

void Client::setUsername(const std::string& username)
{
    _username = username;
}

void Client::setAuthenticated(bool isAuthenticated)
{
    _isAuthenticated = isAuthenticated;
}