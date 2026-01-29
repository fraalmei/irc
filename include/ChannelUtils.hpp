/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/22 13:23:35 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELUTILS_HPP
# define CHANNELUTILS_HPP

# include <string>

class Channel;
class User;

/// @brief Utilities for common operations with channels
class ChannelUtils
{
	public:
		/// @brief Normalizes the channel name adding # if it doesn't start with it
		static std::string normalizeChannelName(const std::string& name);
		
		/// @brief Sends a message to all channel members
		static void broadcastToChannel(Channel* channel, const std::string& message);
		
		/// @brief Sends a message to all members of a chennel excepting one
		static void broadcastToChannelExcept(Channel* channel, const std::string& message, User* except);
		
	private:
		ChannelUtils();
		~ChannelUtils();
};

#endif
