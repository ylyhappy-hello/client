#pragma once
#define DEBUG
#ifdef DEBUG
#define LogInfo                                                                \
  SocketClient::debugPrint(__FILE_NAME__, __LINE__);                           \
  SocketClient::info
#define LogWarn                                                                \
  SocketClient::debugPrint(__FILE_NAME__, __LINE__);                           \
  SocketClient::warn
#define LogError                                                               \
  SocketClient::debugPrint(__FILE_NAME__, __LINE__);                           \
  SocketClient::error
#endif // DEBUG
#ifndef DEBUG
#define LogInfo SocketClient::info
#define LogWarn SocketClient::warn
#define LogError SocketClient::error
#endif // !DEBUG
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace SocketClient {

class StreamBufferSupervisor : public std::streambuf {
public:
  StreamBufferSupervisor() {
    _outFileStream_.open("log", std::ios::out);
    _streamBufferPtr_ = _outputStream_->rdbuf(); // back up cout's streambuf
    _outputStream_->flush();
    setp(nullptr, nullptr);
    //      _streamBufferSupervisorPtr_->setStreamBuffer(cbuf);
    _outputStream_->rdbuf(this); // reassign your streambuf to cout
  }
  ~StreamBufferSupervisor() override {
    if (_outFileStream_.is_open()) {
      _outFileStream_.close();
    }
    _outputStream_->rdbuf(_streamBufferPtr_);
  }
  template <typename T> StreamBufferSupervisor &operator<<(const T &something) {
    (*_outputStream_) << something;
    if (_outFileStream_.is_open())
      _outFileStream_ << something;
    return *this;
  }
  StreamBufferSupervisor &operator<<(std::ostream &(*f)(std::ostream &)) {
    (*_outputStream_) << f;
    if (_outFileStream_.is_open())
      _outFileStream_ << f;
    return *this;
  }
  int_type overflow(int_type c) override {
    if (_streamBufferPtr_ != nullptr)
      _streamBufferPtr_->sputc(char(c));
    return c;
  }

private:
  std::ostream *_outputStream_ = &std::cout;
  std::streambuf *_streamBufferPtr_{nullptr};
  std::ofstream _outFileStream_;
};

static bool const enableColor = false;
static bool const logFilePath = "";
static StreamBufferSupervisor *const __StreamBufPtr__ =
    new StreamBufferSupervisor();
enum class Color {
  RESET = 0,
  BG_RED,
  BG_GREEN,
  BG_YELLOW,
  BG_BLUE,
  BG_MAGENTA,
  BG_GREY
};
template <typename... Args>
static std::string formatPrint(const std::string strToFormat_ = "",
                               const Args &...args) {
  size_t size = snprintf(nullptr, 0, strToFormat_.c_str(), args...) + 1;
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, strToFormat_.c_str(), args...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

static std::string getColorEscapeCode(SocketClient::Color color_) {
  if (color_ == SocketClient::Color::RESET)
    return {"\x1b[0m"};
  if (color_ == SocketClient::Color::BG_RED)
    return {"\x1b[41m"};
  if (color_ == SocketClient::Color::BG_GREEN)
    return {"\x1b[42m"};
  if (color_ == SocketClient::Color::BG_YELLOW)
    return {"\x1b[43m"};
  if (color_ == SocketClient::Color::BG_BLUE)
    return {"\x1b[44m"};
  if (color_ == SocketClient::Color::BG_MAGENTA)
    return {"\x1b[45m"};
  if (color_ == SocketClient::Color::BG_GREY)
    return {"\x1b[46m"};
  return {};
}

static std::string setColorStr(std::string &s, SocketClient::Color color_) {
  if (!enableColor) return {s};
  return getColorEscapeCode(color_) + s +
         getColorEscapeCode(SocketClient::Color::RESET);
}

static void debugPrint(const char *s, int t) {
  std::string _s = formatPrint("%s %d\t", s, t);
  *__StreamBufPtr__ << setColorStr(_s, SocketClient::Color::BG_BLUE);
}
static std::string getCurrentTimeStr() {
  std::time_t now = std::time(0);
  char timeStr[128];
  std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
                std::localtime(&now));
  return timeStr;
}

template <typename... Args>
static void info(const std::string strToFormat_ = "", const Args... args) {
  std::string prefix = "[INFO] " + getCurrentTimeStr();
  *__StreamBufPtr__ << setColorStr(prefix, SocketClient::Color::BG_GREEN) + " ";
  *__StreamBufPtr__ << formatPrint(strToFormat_,
                                   std::forward<const Args>(args)...)
                    << std::endl; // We don't want the '\0' inside
}

template <typename... Args>
static void error(const std::string strToFormat_ = "", const Args... args) {
  std::string prefix = "[EROR] " + getCurrentTimeStr();
  *__StreamBufPtr__ << setColorStr(prefix, SocketClient::Color::BG_RED) + " ";
  *__StreamBufPtr__ << formatPrint(strToFormat_,
                                   std::forward<const Args>(args)...)
                    << std::endl; // We don't want the '\0' inside
}

template <typename... Args>
static void warn(const std::string strToFormat_ = "", const Args... args) {
  std::string prefix = "[WARN] " + getCurrentTimeStr();
  *__StreamBufPtr__ << setColorStr(prefix, SocketClient::Color::BG_YELLOW) +
                           " ";
  *__StreamBufPtr__ << formatPrint(strToFormat_,
                                   std::forward<const Args>(args)...)
                    << std::endl; // We don't want the '\0' inside
}

}; // namespace SocketClient
