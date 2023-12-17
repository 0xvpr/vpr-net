PROJECT   = vpr-net-test

CXX       = x86_64-w64-mingw32-g++-posix
CXX_FLAGS = -std=c++20 -Ofast \
            -Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion \
            -static

LD        = x86_64-w64-mingw32-g++-posix
LDFLAGS   = -lws2_32

BIN       = bin

all: $(PROJECT)

$(PROJECT): $(BIN)
$(PROJECT): $(BIN)/$(PROJECT).exe

$(BIN)/$(PROJECT).exe: test/main.cpp include/vprnet.hpp
	$(CXX) $(CXX_FLAGS) test/main.cpp $(LDFLAGS) -Iinclude -o $(BIN)/$(PROJECT).exe

test: $(BIN)/$(PROJECT).exe
	$(BIN)/$(PROJECT).exe

$(BIN):
	mkdir -p bin
