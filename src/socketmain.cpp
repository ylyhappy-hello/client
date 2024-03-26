#include "socketmain.hpp"
#include "utils/log.hpp"
namespace SocketClient {
SocketClinetMain *SocketClinetMain::instance = nullptr;
void SocketClinetMain::init() {
  Configuration *cg = SocketClient::Configuration::get_instance();
  this->server_port = cg->getSocketConfig()->port;
  this->server_ipv4 = cg->getSocketConfig()->ipv4;
  this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
  this->client_key = cg->getClientConfig()->key;
  this->send_sleep = cg->getClientConfig()->heartbeat_interval;
  auto s_addr = inet_addr(server_ipv4.c_str());
  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(server_port),
      .sin_addr = {.s_addr = s_addr},
  };
  SocketClient::errif(this->client_socket == -1, "Error creating socket");
  SocketClient::errif(connect(this->client_socket,
                              (struct sockaddr *)&serv_addr,
                              sizeof(serv_addr)) != 0,
                      "Error creating connect");
  LogInfo("连接到服务器%s 成功端口: %d", this->server_ipv4.c_str(),
          this->server_port);
  start_polling();
}

SocketClinetMain *SocketClinetMain::get_instance() {
  if (instance == nullptr) {
    instance = new SocketClinetMain();
  }
  return instance;
}

void SocketClinetMain::polling() {
  LogInfo("Socket线程启动, 退出标志是%s", b2s(this->exitFlag == true));
  while (!this->exitFlag) {
    polling_handler();
    std::this_thread::sleep_for(std::chrono::seconds(this->send_sleep));
  }
}
void SocketClinetMain::send_msg(std::string action, std::string msg) {

  const std::string final_msg =
      this->client_key + "," + action + "," + msg + "\n";
  send(client_socket, final_msg.c_str(), strlen(final_msg.c_str()), 0);
  LogInfo("向%s发送了%s", server_ipv4.c_str(), final_msg.c_str());
}

void SocketClinetMain::send_post(std::string payload) {
  std::string request_string = "POST ";
  request_string += "https:/loa";
  request_string += " HTTP/1.1\r\n"; // or HTTP/1.0
  request_string += ("Host: " "\r\n");
  request_string += "Accept: */*\r\n";
  request_string += "Content-Type: application/x-www-form-urlencoded\r\n";
  request_string += ("Content-Length: " + std::to_string(payload.size()) + "\r\n");
  request_string += "Connection: close\r\n"; // omit this if using HTTP/1.0
  request_string += "\r\n";
  send(client_socket, request_string.c_str(), strlen(request_string.c_str()), 0);
}

/*
 * 1. 向服务器发送alive消息
 * 2. 向服务器发送uplaodedFile消息
 * */
void SocketClinetMain::polling_handler() {
  const std::string alive_msg = this->client_key + ",alive, \n";
  send(client_socket, alive_msg.c_str(), strlen(alive_msg.c_str()), 0);
  LogInfo("向%s发送了%s", server_ipv4.c_str(), "alive");
}

void SocketClinetMain::start_polling() {
  this->pollThread = std::thread(&SocketClinetMain::polling, this);
}
void SocketClinetMain::stop_polling() { this->exitFlag = true; }

void SocketClinetMain::join_polling_thead() {
  if (this->pollThread.joinable()) {
    this->pollThread.join();
  }
}

SocketClinetMain::~SocketClinetMain() { close(this->client_socket); }

} // namespace SocketClient
