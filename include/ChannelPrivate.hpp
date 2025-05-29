/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelPrivate.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:10:48 by cagonzal          #+#    #+#             */
/*   Updated: 2025/05/29 21:50:51 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_HPP
#define PRIVATE_HPP

#include "AChannel.hpp"
#include "Client.hpp"
#include <string>
#include <vector>

class ChannelPrivate : public AChannel
{
protected:
	std::string _password;

public:

	ChannelPrivate(const std::string& name, const std::string& password);
	~ChannelPrivate();

	int					addMember(const Client* client, const std::string& password);
	void				removeMember(const std::string& nickname);
	bool				isMember(const std::string& nickname) const;
	virtual bool		isMember(const int &fd) const;

	const std::string&	getName() const;
	const std::vector<Client*>&	getMembers() const;
	const std::string&	getPassword() const;

	void setPassword(const std::string& pass);
};

#endif // PRIVATE_HPP
