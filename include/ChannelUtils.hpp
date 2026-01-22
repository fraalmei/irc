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

/// @brief Utilidades para operaciones comunes con canales
class ChannelUtils
{
	public:
		/// @brief Normaliza nombre de canal agregando # si no lo tiene
		static std::string normalizeChannelName(const std::string& name);
		
		/// @brief Envía un mensaje a todos los miembros de un canal
		static void broadcastToChannel(Channel* channel, const std::string& message);
		
		/// @brief Envía un mensaje a todos los miembros excepto uno
		static void broadcastToChannelExcept(Channel* channel, const std::string& message, User* except);
		
	private:
		ChannelUtils();
		~ChannelUtils();
};

#endif
