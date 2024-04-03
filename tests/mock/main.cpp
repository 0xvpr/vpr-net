#include "vpr/vprnet.hpp"

#include <iostream>

vprnet::void_callback a = []() -> void {
    std::cout << "A action performed\n";
};

int main() {
    auto server = vprnet::HttpServer::instance("vpr-net example");

    server.set_endpoint( "/a", a, vprnet::types::button);

    bool b_toggle = false;
    server.set_endpoint( "/b", (vprnet::void_callback)[&]() {
            b_toggle = !b_toggle;
            std::cout << "B " << (b_toggle ? "activated" : "deactivated") << "\n";
        }
        // Defaults to vprnet::types::toggle
    );

    server.set_endpoint( "/c", (vprnet::u64_callback)[](std::uint64_t value) {
            std::cout << "C set to " << value << "\n";
        },
        vprnet::types::u64_field
    );

    server.set_endpoint( "/d", (vprnet::f64_callback)[](double value) {
            std::cout << "D set to " << value << "\n";
        },
        vprnet::types::f64_field
    );

    server.serve();

    return 0;
}