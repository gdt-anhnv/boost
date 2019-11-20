#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <iostream>
#include "boost/asio.hpp"
#include "boost/bind.hpp"

class Worker
{
private:
	boost::asio::steady_timer timer;
	boost::asio::steady_timer timer2;
	int count;
	boost::asio::strand<boost::asio::io_context::executor_type> strand;
public:
	Worker(boost::asio::io_context& io);
	~Worker();
private:
	void Job();
	void Job2();
};

#endif