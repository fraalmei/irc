/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_setters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:36:24 by p                 #+#    #+#             */
/*   Updated: 2025/06/12 16:40:59 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::set_server_fd(int server_fd)
{
	this->_server_fd = server_fd;
}

void	Server::set_fd_max(int fd_max)
{
	this->_fd_max = fd_max;
}

void	Server::set_master_set(fd_set master_set)
{
	this->_master_set = master_set;
}

void	Server::set_read_fds(fd_set read_fds)
{
	this->_read_fds = read_fds;
}
