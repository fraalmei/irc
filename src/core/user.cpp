/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/07/11 12:23:41 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/srcs/Client/client.cpp
#include "User.hpp"

// Constructors
User::User(int fd ) : _fd(fd), _username(""), _nickname(""), _isAuthenticated(false)
{
}
User::User(int fd, const std::string& nickname, const std::string& username) : 
	_fd(fd), _username(username), _nickname(nickname), _isAuthenticated(false)
{
}

// Destructor
User::~User() {}

// Getters
std::string User::getNickname() const
{
	return _nickname;
}

std::string User::getUsername() const
{
	return _username;
}

bool User::isAuthenticated() const
{
	return _isAuthenticated;
}

int User::getFd() const
{
	return _fd;
}

// Setters
void User::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

void User::setUsername(const std::string& username)
{
	_username = username;
}

void User::setAuthenticated(bool isAuthenticated)
{
	_isAuthenticated = isAuthenticated;
}
