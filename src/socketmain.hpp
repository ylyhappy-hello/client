#pragma once
#include <arpa/inet.h>
#include <atomic>
#include <chrono>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "config/configuration.hpp"
#include "utils/err.hpp"
namespace SocketClient {

class SocketClinetMain {
public:
  void init();
  void start_polling();
  void join_polling_thead();
  void stop_polling();
  void send_msg(std::string ation, std::string msg);
  void send_post(std::string payload);
  static SocketClinetMain* get_instance();

  ~SocketClinetMain();

private:
  static SocketClinetMain* instance;
  SocketClinetMain(){}
  int server_port = 000000;
  std::string server_ipv4 = "xxxx";
  std::string client_key = "xxxx";
  int send_sleep = 10;
  int client_socket = 000000;
  std::atomic<bool> exitFlag = false;
  std::thread pollThread;
  void polling_handler();
  void polling();
};
} // namespace SocketClient
