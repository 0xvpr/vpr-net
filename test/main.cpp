#include <iostream>

#include "vprnet.hpp"

vprnet::void_callback a = []() -> void {
    std::cout << "A activated\n";
};

int main() {
    auto server = vprnet::HttpServer::instance("vpr-net example");

    server.set_endpoint( "/a", a, vprnet::types::button);
    server.set_endpoint( "/b", (vprnet::void_callback)[]() { std::cout << "B activated.\n"; }, vprnet::types::button);
    server.set_endpoint( "/c", (vprnet::void_callback)[]() { std::cout << "C toggled.\n"; },   vprnet::types::toggle);
    server.set_endpoint( "/d", (vprnet::i32_callback)[](auto value) { std::cout << "D set to " << value << "\n"; }, vprnet::types::i32_field);

    server.serve();

    return 0;
}