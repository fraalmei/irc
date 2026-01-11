/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 11:38:11 by p                 #+#    #+#             */
/*   Updated: 2026/01/11 11:33:53 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../include/prints.hpp"

int			Server::isChannelExist(const std::string &channelName) const
{
	std::map<std::string, Channel*>::const_iterator it = this->_channel_list.find(channelName);
	return (it != this->_channel_list.end());
}
