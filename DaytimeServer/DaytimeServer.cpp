// DaytimeServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "boost/asio.hpp"

#include <iostream>
#include <ctime>
#include <string>

std::string Daytime()
{
	std::time_t now = time(0);
	return std::ctime(&now);
}

int main()
{
	try
	{
		boost::asio::io_context io;
		boost::asio::ip::tcp::acceptor accepter(io, boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v4(), 13));
		for (;;)
		{
			boost::asio::ip::tcp::socket socket(io);
			accepter.accept(socket);

			std::string mess = Daytime();
			std::cout << mess << std::endl;

			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(mess), ignored_error);
		}
	}
	catch (...)
	{
		std::cout << "Server: Error" << std::endl;
	}

	return 0;
}
