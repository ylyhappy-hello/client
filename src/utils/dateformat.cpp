#include "dateformat.hpp"
namespace SocketClient {
std::string timeFormatStr() {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
  auto str = oss.str();
  return str;
}

std::string timeFormatStrAddTimestamp() {
  auto t = std::time(nullptr);
  return timeFormatStr() + '_' + std::to_string(t);
}
} // namespace SocketClient
