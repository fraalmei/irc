/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 11:39:30 by samartin         ###   ########.fr       */
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
	_buffer += message;
	if (message.empty())
		return 0;
	if (message.length() == 1 && (message[0] == '\n' || message[0] == '\r'))
		return 0;
	if (message[message.length() - 1] != '\n')
	{
		std::cout << " Mensaje incompleto, sin fin de línea." << std::endl;
		return 0;
	}
	return _buffer.length();
}
