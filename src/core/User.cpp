/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 09:30:07 by samartin         ###   ########.fr       */
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
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer actual: '" << _buffer << "'" << std::endl;
	_buffer += message;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Buffer + mensaje: '" << _buffer << "'" << std::endl;
	/* size_t pos;
	while ((pos = _buffer.find("\r\n")) != std::string::npos)
	{
    	std::string line = _buffer.substr(0, pos);
    	_buffer.erase(0, pos + 2); // +2 porque es "\r\n"
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Linea de mensaje procesada: '" << line << "'" << std::endl;
    	// procesar 'line'
	} */

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje: '" << message << "'" << std::endl;
	if (message.empty())
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacio." << std::endl;
		return 0;
	}
	if (message.length() == 1 && (message[0] == '\n' || message[0] == '\r'))
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje vacio." << std::endl;
		return 0;
	}
	/* if (message.substr(0, 11).compare("CAP LS 302") == 0)
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje CAP LS 302 recibido." << std::endl;
		return -10;
	} */
	/* if (message.substr(message.length() - 2, 2) == "\r\n")
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje completo" << std::endl;
		return -1;
	} */
	if (message[message.length() - 1] != '\n')
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje incompleto, sin fin de línea." << std::endl;
		return 0;
	}

	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Mensaje correcto, con fin de línea, con " << _buffer.length() << " bytes." << std::endl;
	return _buffer.length();
}
