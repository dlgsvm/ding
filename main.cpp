#include "root_certificates.hpp"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <queue>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;               // from <boost/asio/ssl.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

//------------------------------------------------------------------------------

// Report a failure
void
fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    websocket::stream<ssl::stream<tcp::socket>> ws_;
    boost::beast::multi_buffer buffer_;
    std::string host_;
    std::string port_;
    boost::asio::deadline_timer timer;
    boost::asio::io_context* m_ioc;

    std::queue<std::string> m_queue;
    std::queue<std::string> m_queue_;

public:
    // Resolver and socket require an io_context
    explicit
    session(boost::asio::io_context& ioc, ssl::context& ctx)
            : resolver_(ioc)
            , ws_(ioc, ctx),timer(ioc)
    {
        m_ioc = &ioc;
    }

    ~session()
    {
        closeconnect();
    }

    // Start the asynchronous operation
    int init(char const* host,
             char const* port,
             std::queue<std::string> tmp_queue)
    {
        host_ = host;
        port_= port;
        m_queue = tmp_queue;
        m_queue_ = tmp_queue;

        run(host_.c_str(),port_.c_str());

        m_ioc->run();
    }

    void
    run(
            char const* host,
            char const* port)
    {
        // Save these for later
        host_ = host;

        // Look up the domain name
        resolver_.async_resolve(
                host,
                port,
                std::bind(
                        &session::on_resolve,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2));
    }

    void
    on_resolve(
            boost::system::error_code ec,
            tcp::resolver::results_type results)
    {
        if (ec)
            return fail(ec, "resolve");

        // Make the connection on the IP address we get from a lookup
        boost::asio::async_connect(
                ws_.next_layer().next_layer(),
                results.begin(),
                results.end(),
                std::bind(
                        &session::on_connect,
                        shared_from_this(),
                        std::placeholders::_1));
    }

    void
    on_connect(boost::system::error_code ec)
    {
        if (ec) {
            on_reconnect();
        }

        // Perform the SSL handshake
        ws_.next_layer().async_handshake(
                ssl::stream_base::client,
                std::bind(
                        &session::on_ssl_handshake,
                        shared_from_this(),
                        std::placeholders::_1));
    }

    void
    on_ssl_handshake(boost::system::error_code ec)
    {
        if (ec)
        {
            on_reconnect();
        }

        // Perform the websocket handshake
        ws_.async_handshake(host_, "/market/tickers ",
                            std::bind(
                                    &session::on_handshake,
                                    shared_from_this(),
                                    std::placeholders::_1));
    }

    void
    on_handshake(boost::system::error_code ec)
    {
        if (ec)
        {
            on_reconnect();
        }


        // Send the message
        std::string text_ = m_queue_.front();
        m_queue_.pop();

        ws_.async_write(
                boost::asio::buffer(text_),
                std::bind(
                        &session::on_write,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2));

    }

    void
    on_write(
            boost::system::error_code ec,
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
        {
            on_reconnect();
        }


        // Read a message into our buffer
        ws_.async_read(
                buffer_,
                std::bind(
                        &session::on_read,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2));

    }

    void
    on_read(
            boost::system::error_code ec,
            std::size_t bytes_transferred)
    {
        try {

            boost::ignore_unused(bytes_transferred);

            if (ec) {
                on_reconnect();
            }

            std::stringstream ss_decomp;
            boost::iostreams::filtering_istream in;
            in.push(boost::iostreams::gzip_decompressor());
            auto tmp_data = boost::beast::buffers(buffer_.data());//boost::beast::buffers(buffer_.data());
            //auto tmp_data =  boost::beast::buffers_front(buffer_.data());

            std::stringstream ss_tmp;
            ss_tmp << (tmp_data);
            //std::cout<<ss_tmp.str()<<std::endl;
            in.push(ss_tmp);     //从文件中解压
            boost::iostreams::copy(in, ss_decomp);

            std::cout << "decompressor data:" << ss_decomp.str() << std::endl;

            buffer_.consume(buffer_.size());

            if (ss_decomp.str().find("ping") != ss_decomp.str().npos) {
                std::string str_ping = ss_decomp.str();
                std::size_t found = str_ping.find(":");
                std::string str_tmp_time = str_ping.substr((found + 1), (str_ping.size() - (found + 1)));
                std::string str_send = " {\"pong\": " + str_tmp_time + "}";

                ws_.async_write(
                        boost::asio::buffer(str_send),
                        std::bind(
                                &session::on_write,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));


            } else {

                /*timer.expires_from_now(boost::posix_time::seconds(5));
                timer.async_wait([&](boost::system::error_code const &ec) {
                    if (ec == boost::asio::error::operation_aborted)
                        return;

                    std::cout << "Cancelling underlying socket\n";
                    ws_.next_layer().next_layer().cancel();
                });*/

                if(!m_queue_.empty())
                {
                    std::string text_ = m_queue_.front();
                    m_queue_.pop();

                    ws_.async_write(
                            boost::asio::buffer(text_),
                            std::bind(
                                    &session::on_write,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
                }else {

                    ws_.async_read(
                            buffer_,
                            std::bind(
                                    &session::on_read,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
                }
            }
        }catch (std::exception e)
        {
            std::cout<<e.what()<<std::endl;
            on_reconnect();
        }

    }

    void closeconnect()
    {
        ws_.next_layer().next_layer().cancel();

        ws_.async_close(websocket::close_code::normal,
                std::bind(
                        &session::on_close,
                        shared_from_this(),
                        std::placeholders::_1));
    }

    void on_reconnect()
    {
        closeconnect();

        std::chrono::milliseconds timespan(3000); // or whatever
        std::this_thread::sleep_for(timespan);
        init(host_.c_str(),  port_.c_str(),m_queue);
    }

    void
    on_close(boost::system::error_code ec)
    {
        if (ec)
            return fail(ec, "close");

        // If we get here then the connection is closed gracefully

        // The buffers() function helps print a ConstBufferSequence
        std::cout << boost::beast::buffers(buffer_.data()) << std::endl;
    }
};

int main(int argc, char** argv)
{
    auto const host = "api.huobi.br.com";
    auto const port = "https";
    std::string  text =  "{\n"
                       "  \"sub\": \"market.btcusdt.detail\",\n"
                       "  \"id\": \"id1\"\n"
                       "}";
    std::string text1 =  "{\n"
                        "  \"sub\": \"market.ethusdt.detail\",\n"
                        "  \"id\": \"id1\"\n"
                        "}";

    std::queue<std::string> m_queue;
    m_queue.push(text);
    m_queue.push(text1);

    // The io_context is required for all I/O
    boost::asio::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ ssl::context::sslv23_client };

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Launch the asynchronous operation
    std::make_shared<session>(ioc, ctx)->init(host, port, m_queue);

    sleep(100);

    return EXIT_SUCCESS;
}

