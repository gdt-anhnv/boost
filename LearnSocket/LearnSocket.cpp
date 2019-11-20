// LearnSocket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Source/funcs.h"

#include <iostream>
#include <thread>

int main()
{
	boost::asio::io_context io;
	Worker worker(io);
	std::thread thread(boost::bind(&boost::asio::io_context::run, &io));

	io.run();
	thread.join();
}