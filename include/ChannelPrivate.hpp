/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelPrivate.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:48 by cagonzal          #+#    #+#             */
/*   Updated: 2025/07/11 12:36:04 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_HPP
#define PRIVATE_HPP

#include "AChannel.hpp"
#include <string>
#include <vector>

class ChannelPrivate : public AChannel
{
protected:
	std::string _password;

public:

	ChannelPrivate(const std::string& name, const std::string& password);
	~ChannelPrivate();

	int					addMember(const User* client);
	int					addMember(const User* client, const std::string& password);
	void				removeMember(const std::string& nickname);
	bool				isMember(const std::string& nickname) const;
	virtual bool		isMember(const int &fd) const;

	const std::string&	getName() const;
	const std::vector<User*>&	getMembers() const;
	const std::string&	getPassword() const;

	void setPassword(const std::string& pass);
};

#endif // PRIVATE_HPP
