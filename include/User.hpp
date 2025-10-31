#ifndef USER_HPP
#define USER_HPP

#include <string>

class User
{

private:

	int				_fd; // File descriptor for the client socket

	std::string		_username;
	std::string		_nickname;
	std::string		_buffer;
	bool			_isAuthenticated;

public:

	// Constructors
	User(int fd);
	User(int fd, const std::string& nickname, const std::string& username);

	// Destructor
	~User();

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

	// Add to buffer
	int						addToBuffer(const std::string& message);
};

#endif // USER_HPP