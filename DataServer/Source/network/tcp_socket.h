#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
private:
	boost::asio::ip::tcp::socket socket;

public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	static pointer create(boost::asio::io_context& io);

	void start();
	boost::asio::ip::tcp::socket& get_socket();
private:
	tcp_connection(boost::asio::io_context& io);
	void handle_write(const boost::system::error_code&, size_t);
	void handle_read(const boost::system::error_code & err);
};

class tcp_server
{
private:
	boost::asio::ip::tcp::acceptor acceptor;
public:
	tcp_server(boost::asio::io_context& io);
private:
	void start_accept();
	void handle_accept(tcp_connection::pointer conn,
		const boost::system::error_code&);
};

#endif