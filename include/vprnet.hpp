#ifndef   VPRNET_HEADER
#define   VPRNET_HEADER

#include  <winsock2.h>
#include  <ws2tcpip.h>
#include  <windows.h>

#include  <mutex>
#include  <regex>
#include  <memory>

#include  <functional>
#include  <algorithm>
#include  <random>

#include  <sstream>
#include  <string>
#include  <vector>
#include  <map>

#include  <optional>
#include  <variant>

#include  <iostream>
#include  <fstream> // TODO logging
#include  <cstring>

#include  <cfloat>

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


    typedef std::uint32_t button_t;
    enum types : button_t {
        button                          = 0,
        toggle                          = 1,
        i32_field                       = 2,
        i64_field                       = 3,
        u32_field                       = 4,
        u64_field                       = 5,
        f32_field                       = 6,
        f64_field                       = 7,
        str_field                       = 8
    };

    typedef std::uint16_t status_t;
    enum http_status : status_t {
        continue_                       = 100,
        switching_protocols             = 101,
        processing                      = 102,
        early_hints                     = 103,
        ok                              = 200,
        created                         = 201,
        accepted                        = 202,
        non_authoritative_information   = 203,
        no_content                      = 204,
        reset_content                   = 205,
        partial_content                 = 206,
        multi_status                    = 207,
        already_reported                = 208,
        im_used                         = 226,
        multiple_choices                = 300,
        moved_permanently               = 301,
        found                           = 302,
        see_other                       = 303,
        not_modified                    = 304,
        use_proxy                       = 305,
        // unused                       = 306, // 306 is unused, so it's omitted
        temporary_redirect              = 307,
        permanent_redirect              = 308,
        bad_request                     = 400,
        unauthorized                    = 401,
        payment_required                = 402,
        forbidden                       = 403,
        not_found                       = 404,
        method_not_allowed              = 405,
        not_acceptable                  = 406,
        proxy_authentication_required   = 407,
        request_timeout                 = 408,
        conflict                        = 409,
        gone                            = 410,
        length_required                 = 411,
        precondition_failed             = 412,
        payload_too_large               = 413,
        uri_too_long                    = 414,
        unsupported_media_type          = 415,
        range_not_satisfiable           = 416,
        expectation_failed              = 417,
        im_a_teapot                     = 418,
        misdirected_request             = 421,
        unprocessable_entity            = 422,
        locked                          = 423,
        failed_dependency               = 424,
        too_early                       = 425,
        upgrade_required                = 426,
        precondition_required           = 428,
        too_many_requests               = 429,
        request_header_fields_too_large = 431,
        unavailable_for_legal_reasons   = 451,
        internal_server_error           = 500,
        not_implemented                 = 501,
        bad_gateway                     = 502,
        service_unavailable             = 503,
        gateway_timeout                 = 504,
        http_version_not_supported      = 505,
        variant_also_negotiates         = 506,
        insufficient_storage            = 507,
        loop_detected                   = 508,
        not_extended                    = 510,
        network_authentication_required = 511
    };

    using void_callback = std::function<void()>;
    using i32_callback  = std::function<void(std::int32_t)>;
    using i64_callback  = std::function<void(std::int64_t)>;
    using u32_callback  = std::function<void(std::uint32_t)>;
    using u64_callback  = std::function<void(std::uint64_t)>;
    using f32_callback  = std::function<void(std::float_t)>;
    using f64_callback  = std::function<void(std::double_t)>;
    using str_callback  = std::function<void(std::string)>;

    using variant_callback_t = std::variant<void_callback,
                                             i32_callback,
                                             i64_callback,
                                             u32_callback,
                                             u64_callback,
                                             f32_callback,
                                             f64_callback,
                                             str_callback>;

    typedef std::map<std::string,
                     std::pair<variant_callback_t, types>> endpoint_t;

    typedef std::shared_ptr<vprnet::HttpClient>   http_client_ptr;
    typedef std::unique_ptr<vprnet::HttpServer>   http_server_ptr;
    typedef std::shared_ptr<vprnet::HttpRequest>  http_request_ptr;
    typedef std::shared_ptr<vprnet::HttpResponse> http_response_ptr;

    namespace element {

        const std::string default_css =
            "    <style>\r\n"
            "      html, body\r\n"
            "      {\r\n"
            "        font-family: \"Courier New\", Courier, monospace;\r\n"
            "        height:      95%;\r\n"
            "        background:  #050505;\r\n"
            "        margin:      0;\r\n"
            "        padding:     0;\r\n"
            "        overflow-x:  hidden;\r\n"
            "      }\r\n"
            "      button, input\r\n"
            "      {\r\n"
            "        max-width:        40%;\r\n"
            "        background-color: #808080;\r\n"
            "        color:            #E0E0E0;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #player_cont\r\n"
            "      {\r\n"
            "        font-family: \"Courier New\", Courier, monospace;\r\n"
            "        text-decoration: none;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #snip_content_cell\r\n"
            "      {\r\n"
            "        text-decoration: none;\r\n"
            "        text-align: center;\r\n"
            "        vertical-align: middle;\r\n"
            "        color: #ffffff;\r\n"
            "        width: 40%;\r\n"
            "        max-width: 100%;\r\n"
            "        height: 80%;\r\n"
            "        margin: auto;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .sidebar\r\n"
            "      {\r\n"
            "        position: fixed;\r\n"
            "        top: 0;\r\n"
            "        left: 0;\r\n"
            "        height: 100%;\r\n"
            "        width: 200px;\r\n"
            "        max-width: 20%;\r\n"
            "        padding: 10px 14px;\r\n"
            "        background: #202020;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .sidebar .control_panel\r\n"
            "      {\r\n"
            "         display: flex;\r\n"
            "         align-items: center;\r\n"
            "      }\r\n"
            "      .sidebar .text\r\n"
            "      {\r\n"
            "        font-size: 16px;\r\n"
            "        font-weight: 400;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .sidebar .text .logo_text\r\n"
            "      {\r\n"
            "        font-size: 20px;\r\n"
            "        font-weight: 500;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .sidebar .text .logo_description\r\n"
            "      {\r\n"
            "        font-size: 14px;\r\n"
            "        font-weight: 400;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .sidebar header .toggle\r\n"
            "      {\r\n"
            "        position: absolute;\r\n"
            "        top: 6%;\r\n"
            "        right: -5%;\r\n"
            "        height: 25px;\r\n"
            "        width: 25px;\r\n"
            "        background: #404040;\r\n"
            "        color: #202020;\r\n"
            "        transform: translateY(-50%);\r\n"
            "        display: flex;\r\n"
            "        align-items: center;\r\n"
            "        justify-content: center;\r\n"
            "        border-radius: 50%;\r\n"
            "        border-color: #000000;"
            "        font-size: 22px\r\n"
            "      }\r\n"
            "      .snip_link\r\n"
            "      {\r\n"
            "        /* text-decoration: none; */\r\n"
            "        color: #ffffff;\r\n"
            "        font-size:1.0em;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .buy_link\r\n"
            "      {\r\n"
            "        color:#ffffff;\r\n"
            "        font-size:1.0em;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #snip_index_cell\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "        vertical-align:top;\r\n"
            "        height:4em; color: #eeeeee;\r\n"
            "      }\r\n"
            "      #next_cell,#prev_cell\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "        vertical-align:top;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #play_cont_cell, #return_cell,#rehero_cell\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "        vertical-align:top;\r\n"
            "        width: 33%;\r\n"
            "        white-space: nowrap;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #xchg_rax\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "        vertical-align:top;\r\n"
            "        white-space: nowrap;\r\n"
            "        color:#eeeeee;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #play_cont_cell\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "        white-space: nowrap;\r\n"
            "        color:#eeeeee;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #lower_table\r\n"
            "      {\r\n"
            "        height: 97%;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #lower_links_table\r\n"
            "      {\r\n"
            "        width: 97%;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #upper_table\r\n"
            "      {\r\n"
            "        height: 5%;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #lower_menu_cell\r\n"
            "      {\r\n"
            "        text-align: center;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #contact_cell\r\n"
            "      {\r\n"
            "        color: #333333;\r\n"
            "        text-align: center;\r\n"
            "      }\r\n"
            "      \r\n"
            "      #buy_comment\r\n"
            "      {\r\n"
            "        color:#eeeeee;\r\n"
            "        text-align:center;\r\n"
            "        background-color:#222222;\r\n"
            "        padding:0.8em;\r\n"
            "      }\r\n"
            "      \r\n"
            "      a:hover\r\n"
            "      {\r\n"
            "        color: #10ff10;\r\n"
            "      }\r\n"
            "      \r\n"
            "      table\r\n"
            "      {\r\n"
            "        margin-left: auto;\r\n"
            "        margin-right: auto;\r\n"
            "      }\r\n"
            "      \r\n"
            "      pre\r\n"
            "      {\r\n"
            "        font-size: 1.1em;\r\n"
            "      }\r\n"
            "      \r\n"
            "      .menu_link\r\n"
            "      {\r\n"
            "        /* .link_next_prev,.return_link { */\r\n"
            "        color: #8f8f8f;\r\n"
            "        font-size:1.0em;\r\n"
            "        /* text-decoration:none; */\r\n"
            "      }\r\n"
            "      \r\n"
            "      .link_disabled\r\n"
            "       {\r\n"
            "        color: #262626;\r\n"
            "        font-size:1.0em;\r\n"
            "        /* visibility:hidden; */\r\n"
            "      }\r\n"
            "    </style>\r\n";

        const std::string js =
            "  <script src=\"https://unpkg.com/boxicons@2.1.4/dist/boxicons.js\"></script>\r\n"
            "  <script>\r\n"
            "    function call_api(endpoint) {\r\n"
            "      fetch(endpoint)\r\n"
            "      .then(response => response.json())\r\n"
            "      .then(data => console.log(data))\r\n"
            "      .catch(error => console.error('Error:', error));\r\n"
            "    }\r\n"
            "  </script>\r\n";

        constexpr auto head = [](auto title) -> const std::string {
            std::stringstream ss;
            ss << "<!DOCTYPE html>\r\n"
                  "<html lang=\"en\">\r\n"
               << default_css
               << js
               << "  <head>\r\n"
                  "    <meta charset=\"UTF-8\">\r\n"
                  "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
                  "    <title id=\"snip_content_cell\">" << title << "</title>\r\n"
                  "  </head>\r\n";

            return ss.str();
        };

        const std::string left_sidebar = 
            "<nav class=\"sidebar\">\r\n"
            "  <header>\r\n"
            "    <div class=\"control_panel\">\r\n"
            "      <div class=\"logo_text\"\r\n"
            "        <span class=\"panel_title\" id=\"snip_content_cell\">VPR-NET</span>\r\n"
            "      </div>\r\n"
            "      <div class=\"logo_description\"\r\n"
            "        <span class=\"panel_description\" id=\"snip_content_cell\">Control Panel</span>\r\n"
            "      </div>\r\n"
            "    </div>\r\n"
            "\r\n"
            "    <i class=\"bx bx-cheveron-right toggle\"></i>\r\n"
            "  </header>\r\n"
            "</nav>\r\n"
            "<div class=\"sidebar_content\"\r\n"
            "</div>\r\n";
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
            case http_status::ok: {
                result << " OK";
                break;
            }
            case http_status::im_used: {
                result << " Error: I'm used";
                break;
            }
            case http_status::bad_request: {
                result << " Error: Bad request";
                break;
            }
            case http_status::not_found: {
                result << " Error: Not found";
                break;
            }
            case http_status::service_unavailable: {
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
    std::string     protocol_;
    std::uint16_t     status_;
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
    HttpResponse& operator = (const HttpResponse& other) = delete;
    HttpResponse(const HttpResponse&& other) = delete;
    HttpResponse& operator = (const HttpResponse&& other) = delete;

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
    typedef int socklen_t;
public:
    static inline HttpServer& instance(std::string title) noexcept {
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

    inline int serve() noexcept {
        while ((client_socket_ = accept(server_socket_, (sockaddr *)&client_addr_, &client_addr_length_)) != INVALID_SOCKET) {
            if (client_socket_ == INVALID_SOCKET) {
                continue;
            }

            char buffer[1024] = { 0 };
            recv(client_socket_, buffer, sizeof(buffer)-1, 0);
            std::string recv_str(buffer);

            if (!session_manager_.has_active_session()) {
                session_manager_.create_session();

                auto response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::ok ),
                    resources::Headers{
                        {
                            resources::Header( "Content-Type", "text/html" ),
                            resources::Header{ "Set-Cookie", "SessionID=" + session_manager_.active_session_id() + "; SameSite=Lax\r\n"}
                        }
                    },
                    generate_payload()
                );
                send(client_socket_, response.raw_data(), response.size(), 0);
                closesocket(client_socket_);

                continue;
            }


            if (!validate_session(recv_str)) {
                auto err_response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::im_used ),
                    resources::Headers{
                        {
                            resources::Header{ "Content-Type", "text/plain" },
                        }
                    },
                    std::string("Error 226. Currently in use. Disconnect from other session to proceed.\r\n")
                );
                send(client_socket_, err_response.raw_data(), err_response.size(), 0);
                closesocket(client_socket_);

                continue;
            }

            std::regex endpoint_pattern("GET /[a-zA-Z0-9]*");
            std::sregex_iterator endpoint_match(recv_str.begin(), recv_str.end(), endpoint_pattern);
            std::sregex_iterator endpoint_end;

            if (endpoint_match == endpoint_end) {
                auto err_response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::not_found ),
                    resources::Headers{
                        {
                            resources::Header{ "Content-Type", "text/plain" },
                            resources::Header{ "Set-Cookie", "SessionID=" + session_manager_.active_session_id() + "; SameSite=Lax\r\n"}
                        }
                    },
                    std::string("Error 404. Page not found.\r\n")
                );
                send(client_socket_, err_response.raw_data(), err_response.size(), 0);
                closesocket(client_socket_);

                continue;
            }

            std::string endpoint = std::smatch(*endpoint_match).str().substr(4);
            if (endpoint == "/" || endpoint == "/home")
            {
                auto response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::ok ),
                    resources::Headers{
                        {
                            resources::Header( "Content-Type", "text/html" ),
                            resources::Header{ "Set-Cookie", "SessionID=" + session_manager_.active_session_id() + "; SameSite=Lax\r\n"}
                        }
                    },
                    generate_payload()
                );
                send(client_socket_, response.raw_data(), response.size(), 0);
                closesocket(client_socket_);

                continue;
            }
            endpoint = std::smatch(*endpoint_match).str().substr(8);
            if (endpoint == "/terminate" )
            {
                session_manager_.end_session();
                auto response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::ok ),
                    resources::Headers{
                        {
                            resources::Header( "Content-Type", "text/html" ),
                        }
                    },
                    generate_payload()
                );
                send(client_socket_, response.raw_data(), response.size(), 0);
                closesocket(client_socket_);

                continue;
            }

            if (endpoints_.find(endpoint) == endpoints_.end()) {
                auto err_response = HttpResponse(
                    resources::StatusLine( "HTTP/1.1", http_status::not_found ),
                    resources::Headers{
                        {
                            resources::Header{ "Content-Type", "text/plain" },
                            resources::Header{ "Set-Cookie", "SessionID=" + session_manager_.active_session_id() + "; SameSite=Lax\r\n"}
                        }
                    },
                    std::string("Error 404. Page not found.\r\n")
                );
                send(client_socket_, err_response.raw_data(), err_response.size(), 0);
                closesocket(client_socket_);

                continue;
            }

            status_t status = http_status::ok;
            const auto type = endpoints_[endpoint].second;
            switch (type) {
                case types::button:
                case types::toggle:
                {
                    std::get<void_callback>(endpoints_[endpoint].first)();
                    break;
                }
                case types::i32_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const std::int32_t int_value = std::stoi(str_value);

                        std::get<i32_callback>(endpoints_[endpoint].first)(int_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                case types::u32_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const std::uint32_t int_value = std::stoul(str_value);

                        std::get<u32_callback>(endpoints_[endpoint].first)(int_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                case types::i64_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const std::int64_t int_value = std::stoll(str_value);

                        std::get<i64_callback>(endpoints_[endpoint].first)(int_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                case types::u64_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const std::uint64_t int_value = std::stoull(str_value);

                        std::get<u64_callback>(endpoints_[endpoint].first)(int_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                case types::f32_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const float float_value = std::stof(str_value);

                        std::get<f32_callback>(endpoints_[endpoint].first)(float_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                case types::f64_field:
                {
                    std::regex query_pattern(R"(\?value=(\d+))");
                    std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
                    std::smatch query_match;

                    if (std::regex_search(recv_str, query_match, query_pattern)) {
                        const std::string str_value = query_match[1];
                        const double double_value = std::stod(str_value);

                        std::get<f64_callback>(endpoints_[endpoint].first)(double_value);
                    } else {
                        status = http_status::bad_request;
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

            auto response = HttpResponse(
                resources::StatusLine( "HTTP/1.1", status ),
                resources::Headers{
                    {
                        resources::Header( "Content-Type", "text/html" ),
                        resources::Header{ "Set-Cookie", "SessionID=" + session_manager_.active_session_id() + "; SameSite=Lax\r\n"}
                    }
                },
                generate_payload()
            );
            send(client_socket_, response.raw_data(), response.size(), 0);

            closesocket(client_socket_);
        }

        return 1;
    }

    template <typename T>
    bool set_endpoint(const std::string& endpoint, T&& variant, const types type = toggle) {
        if (endpoints_.find(endpoint) != endpoints_.end()) {
            return false;
        }
        endpoints_[endpoint] = { variant, type };

        return true;
    }

protected:
    HttpServer(std::string title, std::uint16_t port = 47001)
      : title_              ( title ),
        port_               ( port ),
        last_status_        ( 0 ),
        wsaData_            ( { } ),
        server_addr_        ( { } ),
        server_socket_      ( INVALID_SOCKET ),
        client_addr_        ( { } ),
        client_socket_      ( INVALID_SOCKET ),
        client_addr_length_ ( sizeof(client_addr_) ),
        session_manager_    ( *this )
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
           << element::left_sidebar
           << "  <body>\r\n"
              "    <div id=\"snip_index_cell\" colspan=\"3\"><h2>" << title_ << "</h2>\r\n"
              "      <br>\r\n"
              "      <br>\r\n"
              "      <br>\r\n"
           << [&] () -> std::string {
                std::string body;
                body.reserve(16384);
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
                        case types::i32_field:
                        case types::i64_field:
                        case types::u32_field:
                        case types::u64_field:
                        {
                            body += "    <form id=\"snip_content_cell\">\r\n"
                                    "      <input colspan=\"3\" type=\"number\" name=\"value\" placeholder=\"Set " + endpoint.substr(1) + " value...\" onclick=\"call_api_with_value('" + endpoint + "')\">\r\n"
                                    "    </form>";
                            break;
                        }
                        case types::f32_field:
                        case types::f64_field: {
                            break;
                        }
                        case types::str_field: {
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    body += "    <br>\r\n";

                }
                body += "    <button id=\"snip_content_cell\" onclick=\"call_api('/terminate')\">terminate session</button>\r\n";

                return body;
           }()
           << "    </div>"
              "  </body>\r\n"
           << element::js
           << "</html>\r\n";

        return ss.str();
    }

    inline HttpServer operator = (const HttpServer& other) = delete;
private:
    inline bool validate_session(const std::string& recv_str) const {
        std::regex query_pattern(R"(Cookie: SessionID=([A-Za-z0-9]{256}))");
        std::sregex_iterator query(recv_str.begin(), recv_str.end(), query_pattern);
        std::smatch query_match;

        if (std::regex_search(recv_str, query_match, query_pattern)) {
            const std::string str_value = query_match[1];
        
            return str_value == session_manager_.active_session_id();
        }

        return false;
    }

    class SessionManager {
    private:
        const HttpServer&                http_server_;
        std::optional<std::string> active_session_id_;

    public:
        SessionManager(const HttpServer& http_server)
          : http_server_(http_server)
        {}

        SessionManager() = delete;
        SessionManager& operator = (const SessionManager& other) = delete;

        inline void generate_session_id() {
            const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<size_t> distribution(0, chars.size() - 1);

            std::string random_string(256, 0);
            for (size_t i = 0; i < random_string.size(); ++i) {
                random_string[i] = chars[distribution(generator)];
            }

            active_session_id_ = random_string;
        }

        std::string create_session() {
            std::lock_guard<std::mutex> lock(http_server_.mutex_);

            if (!active_session_id_.has_value() ) {
                generate_session_id();
            }

            return active_session_id_.value();
        }

        void end_session() {
            std::lock_guard<std::mutex> lock(http_server_.mutex_);
            if (active_session_id_.has_value() ) {
                active_session_id_.reset();
            }
        }

        bool has_active_session() const {
            std::lock_guard<std::mutex> lock(http_server_.mutex_);
            return active_session_id_.has_value();
        }

        const std::string& active_session_id() const { return active_session_id_.value(); }
    }; // SessionManager

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
    SessionManager                   session_manager_;

}; // HttpServer

class vprnet::HttpRequest {
public:
    // Methods to set request type, headers, body, etc.
};

#endif // VPRNET_HEADER