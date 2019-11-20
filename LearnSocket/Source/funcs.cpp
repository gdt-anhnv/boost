#include "pch.h"
#include "funcs.h"

#include "boost/thread/thread.hpp"

#include <chrono>

void Worker::Job()
{
	if (count < 10)
	{
		std::cout << count << std::endl;
		++count;
		timer.expires_at(timer.expiry() + boost::asio::chrono::seconds(1));
		timer.async_wait(boost::asio::bind_executor(strand,
			boost::bind(&Worker::Job, this)));
	}
}

void Worker::Job2()
{
	if (count < 10)
	{
		std::cout << count << std::endl;
		++count;
		timer2.expires_at(timer2.expiry() + boost::asio::chrono::seconds(1));
		timer2.async_wait(boost::asio::bind_executor(strand,
			boost::bind(&Worker::Job2, this)));
	}
}

Worker::Worker(boost::asio::io_context & io) :
	strand(boost::asio::make_strand(io)),
	timer(io, boost::asio::chrono::seconds(1)),
	timer2(io, boost::asio::chrono::seconds(1)),
	count(0)
{
	timer.async_wait(boost::asio::bind_executor(strand,
		boost::bind(&Worker::Job, this)));
	timer2.async_wait(boost::asio::bind_executor(strand,
		boost::bind(&Worker::Job2, this)));
}

Worker::~Worker()
{
	std::cout << "Final count is: " << count << std::endl;
}
