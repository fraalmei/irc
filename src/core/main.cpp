/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2025/10/31 13:10:10 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include "Server.hpp"
#include <cstdlib> // atoi
#include <string>

static void leaks()
{
	system ("leaks -q ircserv");
}


int	read_args(char** argv)
{
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535)
	{
		std::cerr << "Invalid port number. Please use a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << "Using port: " << argv[1] << std::endl;
	if (argv[2])
		std::cout << "Using password: " << argv[2] << std::endl;
	return 0;
}

int	main(int argc, char **argv)
{
	atexit(leaks);
	if (argc != 3 && !read_args(argv))
		return 1;
	std::cout << "Initiating server." << std::endl;
	//Server server(argv[2], atoi(argv[1]));
	Server server;
	server.run();
	std::cout << "Server distroyed." << std::endl;
	return 0;
}
