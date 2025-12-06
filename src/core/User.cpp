/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/12/06 13:29:45 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "colors.hpp"

bool User::isAdminOfChannel(const Channel &channel) const
{
	const std::vector<User*> admins = channel.getAdminMembers();
	for (std::vector<User*>::const_iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if ((*it)->getNickname() == this->getNickname())
			return true;
	}
	return false;
}

int User::addToBuffer(const std::string &message)
{
	if (_buffer[_buffer.length() - 1] == '\n')
	{
		clearBuffer();
	}
	_buffer += message;
	
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje: '" << message << "'" << std::endl;
	if (message.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacio." << std::endl;
		return 0;
	}
/* 	if (message.substr(message.length() - 2, 2) != "\r\n")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje incompleto, mirando si el final son \\r\\n" << std::endl;
		return 0;
	} */
	if (message[message.length() - 1] != '\n')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje incompleto, sin fin de línea." << std::endl;
		return 0;
	}

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje correcto, con fin de línea, con " << _buffer.length() << " bytes." << std::endl;
	return _buffer.length();
}
