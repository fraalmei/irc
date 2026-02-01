*This project has been created as part of the 42 curriculum by fraalmei, cagonzal, and samartin*

# Description: IRC Server
This project consists of the implementation of an IRC (Internet Relay Chat) server in C++98, using sockets and the TCP/IP protocol. For evaluation purposes, a widely used IRC client is utilized, such as HexChat, which is currently installed on the Macs in the 42 clusters.

## What is a socket?
A *socket* is an interface that allows communication between two programs over a network, such as the Internet or a local network.

You can think of it as a "digital plug" that connects two programs so they can send messages to each other.

In this project, we use *TCP sockets (SOCK_STREAM)*, which means a stable connection is established between the client and the server before exchanging data, ensuring the order and delivery of messages.

## How does our server work?
1. **Socket creation:**
   The server creates a socket that will act as the entry point for client connections.

2. **Binding to an IP and port:**
   We use `bind()` to assign that socket to an address (such as 127.0.0.1) and a port (6667, the standard for IRC).

3. **Listening for connections:**
   With `listen()`, the server enters a "waiting" state for new clients.

4. **Accepting clients:**
   Every time a client connects, we use `accept()` to establish a new dedicated connection for that specific client.

5. **Communication:**
   Through recv() and send(), the server receives messages from each client and responds to them.

6. **Disconnection:**
   When a client leaves, the socket is closed, and the server continues to run for the remaining clients.


# Instructions

## Using the server's functionalities
On the client side, commands are usually sent using `/`, while the server receives them "translated" by the client. For example, by typing `/join #mychannel` in the client, the server will receive the message `user42: JOIN #mychannel :`. When writing a message in a channel window that the user has already joined, they can simply type `Hello everyone`, while the server will receive `user42: PRIVMSG #mychannel :Hello everyone` and process the message to send it to all users present in that channel.

### Connection
The client software must attempt a TCP/IP connection to the IP where the server is running via the open port for that service. After detecting the new connection, the server will expect the following messages from the client in any order:

NICK <name>\r\n

USER <name>\r\n

PASS <password>\r\n

CAP <capabilities> (optional)

Our server can recognize the CAP message to send its own basic CAP response; however, performing capability negotiation is not part of this project.

If the client correctly sends the three required messages and the password matches the server's, the user will be recognized and can start sending commands to the server.

### Joining and leaving channels
The remaining commands required for this project are related to channels and the interaction between users and channels. The first two are for any user willing to enter or leave a channel.

The JOIN command is used to enter a channel. If the channel does not exist, a new one is created with that name, and the creator receives operator status for that channel. Entry may be restricted by channel modes (see below).

The PART command is used to leave a channel.

Managing channels: The following commands are for operators to maintain control over users within a channel.

The KICK command is used to eject a user from the channel. Since there is no BAN command implemented, the user can rejoin with a new JOIN.

The INVITE command sends an invitation notification from an operator already in a channel to another user. This invitation may be required if the channel mode is set to "invite-only."

The TOPIC command changes the automatic message sent to anyone joining the channel.

The MODE command changes attributes of the channel or a user's status within it. A flag with a + or - prefix must follow the channel name:
+i / -i: Adds or removes the restriction of needing an invitation to join the channel.
+t / -t: Protects or allows topic changes for operators only.
+k / -k: Adds or removes the requirement of a password to join. Must be followed by the password as a parameter if it is `+k`.
+o / -o: Grants or revokes operator status. Must be followed by the target username.
+l / -l: Adds or removes a user limit for the channel. It does not kick users if the limit is already exceeded but prevents new ones from joining unless the current count is below the limit. Must be followed by the limit figure as a parameter if it is `+l`.


# Resources

## Protocol Descriptions

**Primary: Modern IRC Protocol Description**
   https://modern.ircdocs.horse/

**Supplementary: Official 1993 Documentation, 2.1 specification**
   https://www.ietf.org/rfc/rfc1459.txt

## C++ Language Reference

**Primary: CPlusPlus**
   https://cplusplus.com/

**Supplementary: W3Schools**
   https://www.w3schools.com/cpp/

## Use of AI

Specific questions regarding C++ and the IRC protocol were directed to various AI services. No code snippets shown in example responses were copied directly into our codebase. Approximately one-third of this own document was drafted by AI.
