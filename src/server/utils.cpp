/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 11:38:20 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 10:08:16 by samartin         ###   ########.fr       */
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
	std::cout << "Printing user lists." << std::endl;
	for(std::map<int, User*>::const_iterator it = getConstClientList().begin(); it != getConstClientList().end(); ++it)
		if (it->first)
			std::cout << " User with fd " << it->first << " not found." << std::endl;
}
