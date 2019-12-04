#include "pch.h"
#include "tcp_socket.h"

#include <chrono>
#include <ctime>

tcp_connection::pointer tcp_connection::create(boost::asio::io_context & io)
{
	return pointer(new tcp_connection(io));
}

void tcp_connection::start()
{
	boost::asio::async_read_until(socket, buffer, "\n",
		boost::bind(&tcp_connection::handle_read,
			shared_from_this(),
			boost::asio::placeholders::error));
}

boost::asio::ip::tcp::socket & tcp_connection::get_socket()
{
	return socket;
}

tcp_connection::tcp_connection(boost::asio::io_context & io) :
	socket(io),
	buffer()
{
}

void tcp_connection::handle_write(const boost::system::error_code & err, size_t s)
{
}

void tcp_connection::handle_read(const boost::system::error_code & err)
{
	if (err)
	{
		socket.close();
		return;
	}

	std::istream stream(&buffer);
	std::string data;
	std::getline(stream, data);
	if (data.length() != 0)
	{
		if (0 == data.compare("data"))
		{
			std::cout << "data" << std::endl;
			write("recieved data\n");
		}
		if (0 == data.compare("data2"))
		{
			std::cout << "data2" << std::endl;
			write("recieved data2\n");
		}
	}
	else
		std::cout << "Error" << std::endl;
	buffer.consume(buffer.size());
	start();
}

void tcp_connection::write(std::string data)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket, boost::asio::buffer(data.c_str(), data.length()),
		[this, self](boost::system::error_code err, std::size_t)
	{
		auto time = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
		std::cout << std::ctime(&time) << std::endl;
	});
}

tcp_server::tcp_server(boost::asio::io_context & io) :
	acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001))
{
	start_accept();
}

void tcp_server::start_accept()
{
	tcp_connection::pointer conn = tcp_connection::create(
		static_cast<boost::asio::io_context&>(acceptor.get_executor().context()));
	acceptor.async_accept(conn->get_socket(),
		boost::bind(&tcp_server::handle_accept, this, conn,
			boost::asio::placeholders::error));
}

void tcp_server::handle_accept(
	tcp_connection::pointer conn,
	const boost::system::error_code & error)
{
	if (!error)
		conn->start();

	start_accept();
}
