/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AChannel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:53:50 by p                 #+#    #+#             */
/*   Updated: 2025/07/11 12:23:06 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <vector>
# include <string>
# include "Client.hpp"

class AChannel
{
public:
	
	virtual int			addMember(const User* client, std::string password) = 0;
	virtual void		removeMember(const std::string &nickname) = 0;
	virtual bool		isMember(const std::string &nickname) const = 0;
	virtual bool		isMember(const int &fd) const = 0;
	
	virtual const		std::string &getName() const = 0;
	virtual const		std::vector<User*> &getMembers() const = 0;
	
protected:

	AChannel(std::string name) : _name(name) {}
	virtual ~AChannel() {}
	std::string				_name;
	std::vector<User*>	_members; // List of the user sockets
};

#endif
