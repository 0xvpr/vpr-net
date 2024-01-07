#include <iostream>

#include "vprnet.hpp"

int main() {
    auto server = vprnet::HttpServer::instance("vpr-net");

    server.set_endpoint( "/a", []() { std::cout << "A activated.\n"; }, vprnet::types::button);
    server.set_endpoint( "/b", []() { std::cout << "B activated.\n"; }, vprnet::types::button);
    server.set_endpoint( "/c", []() { std::cout << "C toggled.\n"; },   vprnet::types::toggle);
    server.set_endpoint( "/d", []() { std::cout << "D set.\n"; },       vprnet::types::field);

    server.serve();

    return 0;
}