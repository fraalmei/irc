/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2025/11/13 16:48:53 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include "Server.hpp"
#include <cstdlib> // atoi

static void leaks()
{
	system ("leaks -q ircserv");
}

static int	check_args(char** argv)
{
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535)
		return 1;
	std::cout << "Using port: " << argv[1] << std::endl; //Redundantes
	std::cout << "Using password: " << argv[2] << std::endl;
	return 0;
}

int	main(int argc, char **argv)
{
	atexit(leaks);
	if (argc != 3 || check_args(argv))
	{
		std::cerr << "Invalid number of arguments or port number. Please use 'ircserv password port' with a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << "Initiating server." << std::endl;
	Server server(argv[2], atoi(argv[1]));
	//Server server;
	server.run();
	std::cout << "Server distroyed." << std::endl;
	return 0;
}
