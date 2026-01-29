/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-10 11:38:20 by samartin          #+#    #+#             */
/*   Updated: 2026-01-23 11:12:14 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::rtrim_crlf(std::string &s)
{
    while (!s.empty())
    {
        char c = s[s.size() - 1];
        if (c == '\r' || c == '\n')
            s.erase(s.size() - 1);
        else
            break;
    }
}

void Server::print_users(void)
{
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Printing user lists." << std::endl;
	for(std::map<int, User*>::const_iterator it = getConstClientList().begin(); it != getConstClientList().end(); ++it)
		if (it->first)
			std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " User with fd " << it->first << " not found." << std::endl;
}
