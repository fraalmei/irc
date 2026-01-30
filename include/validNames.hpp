/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validNames.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samartin <samartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:10:48 by samartin          #+#    #+#             */
/*   Updated: 2026/01/30 09:27:22 by samartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDNAMES_HPP
# define VALIDNAMES_HPP

/*
 Whitespace alias.
*/
# ifndef WS
#  define WS " \n\r\t"
# endif

/*
 Client nicknames must not contain any of the single characters included in the
 NICKINVALID string and must not start with any of the single characters
 included in the NICKFIRSTINVALID string
*/
# ifndef NICKINVALID
#  define NICKINVALID " *,.!?@"
# endif
# ifndef NICKFIRSTINVALID
#  define NICKFIRSTINVALID ":~#&%+"
# endif

/*
 Channel names must not contain any of the single characters included in the
 CHNAMEINVALID string and must start with either one of the two characters
 identifying them as regular or local channel
*/
# ifndef CHNAMEINVALID
#  define CHNAMEINVALID " \7,"
# endif
# ifndef REGCH
#  define REGCH '#'
# endif
# ifndef LOCCH
#  define LOCCH '&'
# endif

#endif