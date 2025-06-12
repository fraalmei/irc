/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/06/12 17:28:03 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// filepath: /home/arksh/Documentos/42 Madrid/irc/srcs/srcs/Client/client.cpp
#include "Client.hpp"

// Constructors
Client::Client(int fd ) : _fd(fd), _username(""), _nickname(""), _isAuthenticated(false)
{
}
Client::Client(int fd, const std::string& nickname, const std::string& username) : 
	_fd(fd), _username(username), _nickname(nickname), _isAuthenticated(false)
{
}

// Destructor
Client::~Client() {}

// Getters
std::string Client::getNickname() const
{
	return _nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

bool Client::isAuthenticated() const
{
	return _isAuthenticated;
}

int Client::getFd() const
{
	return _fd;
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
