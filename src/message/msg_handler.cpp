/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:39:07 by p                 #+#    #+#             */
/*   Updated: 2025/05/29 17:39:42 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msg_handler.hpp"

msg_handler::msg_handler() {}

msg_handler::~msg_handler() {}

template<>
std::string		msg_handler::handle_buffer<std::string>(char* buffer)
{
	(void) buffer;
	std::cout << "Handling buffer, return string." << std::endl;
	return "";
}

template<>
int				msg_handler::handle_buffer<int>(char* buffer)
{
	(void) buffer;
	std::cout << "Handling buffer, return int." << std::endl;
	return 0;
}
