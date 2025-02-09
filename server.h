#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "async.h"

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket, unsigned int bs, async::handle_t h)
    : socket_(std::move(socket)), bulk_size(bs), handle(h)
  {
  }

  void start();

private:
  void do_read();
  void do_write(std::size_t length);

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];

  unsigned int bulk_size;
  async::handle_t handle;
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port, unsigned int s)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), bulk_size(s)
  {
    do_accept();
  }

private:
  void do_accept();

  tcp::acceptor acceptor_;
  unsigned int bulk_size;
};
