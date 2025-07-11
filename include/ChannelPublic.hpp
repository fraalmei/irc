/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelPublic.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:50 by cagonzal          #+#    #+#             */
/*   Updated: 2025/05/23 13:13:31 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUBLIC_HPP
#define PUBLIC_HPP

#include "AChannel.hpp"

class ChannelPublic : public AChannel
{

public:

	ChannelPublic(const std::string &name);
	virtual ~ChannelPublic();

	virtual int							addMember(const User* user);
	virtual int							addMember(const User* user, std::string password);
	virtual void						removeMember(const std::string &nickname);
	virtual bool						isMember(const std::string &nickname) const;
	virtual bool						isMember(const int &fd) const;

	virtual const std::string			&getName() const;
	virtual const std::vector<User*>	&getMembers() const;

};

#endif // PUBLIC_HPP