/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   achannel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:53:50 by p                 #+#    #+#             */
/*   Updated: 2025/05/23 11:57:12 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <vector>
# include <string>
# include <map>
# include "client.hpp"

class	AChannel
{
	public:

		AChannel(std::string name);
		AChannel & operator=(const AChannel &assign);
		~AChannel();
	
	private:
		std::string			_name;
		std::map<int, Client>	_members;	//List of the users sokets

};

#endif
