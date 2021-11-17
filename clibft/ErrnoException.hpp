/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrnoException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:57:13 by abaur             #+#    #+#             */
/*   Updated: 2021/11/07 20:49:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRNOEXCEPTION_HPP
#define ERRNOEXCEPTION_HPP

#include <stdexcept>

namespace ft
{
	class ErrnoException : public std::runtime_error
	{
	public:
		ErrnoException() throw();
		ErrnoException(int errnum) throw();
		ErrnoException(int errnum, const std::string& context) throw();
		ErrnoException(const std::string& context) throw();
		~ErrnoException() throw();
	private:
		static std::string	What(int errnum, const std::string& context) throw();
	};
}

#endif
