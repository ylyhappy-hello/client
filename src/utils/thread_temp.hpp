#include <iostream>
#include <thread>
#include <valarray>

namespace SocketClient {
class thread_temp : public std::thread {
public:
  using std::thread::thread;
  template <typename Function, typename... Args>
  explicit thread_temp(Function &&f, Args &&...args)
      : std::thread(std::forward<Function>(f), std::forward<Args>(args)...) {
  }
  virtual void operator()() {}
  void join() {
    if (std::thread::joinable()) {
      std::thread::join();
    }
  }

private:
};
} // namespace SocketClient
