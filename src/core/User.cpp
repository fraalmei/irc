/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/11/14 13:37:47 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

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
	
	if (message.empty() || message.rfind("\r\n", 0) != message.size() - 2)
		return 0;
	return _buffer.length();
}
