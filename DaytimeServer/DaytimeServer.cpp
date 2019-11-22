// DaytimeServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/asio.hpp"

#include <iostream>
#include <ctime>
#include <string>

std::string Daytime()
{
	std::time_t now = time(0);
	return std::ctime(&now);
}

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
private:
	boost::asio::ip::tcp::socket socket;
	std::string message;

	tcp_connection(boost::asio::io_context& io) :
		socket(io),
		message()
	{}

public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	static pointer create(boost::asio::io_context& io)
	{
		return pointer(new tcp_connection(io));
	}

	boost::asio::ip::tcp::socket& get_socket()
	{
		return socket;
	}

	void start()
	{
		message = Daytime();
		boost::asio::async_write(socket, boost::asio::buffer(message),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code&, size_t)
	{}
};

class tcp_server
{
	boost::asio::ip::tcp::acceptor accepter;
public:
	tcp_server(boost::asio::io_context& io) :
		accepter(io, boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v4(), 13))
	{
		start_accept();
	}

private:
	void handle_accept(tcp_connection::pointer conn,
		const boost::system::error_code& error)
	{
		if (!error)
			conn->start();

		start_accept();
	}

	void start_accept()
	{
		tcp_connection::pointer conn =
			tcp_connection::create(static_cast<boost::asio::io_context&>(accepter.get_executor().context()));
		accepter.async_accept(conn->get_socket(),
			boost::bind(&tcp_server::handle_accept,
				this, conn, boost::asio::placeholders::error));
	}
};

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
