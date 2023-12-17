#ifndef   VPRNET_HEADER
#define   VPRNET_HEADER

#include <winsock2.h>
#include <windows.h>

#include <mutex>
#include <regex>
#include <memory>
#include <algorithm>
#include <functional>

#include <vector>
#include <sstream>
#include <string>
#include <map>

#include <optional>

#include <iostream>
#include <cstring>


namespace vprnet {
    class HttpClient;
    class HttpServer;
    class HttpRequest;
    class HttpResponse;

    namespace resources {
        class StatusLine;
        class Header;
        class Headers;
    } // namespace resources

    enum types : std::uint32_t {
        button = 0,
        toggle = 1,
        field  = 2
    };

    typedef int* socklen_t;
    typedef std::unordered_map<std::string, std::pair<std::function<void()>,std::uint32_t>> endpoint_t;

    typedef std::shared_ptr<vprnet::HttpClient>   http_client_ptr;
    typedef std::unique_ptr<vprnet::HttpServer>   http_server_ptr;
    typedef std::shared_ptr<vprnet::HttpRequest>  http_request_ptr;
    typedef std::shared_ptr<vprnet::HttpResponse> http_response_ptr;
} // namespace vprnet

//
//
//

class vprnet::resources::StatusLine {
public:
    StatusLine() = delete;
    StatusLine operator = (const StatusLine& other) = delete;
    StatusLine(std::string protocol, std::uint16_t status)
      : protocol_( protocol ),
        status_( status )
    {
    }

    std::string data() const {
        std::stringstream result;
        result << protocol_ << " " << status_;

        switch (status_)
        {
            case 200: {
                result << " OK";
            }
            default: {
                break;
            }
        }

        result << "\r\n";
        return result.str();
    }
private:
    std::string   protocol_;
    std::uint16_t   status_;
};

class vprnet::resources::Header {
public:
    Header(std::string field, std::string value)
      : field_(field),
        value_(value)
    {
    }

    std::string data() const {
        std::stringstream result;
        result << field_ << ": " << value_ << "\r\n";

        return result.str();
    }
private:
    const std::string field_;
    const std::string value_;
};

class vprnet::resources::Headers {
public:
    Headers() = delete;
    Headers operator = (const Headers& other) = delete;
    Headers(std::vector<Header> headers)
      : headers_( headers )
    {
        std::stringstream result;
        for (auto header : headers_) {
            result << header.data();

        }
        
        data_ = result.str();
    }

    std::string data() const {
        return data_;
    }
private:
    std::vector<Header> headers_;
    std::string            data_;
};

class vprnet::HttpClient {
public:
    HttpResponse sendRequest(const HttpRequest& request);
    // Other utility functions.
};

class vprnet::HttpResponse {
public:
    HttpResponse() = delete;
    HttpResponse(const HttpResponse& other) = delete;

    HttpResponse(const resources::StatusLine&& status_line, resources::Headers headers, const std::string&& payload)
    : status_line_ ( status_line ),
      headers_         ( headers ),
      payload_         ( payload ),
      size_            ( static_cast<std::int32_t>( payload.size() ) )
    {
        std::stringstream result;
        result << status_line_.data()
               << headers_.data()
               << resources::Header( "Content-Length", std::to_string(size_) ).data()
               << "\r\n"
               << payload_.data();

        data_ = result.str();
    }

    const std::string data() const noexcept {
        return data_;
    }

    const char* raw_data() const noexcept {
        return data_.c_str();
    }

    std::int32_t size() const noexcept {
        return static_cast<std::int32_t>(data_.size());
    }
private:
    resources::StatusLine status_line_;
    resources::Headers        headers_;
    std::string               payload_;
    std::int32_t                 size_;
    std::string                  data_;
};

// Optionally, HTTP Server components
class vprnet::HttpServer {
public:
    static inline HttpServer& instance() {
        std::lock_guard<std::mutex> lock( mutex_ );
        if (!instance_) {
            instance_ = http_server_ptr( new HttpServer() );
        }

        return *instance_;
    }
    ~HttpServer() {
        if (server_socket_ > 0) {
            closesocket(server_socket_);
        }

        if (client_socket_ > 0) {
            closesocket(client_socket_);
        }

        WSACleanup();
    }

    inline int serve() {
        while((client_socket_ = accept(server_socket_, (sockaddr *)&client_addr_, client_addr_length_)) != INVALID_SOCKET) {
            if (client_socket_ == INVALID_SOCKET) {
                continue;
            }

            char buffer[64] = { 0 };
            recv(client_socket_, buffer, sizeof(buffer)-1, 0);
            std::string recv_str(buffer);


            std::regex pattern("GET /[a-zA-z0-9]*");
            std::sregex_iterator match(recv_str.begin(), recv_str.end(), pattern);
            std::sregex_iterator end;

            if (match != end) {
                std::string endpoint = std::smatch(*match).str().substr(4);
                if (endpoints_.find(endpoint) != endpoints_.end()) {
                    endpoints_[endpoint].first();
                }
            }

            auto response = HttpResponse(
                resources::StatusLine( "HTTP/1.1", 200 ),
                resources::Headers{
                    { resources::Header( "Content-Type", "text/html" ) }
                },
                std::string(
                    "<!DOCTYPE html>\n"
                    "<html lang=\"en\">\n"
                    "<head>\n"
                    "    <meta charset=\"UTF-8\">\n"
                    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                    "    <title>API Call Example</title>\n"
                    "    <script>\n"
                    "        function callApi() {\n"
                    "            fetch('/test')\n"
                    "                .then(response => response.json())\n"
                    "                .then(data => console.log(data))\n"
                    "                .catch(error => console.error('Error:', error));\n"
                    "        }\n"
                    "    </script>\n"
                    "</head>\n"
                    "<body>\n"
                    "    <button onclick=\"callApi()\">Call API</button>\n"
                    "</body>\n"
                    "</html>\n"
                )
            );

            send(client_socket_, response.raw_data(), response.size(), 0);
        }

        return 1;
    }

    bool set_endpoint(const std::string endpoint, std::function<void()> function, const std::uint32_t type) {
        if (endpoints_.find(endpoint) != endpoints_.end()) {
            return false;
        }
        endpoints_[endpoint] = { function, type };

        return true;
    }
protected:
    HttpServer()
      : port_                   ( 47001 ),
        last_status_                ( 0 ),
        wsaData_                  ( { } ),
        server_addr_              ( { } ),
        server_socket_ ( INVALID_SOCKET ),
        client_addr_              ( { } ),
        client_socket_ ( INVALID_SOCKET ),
        client_addr_length_   ( nullptr )
    {
        const auto wVersionRequested = MAKEWORD(2, 2);
        if ( (last_status_ = WSAStartup( wVersionRequested, &wsaData_ )) != EXIT_SUCCESS) {
            last_status_ = -1;
            return;
        }

        server_addr_.sin_family      = AF_INET;
        server_addr_.sin_port        = htons( port_ );
        server_addr_.sin_addr.s_addr = INADDR_ANY;

        if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
            last_status_ = -1;
            return;
        }

        if ((last_status_ = bind(server_socket_, (sockaddr *)(&server_addr_), sizeof(server_addr_))) == SOCKET_ERROR) {
            return;
        }

        if ((last_status_ = listen(server_socket_, 5)) == SOCKET_ERROR) {
            return;
        }
    }

    inline HttpServer operator = (const HttpServer& other) = delete;
private:
    static inline std::mutex                   mutex_;
    static inline http_server_ptr           instance_;
    const std::uint16_t                         port_;
    mutable std::int32_t                 last_status_;
    WSADATA                                  wsaData_;
    SOCKADDR_IN                          server_addr_;
    mutable SOCKET                     server_socket_;
    SOCKADDR_IN                          client_addr_;
    mutable SOCKET                     client_socket_;
    socklen_t                     client_addr_length_;
    endpoint_t                             endpoints_;
};

class vprnet::HttpRequest {
public:
    // Methods to set request type, headers, body, etc.
};


#endif // VPRNET_HEADER