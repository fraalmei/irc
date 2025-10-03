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

	int							addMember(const User* user);
	int							addMember(const User* user, const std::string& password);
	void						removeMember(const std::string &nickname);
	bool						isMember(const std::string &nickname) const;
	bool						isMember(const int &fd) const;

	const std::string			&getName() const;
	const std::vector<User*>	&getMembers() const;
	const std::vector<User*>	&getAdminMembers() const;

};

#endif // PUBLIC_HPP