/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 00:00:00 by cagonzal          #+#    #+#             */
/*   Updated: 2026/01/23 11:01:38 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERUTILS_HPP
# define SERVERUTILS_HPP

# include <string>

/// @brief Utilidades estáticas para operaciones comunes del servidor
class ServerUtils
{
	public:
		// String utilities
		static void rtrimCrlf(std::string& s);
		
	private:
		ServerUtils();
		~ServerUtils();
};

#endif
