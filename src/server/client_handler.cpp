/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 16:44:19 by p                 #+#    #+#             */
/*   Updated: 2026/01/09 00:29:11 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::closeClientFd(int fd)
{
    // Close the client file descriptor and remove it from the clients map
    // Cerrar el descriptor de archivo del cliente y eliminarlo del mapa de clientes
    std::map<int, User*>::iterator it = getClientList().find(fd);
    if (it != getClientList().end())
    {
        delete it->second; // Free the User object
        getClientList().erase(it); // Remove from the map
        close(fd); // Close the file descriptor
        std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Closed client fd " << fd << "." << std::endl;
        return 0; // Success
    }
    std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Client fd " << fd << " not found." << std::endl;
    return -1; // Failure
}

int Server::set_Authentication(User *client)
{
    std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Cheking autentication." << std::endl;
    if (client->getNickname() != "" && client->getUsername() != "" && client->isPasswdCorrect())
    {
        std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Autenticated." << std::endl;
        return 1;
    }
    else
    {
        std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Not autenticated yet." << std::endl;
        return 0;
    }
}
