/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:59:06 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 13:50:59 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIBFT_MISC_HPP
#define CLIBFT_MISC_HPP

#include <unistd.h>

namespace ft
{
	/**
	 * Deletes all pointers in a container.
	 * Null pointers are safely ignored.
	 */
	template <class C>
	void	DeleteContent(C& container){
		for (typename C::iterator it=container.begin(); it!=container.end(); it++)
			if (*it != NULL)
				delete *it;
	}

}

#endif
