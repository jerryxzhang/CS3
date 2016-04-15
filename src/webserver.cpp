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

/* Citation: this function courtesy of the provided sample solution */
int getPort(const NginxConfig &config) {
  for (const auto& statement : config.statements_) {
    bool kl = true;
    for (const std::string& token : statement->tokens_) {
      if (!kl) {
        try { return stoi(token); } catch (...) {}
      }
      kl = (token != "port");
    }
  }
  return -1;
}


int main(int argc, char **argv) {
    /* Citation: these 14 lines courtesy of the provided sample solution */
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " config_file" << endl;
        return -1;
    }
    NginxConfigParser config_parser;
    NginxConfig config;
    if (!config_parser.Parse(argv[1], &config)) {
        return -1;
    }
    unsigned short port = getPort(config);
    if (port < 1) {
        cerr << "Couldn't find a port in the config file." << endl;
        return -1;
    }

    io_service ioservice;
    tcp::endpoint tcp_endpoint{tcp::v4(), port};
    tcp::acceptor tcp_acceptor{ioservice, tcp_endpoint};
    tcp::socket tcp_socket{ioservice};

    string content("HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: 40\r\n"
                   "\r\n"
                   "<html><body>Hello, worlds!</body></html>");

    /* These two functions are mutually recursive, so we have to declare them
    before we use them. */
    std::function<void (const boost::system::error_code)> accept_handler;
    std::function<void (const boost::system::error_code, std::size_t)> write_handler;

    /* Lambdas are perfect for this. The [&] captures by reference all
    (three) local variables that are used in the lambda. */
    accept_handler = [&] (const boost::system::error_code &ec) {
        if (ec) {
            cerr << "Error code " << ec.value() << endl;
            exit(1);
        }
        cout << "Serving request...";
        async_write(tcp_socket, buffer(content), write_handler);
    };
    write_handler = [&] (const boost::system::error_code &ec, std::size_t num_bytes) {
        if (ec) {
            cerr << "Error code " << ec.value() << endl;
            exit(1);
        }
        cout << "request served. Now listening again." << endl;
        tcp_socket.close();
        tcp_acceptor.async_accept(tcp_socket, accept_handler);
    };

    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, accept_handler);
    cout << "Server started, listening on port " << port << endl;
    ioservice.run();
    return 0;
}
