/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OChannel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:53:50 by p                 #+#    #+#             */
/*   Updated: 2025/10/24 13:58:54 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <vector>
# include <string>
# include "User.hpp"

class OChannel
{
public:

	OChannel(std::string name) : _name(name), _topic(""), _password(""), _userLimit(0) {} // Public channel
	OChannel(std::string name, std::string topic) : _name(name), _topic(topic), _password(""), _userLimit(0) {} // Public channel
	OChannel(std::string name, std::string topic, std::string password) : _name(name), _topic(topic), _password(password), _userLimit(0) {} // Private channel
	~OChannel() {}
	
	int			addMember(const User* user);
	int			addMember(const User* user, const std::string& password);
	void		removeMember(const std::string &nickname);
	bool		isMember(const std::string &nickname) const;
	bool		isMember(const int &fd) const;
	
	const		std::string &getName() const;
	const		std::vector<User*> &getMembers() const;
	const		std::vector<User*> &getAdminMembers() const;

private:

	const std::string	_name; // Name as no command to change it after creation
	std::string			_topic;
	std::string			_password; // mode -k sets password to ""

	std::vector<User*>	_members; // List of the user sockets
	std::vector<User*>	_adminMembers; // List of the user sockets

	unsigned int		_userLimit; // User limit 0 = No Limit

	// Modes
	bool				_inviteOnly; // Invite only mode
	bool				_topicProtected; // Topic protected mode. Any user can change the topic when False
	
};

#endif
