/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AChannel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:54:02 by p                 #+#    #+#             */
/*   Updated: 2025/05/23 11:40:49 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/AChannel.hpp"

AChannel::AChannel(std::string name) : _name(name)
{
    // Constructor implementation
    // Initialize members or any other necessary setup
}
AChannel & AChannel::operator=(const AChannel &assign)
{
    if (this != &assign)
    {
        this->_name = assign._name;
        this->_members = assign._members;
    }
    return *this;
}