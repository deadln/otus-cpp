#include "server.h"

void session::start()
{
    do_read();
}

void session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),  // Получаем строчку, и обрабатываем как обычно
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            async::receive(handle, data_, length);

            // do_write(length);
          }
          async::disconnect(handle);
        });
}

void session::do_write(std::size_t length)  // Делаем вывод в консоль а потом disconnect
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
          // async
        });
}

void server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            auto h = async::connect(bulk_size);
            std::make_shared<session>(std::move(socket), bulk_size, h)->start();  // Инициализировать connect и передать handle
          }

          do_accept();
        });
}