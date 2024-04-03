from conan import ConanFile

from conan.tools.cmake import (
    CMakeToolchain,
    CMakeDeps,
    CMake,
    cmake_layout
)

class Vprnet(ConanFile):
    name            = "vpr-net"
    version         = "1.0.1"
    author          = "Your Name <your.email@example.com>"
    license         = "MIT"
    url             = "https://github.com/0xvpr/vpr-net"
    description     = "Brief description of my header-only library"
    topics          = ("http", "header-only", "library")
    no_copy_source  = True

    settings        = "os", "compiler", "build_type", "arch"
    options         = {
        "shared" : [True, False]
    }
    default_options = {
        "shared" : False
    }

    def source(self):
        self.run("git clone https://github.com/0vpr/vpr-net.git")

    def package_info(self):
        self.cpp_info.libs = ["vprnet"]

    def package_id(self):
        self.info.header_only = True

    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "vpr/vprnet.hpp", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def package(self):
        self.copy("vprnet.hpp", dst="vpr", src="vpr")
        cmake = CMake(self)
        cmake.install()