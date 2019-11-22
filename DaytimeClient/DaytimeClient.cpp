// DaytimeClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "boost/array.hpp"
#include "boost/asio.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		std::cout << argc << std::endl;
		std::cout << argv[0] << std::endl;
		std::cout << argv[1] << std::endl;
		if (2 != argc)
			return 1;

		boost::asio::io_context io;
		boost::asio::ip::tcp::resolver resolver(io);
		boost::asio::ip::tcp::resolver::results_type epnts = resolver.resolve(argv[1], "8001");

		boost::asio::ip::tcp::socket socket(io);
		boost::asio::connect(socket, epnts);

		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			if (error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);

			std::cout.write(buf.data(), len);
		}
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}

	return 0;
}