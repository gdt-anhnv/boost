#include "pch.h"
#include "tcp_socket.h"

tcp_connection::pointer tcp_connection::create(boost::asio::io_context & io)
{
	return pointer(new tcp_connection(io));
}

void tcp_connection::start()
{
	boost::asio::async_write(socket, boost::asio::buffer("Testing"),
		boost::bind(&tcp_connection::handle_write,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

boost::asio::ip::tcp::socket & tcp_connection::get_socket()
{
	return socket;
}

tcp_connection::tcp_connection(boost::asio::io_context & io) :
	socket(io)
{
}

void tcp_connection::handle_write(const boost::system::error_code & err, size_t s)
{
	std::cout << err << std::endl;
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
