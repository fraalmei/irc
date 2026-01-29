/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:53:50 by samartin          #+#    #+#             */
/*   Updated: 2025/10/31 14:34:06 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <vector>
# include <string>
# include "User.hpp"
# include "colors.hpp"

class User;

class Channel
{
	private:
	
		const std::string	_name;
		std::string			_topic;
		std::string			_password;
	
		std::vector<User*>	_members; // List of the user sockets
		std::vector<User*>	_adminMembers; // List of the users who are admin in this channel
		std::vector<User*>	_invitedUsers; // List of the users who are invited to this channel if it is in +i mode
	
		unsigned int		_userLimit; // User limit = 0 = No Limit
	
		// Modes
		bool				_inviteOnly; // Invite only mode
		bool				_topicProtected; // Topic protected mode
		
	public:

		// Constructors and Destructor
		Channel(std::string name) : _name(name), _topic(""), _password(""), _userLimit(0) {} // Public channel
		Channel(std::string name, std::string topic) : _name(name), _topic(topic), _password(""), _userLimit(0) {} // Public channel
		Channel(std::string name, std::string topic, std::string password) : _name(name), _topic(topic), _password(password), _userLimit(0) {} // Private channel
		~Channel() {}
		
		// Member management
		int			addMember(const User* user);
		int			addMember(const User* user, const std::string& password);
		int			addInvitation(const User* user);
		void		removeMember(const std::string &nickname);
		void		removeMember(User* user);
		void		removeInvitation(const User* user);
		bool		isMember(const std::string &nickname) const;
		bool		isMember(const int &fd) const;
		bool		isAdminMember(const std::string &nickname) const;
		bool		isInvited(const std::string &nickname) const;
		bool		isOperator(User* user) const;
		void		addOperator(User* user);
		void		removeOperator(User* user);
		
		// Getters
		const		std::string &getName() const{ return _name; }
		const		std::vector<User*> &getMembers() const{ return _members; }
		const		std::vector<User*> &getAdminMembers() const{ return _adminMembers; }
		const		std::vector<User*> &getInvitedUsers() const{ return _invitedUsers; }
		const		std::string &getTopic() const{ return _topic; }
		const		std::string &getPassword() const{ return _password; }
		unsigned int	getUserLimit() const{ return _userLimit; }
		bool		isInviteOnly() const{ return _inviteOnly; }
		bool		isTopicProtected() const{ return _topicProtected; }

		// Setters
		void		setTopic(const std::string &topic){ _topic = topic; }
		void		setPassword(const std::string &password){ _password = password; }
		void		setUserLimit(const unsigned int &limit){ _userLimit = limit; }
		void		setInviteOnly(const bool &mode){ _inviteOnly = mode; }
		void		setTopicProtected(const bool &mode){ _topicProtected = mode; }
		
	
};

#endif
