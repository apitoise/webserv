/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeoutManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 18:04:53 by abaur             #+#    #+#             */
/*   Updated: 2021/11/07 19:22:17 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TimeoutManager.hpp"

#include "logutil/logutil.hpp"
#include <stdexcept>

namespace ft
{
	TimeoutManager::ListenerMap TimeoutManager::_listeners;

	static time_t	GetWakeTime(unsigned int timeout_seconds){
		return std::time(NULL) + timeout_seconds;
	}


	void	TimeoutManager::AddListener(ITimeoutListener& listener, unsigned int timeout){
		if (_listeners.count(&listener))
			ft::clog << log::info << "Listener already registered to the timeout"
			            " manager, it will be replaced." << std::endl;

		_listeners[&listener] = GetWakeTime(timeout);
	}

	void	TimeoutManager::RemoveListener(ITimeoutListener& listener, bool warn){
		if (!_listeners.erase(&listener) && warn)
			ft::clog << log::warning << "Attempted to remove a listener which "
			            "was not registered to the timeout manager." << std::endl;
	}

	void	TimeoutManager::DeleteAll(){
		const ListenerMap lstnr(_listeners);
		for (ListenerMap::const_iterator it=lstnr.begin(); it!=lstnr.end(); it++){
			if (it->first != NULL) {
				TimeoutManager::RemoveListener(*it->first);
				delete it->first;
			} else {
				ft::clog << log::error << "NULL pointer found astray in the Timeout Manager." << std::endl;
			}
		}

		if (_listeners.size() > 0)
			ft::clog << log::error << "New TimeoutListeners registered themselves during cleanup process." << std::endl;
	}

	bool	TimeoutManager::TimeLoop(){
		const ListenerMap	listeners(_listeners);
		bool r = false;

		for (ListenerMap::const_iterator it=listeners.begin(); it!=listeners.end(); it++)
		{
			ITimeoutListener& ls = *it->first;
			if (it->second < std::time(NULL)) {
				r = true;
				ft::clog << log::event << "Timeout Event" << std::endl;
				TimeoutManager::RemoveListener(ls);
				try {
					ls.OnTimeout();
				}
				catch (const std::exception& e){
					ft::clog << log::error << "Uncaught exception during timeout event: \n"
					          << e.what()
					          << std::endl;
				}
			}
		}
		return r;
	}
}
