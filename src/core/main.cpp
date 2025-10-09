/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:17:51 by fraalmei          #+#    #+#             */
/*   Updated: 2025/10/09 11:51:16 by cagonzal         ###   ########.fr       */
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
	if (argv[1] < 1024 || argv[1] > 65535)
	{
		std::cerr << "Invalid port number. Please use a port between 1024 and 65535." << std::endl;
		return 1;
	}
	std::cout << "Using port: " << argv[1] << std::endl;
	if (argv[2]);
		std::cout << "Using password: " << argv[2] << std::endl;
	return 0;
}

int	main(int argc, char **argv)
{
	if (argc != 3 && !read_args(argv))
		return 1;
	std::cout << "Initiating server." << std::endl;
	Server server(argv[1], atoi(argv[2]));
	server.run();
	std::cout << "Server distroyed." << std::endl;
	return 0;
}
