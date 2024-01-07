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
    typedef std::map<std::string, std::pair<std::function<void()>,std::uint32_t>> endpoint_t;

    typedef std::shared_ptr<vprnet::HttpClient>   http_client_ptr;
    typedef std::unique_ptr<vprnet::HttpServer>   http_server_ptr;
    typedef std::shared_ptr<vprnet::HttpRequest>  http_request_ptr;
    typedef std::shared_ptr<vprnet::HttpResponse> http_response_ptr;

    namespace element {
        auto head = [](auto title) -> const std::string {
            std::stringstream ss;
            ss << "<!DOCTYPE html>\r\n"
                  "<html lang=\"en\">\r\n"
                  "  <head>\r\n"
                  "    <style>\r\n"
                  "      html,body {\r\n"
                  "          font-family: \"Courier New\", Courier, monospace;\r\n"
                  "          height: 95%;\r\n"
                  "          background: #050505;\r\n"
                  "          margin: 0;\r\n"
                  "          padding: 0;\r\n"
                  "          overflow-x: hidden;\r\n"
                  "      }\r\n"
                  "      button {\r\n"
                  "          background-color: #808080;\r\n"
                  "          color: #E0E0E0;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #player_cont {\r\n"
                  "          font-family: \"Courier New\", Courier, monospace;\r\n"
                  "          text-decoration: none;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #snip_content_cell {\r\n"
                  "          text-decoration: none;\r\n"
                  "          text-align: center;\r\n"
                  "          vertical-align: center;\r\n"
                  "          color: #ffffff;\r\n"
                  "          width: 40%;\r\n"
                  "          max-width: 100%;\r\n"
                  "          height: 80%;\r\n"
                  "          margin: auto;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      \r\n"
                  "      .snip_link {\r\n"
                  "          /* text-decoration: none; */\r\n"
                  "          color: #ffffff;\r\n"
                  "          font-size:1.0em;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      .buy_link {\r\n"
                  "          color:#ffffff;\r\n"
                  "          font-size:1.0em;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #snip_index_cell {\r\n"
                  "          text-align: center;\r\n"
                  "          vertical-align:top;\r\n"
                  "          height:4em; color: #eeeeee;\r\n"
                  "      }\r\n"
                  "      #next_cell,#prev_cell {\r\n"
                  "          text-align: center;\r\n"
                  "          vertical-align:top;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #play_cont_cell, #return_cell,#rehero_cell {\r\n"
                  "          text-align: center;\r\n"
                  "          vertical-align:top;\r\n"
                  "          width: 33%;\r\n"
                  "          white-space: nowrap;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #xchg_rax {\r\n"
                  "          text-align: center;\r\n"
                  "          vertical-align:top;\r\n"
                  "          white-space: nowrap;\r\n"
                  "          color:#eeeeee;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #play_cont_cell {\r\n"
                  "          text-align: center;\r\n"
                  "          white-space: nowrap;\r\n"
                  "          color:#eeeeee;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      \r\n"
                  "      #lower_table {\r\n"
                  "          height: 97%;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #lower_links_table {\r\n"
                  "          width: 97%;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #upper_table {\r\n"
                  "          height: 5%;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #lower_menu_cell {\r\n"
                  "          text-align: center;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #contact_cell {\r\n"
                  "          color: #333333;\r\n"
                  "          text-align: center;\r\n"
                  "      \r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      #buy_comment {\r\n"
                  "          color:#eeeeee;\r\n"
                  "          text-align:center;\r\n"
                  "          background-color:#222222;\r\n"
                  "          padding:0.8em;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      a:hover {\r\n"
                  "          color: #10ff10;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      table {\r\n"
                  "          margin-left: auto;\r\n"
                  "          margin-right: auto;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      pre {\r\n"
                  "          font-size: 1.2em;\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      .menu_link {\r\n"
                  "      /* .link_next_prev,.return_link { */\r\n"
                  "          color: #8f8f8f;\r\n"
                  "          font-size:1.0em;\r\n"
                  "          /* text-decoration:none; */\r\n"
                  "      }\r\n"
                  "      \r\n"
                  "      .link_disabled {\r\n"
                  "          color: #262626;\r\n"
                  "          font-size:1.0em;\r\n"
                  "          /* visibility:hidden; */\r\n"
                  "      }\r\n"
                  "    </style>\r\n"
                  "    <meta charset=\"UTF-8\">\r\n"
                  "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
                  "    <title id=\"snip_content_cell\">" << title << "</title>\r\n"
                  "  </head>\r\n";

            return ss.str();
        };

        const std::string js =
            "  <script>\r\n"
            "    function call_api(endpoint) {\r\n"
            "      fetch(endpoint)\r\n"
            "      .then(response => response.json())\r\n"
            "      .then(data => console.log(data))\r\n"
            "      .catch(error => console.error('Error:', error));\r\n"
            "    }\r\n"
            "  </script>\r\n";
    } // namespace element
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
                break;
            }
            case 404: {
                result << " Error: Not Found";
                break;
            }
            case 503: {
                result << " Error: Service unavailable";
                break;
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
    HttpResponse send_request(const HttpRequest& request);
    // Other utility functions.
};

class vprnet::HttpResponse {
public:
    HttpResponse() = delete;
    HttpResponse(const HttpResponse& other) = delete;

    HttpResponse(const resources::StatusLine&& status_line, resources::Headers headers, const std::string&& payload)
    : status_line_ ( status_line ),
      headers_     ( headers ),
      payload_     ( payload ),
      size_        ( static_cast<std::int32_t>( payload.size() ) )
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
    static inline HttpServer& instance(std::string title) {
        std::lock_guard<std::mutex> lock( mutex_ );
        if (!instance_) {
            instance_ = http_server_ptr( new HttpServer(title) );
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
        while ((client_socket_ = accept(server_socket_, (sockaddr *)&client_addr_, client_addr_length_)) != INVALID_SOCKET) {
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
                } else if ( endpoint != "/" ){
                    auto response = HttpResponse(
                        resources::StatusLine( "HTTP/1.1", 404 ),
                        resources::Headers{
                            {
                                resources::Header{ "Content-Type", "text/plain" }
                            }
                        },
                        std::string("Error 404. Page not found.\r\n")
                    );
                    send(client_socket_, response.raw_data(), response.size(), 0);
                    closesocket(client_socket_);

                    continue;
                }
            } else {
                closesocket(client_socket_);
                
                continue;
            }

            auto response = HttpResponse(
                resources::StatusLine( "HTTP/1.1", 200 ),
                resources::Headers{
                    {
                        resources::Header( "Content-Type", "text/html" )
                    }
                },
                generate_payload()
            );

            send(client_socket_, response.raw_data(), response.size(), 0);
            closesocket(client_socket_);
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
    HttpServer(std::string title)
      : title_              ( title ),
        port_               ( 47001 ),
        last_status_        ( 0 ),
        wsaData_            ( { } ),
        server_addr_        ( { } ),
        server_socket_      ( INVALID_SOCKET ),
        client_addr_        ( { } ),
        client_socket_      ( INVALID_SOCKET ),
        client_addr_length_ ( nullptr )
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

    std::string generate_payload() const {
        std::stringstream ss;

        ss << element::head( title_ )
           << "  <body>\r\n"
              "    <div id=\"snip_index_cell\" colspan=\"3\"><h2>" << title_ << "</h2>\r\n"
              "      <br>\r\n"
              "      <br>\r\n"
              "      <br>\r\n"
            //   "      <table colspan=\"2\">\r\n"
           << [&] () -> std::string {
                  std::string body;
                  body.reserve(2048);
                  for (const auto& [endpoint, pair] : endpoints_) {
                      const auto type = pair.second;
                      switch (type) {
                          case types::button:
                          {
                              body += "    <button id=\"snip_content_cell\" onclick=\"call_api('" + endpoint + "')\">Activate " + endpoint.substr(1) + "</button>\r\n";
                              break;
                          }
                          case types::toggle:
                          {
                              body += "    <button id=\"snip_content_cell\" onclick=\"call_api('" + endpoint + "')\">Toggle " + endpoint.substr(1) + "</button>\r\n";
                              break;
                          }
                          case types::field:
                          {
                              body += "    <form id=\"snip_content_cell\">\r\n"
                                      "      <input type=\"number\" name=\"number_field\" placeholder=\"Set " + endpoint.substr(1) + " value...\" onclick=\"call_api_with_value('" + endpoint + "')\">\r\n"
                                      "    </form>";
                              break;
                          }
                          default:
                          {
                              break;
                          }
                      }

                      body += "    <br>\r\n";
                  }

                  return body;
              }()
        //    << "      </table>"
           << "    </div>"
              "  </body>\r\n"
           << element::js
           << "</html>\r\n";

        return ss.str();
    }

    inline HttpServer operator = (const HttpServer& other) = delete;
private:
    static inline std::mutex                   mutex_;
    static inline http_server_ptr           instance_;
    const std::string                          title_;
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