#include <iostream>

#include "vprnet.hpp"

void function() {
    std::cout << "Endpoint Reached" << std::endl;
}

int main() {
    auto server = vprnet::HttpServer::instance();
    server.set_endpoint(
        "/test",
        function,
        vprnet::types::toggle
    );

    server.serve();

    return 0;
}