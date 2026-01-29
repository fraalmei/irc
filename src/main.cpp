/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2026-01-29 16:32:07 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib> // atoi
#include <csignal> // signal

static int	check_args(char** argv)
{
	// Check if port is a number between 1024 and 65535
	// Because ports range from 0 to 1023 are reserved for system use
	// and ports from 65536 and above are invalid
	for (size_t i = 0; argv[1][i]; i++)
	{
		if (!isdigit(argv[1][i]))
			return 1;
	}
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535)
		return 1;
	return 0;
}

int	main(int argc, char **argv)
{
	if (argc != 3 || check_args(argv))
	{
		std::cout << "Invalid number of arguments or port number. Please use 'ircserv port password' with a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << CGRE << "Initiating server." << CRST << std::endl;
	// Creation of the server object
	Server server(argv[2], atoi(argv[1]));
	try
	{
		// Set signal handlers for safe shutdown
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		// Run the server
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << CGRE << "Server shut down."  << CRST << std::endl;
	return 0;
}
