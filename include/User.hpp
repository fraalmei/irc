#ifndef USER_HPP
#define USER_HPP

#include <string>

class User
{

private:

	int				_fd; // File descriptor for the client socket

	std::string		_username;
	std::string		_nickname;
	std::string		_buffer; // Store broken message
	bool			_isAuthenticated;

public:

	// Constructors
	User(int fd) { _fd = fd; _username = ""; _nickname = ""; _isAuthenticated = false; }
	User(int fd, const std::string& nickname, const std::string& username)
	{ _fd = fd; _nickname = nickname; _username = username; _isAuthenticated = false; }

	// Destructor
	~User() {}

	// Getters
	std::string				getNickname() const { return _nickname; }
	std::string				getUsername() const { return _username; }
	std::string				getBuffer() const { return _buffer; }
	bool					isAuthenticated() const { return _isAuthenticated; }
	int						getFd() const { return _fd; }

	// Setters
	void					setNickname(const std::string& nickname);
	void					setUsername(const std::string& username);
	void					setAuthenticated(bool isAuthenticated);
	void					setBuffer(const std::string& buffer);
	int						setClient(){ return 0; }

	// Buffer management
	void				addToBuffer(const std::string& message);
	void				clearBuffer(){ _buffer=""; }
};

#endif // USER_HPP