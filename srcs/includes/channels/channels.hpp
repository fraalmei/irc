/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:53:50 by p                 #+#    #+#             */
/*   Updated: 2025/05/19 21:09:22 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <vector>
# include <string>
# include <map>

// Structure of the channel
// Estructura del canal
struct	Channel
{
	std::string			name;
	std::vector<int>	members;	//List of the users sokets
};

class	channels
{
	public:

		channels(/* args */);
		~channels();
	
	private:



};

#endif
