// DataServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Source/network/tcp_socket.h"

#include <iostream>

int main()
{
	try
	{
		boost::asio::io_context io;
		tcp_server server(io);
		io.run();
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
