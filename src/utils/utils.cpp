#include "utils.hpp"
#include "lib/httplib.h"
namespace SocketClient {
static httplib::Client cli("localhost", 48080);
httplib::Result sendDataToServer(std::string filename, std::string filepath,
                                 std::string fileremoteurl, std::string type,
                                 std::string size) {

  httplib::Headers h;
  std::string p = " { \"name\": \"" + filename + "\", \"path\": \"" + filepath +
                  "\", \"url\": \"" + fileremoteurl + "\", \"type\": \"" +
                  type + "\", \"size\": " + size + " } ";
  h.emplace("tenant-id", "1");
  h.emplace("content-type", "application/json");

  httplib::Result res =
      cli.Post("/admin-api/motocycleDetect/detect-files/create", h, p, "");
  return res;
}
} // namespace SocketClient
