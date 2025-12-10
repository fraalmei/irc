/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 09:30:36 by p                 #+#    #+#             */
/*   Updated: 2025/12/10 14:41:03 by fraalmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTS_HPP
# define PRINTS_HPP

# include <string>
# include <iostream>
# include "colors.hpp"
# include "Server.hpp"

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
