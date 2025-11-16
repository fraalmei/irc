/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 16:02:08 by cagonzal          #+#    #+#             */
/*   Updated: 2025/11/16 16:04:35 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP
# define STRUCTS_HPP

#include <string>

struct command
{
	std::string		name;
	std::string		cmd;
	std::string*	params;	
};



#endif