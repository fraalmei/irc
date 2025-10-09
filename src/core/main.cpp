/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2025/10/03 12:21:52 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include "Server.hpp"
#include <cstdlib> // atoi
#include <string>

int	read_args(char** argv)
{
	int	port;
	port = atoi(argv[1]);
	if (port < 1024 || port > 65535)
	{
		std::cerr << "Invalid port number. Please use a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << "Using port: " << port << std::endl;
	std::string password = argv[2];
	std::cout << "Using password: " << password << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc != 3 &&!read_args(argv))
		return 1;
	std::cout << "Initiating server." << std::endl;
	Server server;
	server.run();
	std::cout << "Server distroyed." << std::endl;
	return 0;
}
