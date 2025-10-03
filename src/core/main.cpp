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

int	main(void)
{
	std::cout << "Initiating server." << std::endl;
	Server server;
	server.run();
	std::cout << "Server distroyed." << std::endl;
	return 0;
}
