/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:33:01 by p                 #+#    #+#             */
/*   Updated: 2025/11/06 18:28:42 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <ostream>

class Server;

# include "Server.hpp"

class User
{

private:

	int				_fd; // File descriptor for the client socket

	std::string		_username;
	std::string		_nickname;
	std::string		_buffer; // Store broken message
	bool			_passwd_correct;
	bool			_isAuthenticated;

public:

	// Constructors
	User(int fd) { _fd = fd; _username = ""; _nickname = ""; _isAuthenticated = false; }
	User(int fd, const std::string& nickname, const std::string& username)
	{ _fd = fd; _nickname = nickname; _username = username; _isAuthenticated = false; }

	// Destructor
	~User() {}

	// Getters
	std::string		getNickname() const { return _nickname; }
	std::string		getUsername() const { return _username; }
	std::string		getBuffer() const { return _buffer; }
	bool			isPasswdCorrect() const { return _passwd_correct; }	
	bool			isAuthenticated() const { return _isAuthenticated; }
	int				getFd() const { return _fd; }

	// Setters
	void			setNickname(const std::string& nickname) { _nickname = nickname; }
	void			setUsername(const std::string& username) { _username = username; }
	void			setPasswdCorrect(bool passwd_correct) { _passwd_correct = passwd_correct; }
	void			setAuthenticated(bool isAuthenticated) { _isAuthenticated = isAuthenticated; }
	void			setBuffer(const std::string& buffer) { _buffer = buffer; }
	int				setClient(){ return 0; }

	// Buffer management
	int				addToBuffer(const std::string& message);
	void			clearBuffer(){ _buffer.clear(); }

};

#endif // USER_HPP