<h1 align="center">vpr-net</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Windows-supported-44CC11"/>
  <!-- <img src="https://img.shields.io/badge/Mac-supported-44CC11"/>
  <img src="https://img.shields.io/badge/Linux-supported-44CC11"/> -->
  <a href="https://mit-license.org/"/>
    <img src="https://img.shields.io/badge/License-MIT-44CC11"/>
  </a>
  <!-- <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/vpr-omega-zero/build.yml?label=Build"/>
</p> -->
<br>

This project aims to provide a header only library to provide implementation of  
a RESTful via C++20.

### Example implementation:
(the following can be found in [test/main.cpp](#test/main.cpp))

```cpp
#include <iostream>

#include "vprnet.hpp"

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
        },
        vprnet::types::toggle
    );

    server.set_endpoint( "/d", (vprnet::i32_callback)[](auto value) {
            std::cout << "C set to " << value << "\n";
        },
        vprnet::types::i32_field
    );

    server.serve();

    return 0;
}
```
