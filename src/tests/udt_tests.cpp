#include <gtest/gtest.h>

#include <boost/thread.hpp>
#include <chrono>

#include <boost/asio/basic_deadline_timer.hpp>
#include <boost/asio/buffered_write_stream.hpp>
#include <boost/asio/ip/udp.hpp>

#include <boost/chrono.hpp>

#include <boost/log/trivial.hpp>

#include "tests/tests_helpers.h"
#include "tests/stream_helpers.h"
#include "tests/protocol_helpers.h"
#include "tests/endpoint_helpers.h"

#include "udt/connected_protocol/protocol.h"
#include "udt/ip/udt.h"

typedef ip::udt<> udt_protocol;

TEST(UDTTest, AsioProtocolTests) {
  TestAsioProtocol<udt_protocol>();
}

TEST(UDTTest, AsioEndpointTests) {
  TestAsioEndpoint<udt_protocol>();
}

TEST(UDTTest, UDTTestMultipleConnections) {
  udt_protocol::resolver::query acceptor_udt_query(boost::asio::ip::udp::v4(), "9000");
  udt_protocol::resolver::query client_udt_query("127.0.0.1", "9000");

  TestMultipleConnections<udt_protocol>(client_udt_query, acceptor_udt_query,
                                        20);
}

TEST(UDTTest, UDTProtocolTest) {
  udt_protocol::resolver::query acceptor_udt_query(boost::asio::ip::udp::v4(), "9000");
  udt_protocol::resolver::query client_udt_query("127.0.0.1", "9000");

  TestStreamProtocol<udt_protocol>(client_udt_query, acceptor_udt_query, 10);

  TestStreamProtocolFuture<udt_protocol>(client_udt_query, acceptor_udt_query);

  TestStreamProtocolSpawn<udt_protocol>(client_udt_query, acceptor_udt_query);
}

// TEST_F(UDTTestFixture, Coroutine) {
//  typedef boost::asio::ip::tcp tcp;
//
//  boost::asio::io_service io_service;
//  tcp::endpoint endpoint(tcp::v4(), 9000);
//  tcp::acceptor acceptor(io_service, endpoint);
//  acceptor.listen();
//  boost::asio::spawn(io_service, [&acceptor] (boost::asio::yield_context
//  yield) {
//    boost::system::error_code ec;
//
//    boost::asio::buffered_write_stream<tcp::socket>
//    write_stream(acceptor.get_io_service());
//    acceptor.async_accept(write_stream.next_layer(), yield[ec]);
//    if (!ec) {
//      std::array<uint8_t, 1024> buffer;
//      for (;;) {
//        write_stream.async_read_some(boost::asio::buffer(buffer), yield[ec]);
//      }
//    }
//  });
//
//  auto connect = [&io_service] () {
//    tcp::socket socket(io_service);
//    boost::system::error_code ec;
//    tcp::resolver resolver(io_service);
//    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "9000");
//    tcp::resolver::iterator iterator = resolver.resolve(query);
//
//    tcp::socket write_stream(io_service);
//    write_stream.connect(*iterator);
//    if (!ec) {
//      std::string test("test");
//      for (;;) {
//        write_stream.write_some(boost::asio::buffer(test), ec);
//      }
//    }
//  };
//
//  boost::thread_group threads;
//  threads.create_thread(connect);
//  for (uint16_t i = 1; i <= 2; ++i) {
//    threads.create_thread([&io_service]() { io_service.run(); });
//  }
//  threads.join_all();
//}

// TEST(UDTTest, TestTimer) {
//  typedef boost::chrono::high_resolution_clock clock;
//  typedef boost::asio::basic_waitable_timer<clock> clock_timer;
//
//  typedef boost::chrono::time_point<clock> clock_time_point;
//
//  boost::asio::io_service io_service;
//  std::array<long long, 10> values;
//  clock_time_point now;
//  clock_time_point last_now;
//  uint32_t count(0);
//
//  clock_timer timer(io_service);
//  std::function<void(const boost::system::error_code&)> wait_handler;
//  wait_handler = [&](const boost::system::error_code& ec) {
//    //std::cout << ec.value() << " : " << ec.message() << std::endl;
//    now = clock::now();
//    values[count] = boost::chrono::duration_cast<boost::chrono::microseconds>(
//                        now - last_now)
//                        .count();
//    ++count;
//    last_now = now;
//    if (count < 10 && !ec) {
//      //timer.expires_at(clock::now() + boost::chrono::microseconds(1));
//      timer.expires_from_now(boost::chrono::microseconds(10));
//      timer.async_wait(wait_handler);
//    } else {
//      for (auto& val : values) {
//        fprintf(stdout, "diff %i us\n", val);
//      }
//    }
//  };
//
//  boost::system::error_code ec;
//  wait_handler(ec);
//
//  boost::thread_group threads;
//  for (uint16_t i = 1; i <= 16; ++i) {
//    threads.create_thread([&io_service]() { io_service.run(); });
//  }
//
//  threads.join_all();
//}
