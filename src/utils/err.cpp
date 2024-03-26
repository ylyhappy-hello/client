#include "err.hpp"
#include "log.hpp"
namespace SocketClient {
void errif(bool condition, const char *errmsg) {
  if (condition) {
    LogError(errmsg);
    std::exit(EXIT_FAILURE);
  }
}
// bool to str
const char* b2s(bool b) {
  return b ? "true" : "false";
}
} // namespace SocketClient
