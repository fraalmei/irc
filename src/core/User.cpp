/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:28:00 by p                 #+#    #+#             */
/*   Updated: 2025/10/31 14:47:46 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

void User::addToBuffer(const std::string& message)
{
	if (message.empty() || message.rfind('\n\r', 0) == 0)
		return;
	_buffer += message;
}