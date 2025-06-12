#ifndef USER_HPP
#define USER_HPP

#include <string>

class Client
{

private:

	int				_fd; // File descriptor for the client socket

	std::string		_username;
	std::string		_nickname;
	bool			_isAuthenticated;

public:

	// Constructors
	Client(int fd);
	Client(int fd, const std::string& nickname, const std::string& username);

	// Destructor
	~Client();

	// Getters
	std::string				getNickname() const;
	std::string				getUsername() const;
	bool					isAuthenticated() const;
	int						getFd() const;

	// Setters
	void					setNickname(const std::string& nickname);
	void					setUsername(const std::string& username);
	void					setAuthenticated(bool isAuthenticated);

	int						setClient();

};

#endif // USER_HPP