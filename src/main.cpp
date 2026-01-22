/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2026/01/17 11:59:40 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib> // atoi
#include <csignal> // signal

static void leaks()
{
	system ("leaks -q ircserv");
}

static int	check_args(char** argv)
{
	// Check if port is a number between 1024 and 65535
	// Becasue ports range from 0 to 1023 are reserved for system use
	// and ports from 65536 and above are invalid
	for (size_t i = 0; argv[1][i]; i++)
	{
		if (!isdigit(argv[1][i]))
			return 1;
	}
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535)
		return 1;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Using port: " << argv[1] << std::endl;
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Using password: " << argv[2] << std::endl;
	return 0;
}

int	main(int argc, char **argv)
{
	atexit(leaks);
	if (argc != 3 || check_args(argv))
	{
		std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Invalid number of arguments or port number. Please use 'ircserv port password' with a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Initiating server." << std::endl;
	// Creation of the server object
	// Creacion del objeto servidor
	Server server(argv[2], atoi(argv[1]));
	try
	{
		// Set signal handlers for graceful shutdown
		// Establecer manejadores de señales para un cierre ordenado
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		// Run the server
		// Ejecutar el servidor
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << " Server distroyed." << std::endl;
	return 0;
}
