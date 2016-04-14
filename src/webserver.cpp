#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <ctime>
#include "config_parser.hpp"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

io_service ioservice;
tcp::endpoint tcp_endpoint{tcp::v4(), 80};
tcp::acceptor tcp_acceptor{ioservice, tcp_endpoint};
tcp::socket tcp_socket{ioservice};

string content("<html><body>Hello, world!</body></html>");

void write_handler(const error_code &ec, std::size_t num_bytes) {
    if (ec) {
        cerr << "Error code " << ec.value() << endl;
        exit(1);
    }
}

void accept_handler(const error_code &ec) {
    if (ec) {
        cerr << "Error code " << ec.value() << endl;
        exit(1);
    }
    async_write(tcp_socket, buffer(content), write_handler);
}

int main() {
    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, accept_handler);
    ioservice.run();
    return 0;
}
