/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/10/31 14:52:33 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int User::addToBuffer(const std::string& message)
{
	_buffer += message;
	
	if (message.empty() || message.rfind('\n\r', 0) != message.size() - 2)
		return 0;
	return _buffer.length();
}