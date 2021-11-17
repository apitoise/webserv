/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputPollListener.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 19:12:06 by abaur             #+#    #+#             */
/*   Updated: 2021/11/01 14:56:07 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutputPollListener.hpp"

#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"
#include <unistd.h>

namespace ft
{
	OutputPollListener::OutputPollListener(void) :
		fail(false),
		eof (false),
		buffer(),
		writeAmount(0),
		_file(NULL),
		_fd(-1),
		_pollfd(),
		_pollAction(NULL)
	{
		this->_pollfd.events = POLLOUT;
	}

	OutputPollListener::OutputPollListener(int fd){
		new(this) OutputPollListener();
		this->SetFd(fd);
	}

	OutputPollListener::OutputPollListener(std::FILE& file){
		new(this) OutputPollListener();
		this->SetFile(file);
	}

	OutputPollListener::~OutputPollListener() {}


	void	OutputPollListener::SetFd(int fd){
		this->_fd         = fd;
		this->_pollfd.fd  = fd;
		this->_pollAction = &OutputPollListener::WriteFd;
	}
	void	OutputPollListener::SetFile(std::FILE& file){
		this->_file       = &file;
		this->_pollfd.fd  = ::fileno(&file);
		this->_pollAction = &OutputPollListener::WriteFile;

		if (_pollfd.fd < 0)
			throw ft::ErrnoException("Invalid FILE* to fd conversion.");
	}



/******************************************************************************/
/* # PollListener Interface                                                   */
/******************************************************************************/

	void	OutputPollListener::GetPollFd(pollfd& outpfd){
		outpfd = this->_pollfd;
	}

	void	OutputPollListener::OnPollEvent(const pollfd& pfd){
		this->fail = false;
		this->eof  = false;
		if (buffer.empty())
			ft::clog << log::warning
			         << "Attempted to write an empty string through "
			         << "a PollListener"
			         << std::endl;
		else
			(this->*_pollAction)(pfd);
		// ft::clog << log::debug << "Write: " << writeAmount << ", Fail: " << fail
		//          << ", Eof: " << eof << std::endl;
	}

	void	OutputPollListener::WriteFd(const pollfd&){
		size_t	writelen = ::write(_fd, buffer.data(), buffer.size());

		if (writelen < 0)
			this->fail = true;
		else if (writelen == 0)
			this->eof = true;
		else {
			this->writeAmount += writelen;
			this->buffer.erase(0, writelen);
		}
	}

	void	OutputPollListener::WriteFile(const pollfd&){
		size_t	writelen = std::fwrite(buffer.data(), 1, buffer.size(), _file);

		this->fail = std::ferror(_file);
		this->eof  = std::feof(_file);
		this->writeAmount += writelen;
		this->buffer.erase(0, writelen);
	}

}
