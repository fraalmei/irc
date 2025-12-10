/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: p <p@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 09:30:36 by p                 #+#    #+#             */
/*   Updated: 2025/12/10 10:37:56 by p                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTS_HPP
# define PRINTS_HPP

# include <string>
# include <iostream>
# include "colors.hpp"

class prints
{
    public:
        static void printchars(const std::string &message)
        {
            std::cout << CGRE << "[" << __FUNCTION__ << "]" << CRST << "Mensaje completo ascii: ";
            for (size_t i = 0; i < message.length(); i++)
                std::cout << int(message[i]) << " ";
            std::cout << std::endl;
        }

};

#endif
