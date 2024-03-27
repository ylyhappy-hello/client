#include "../lib/httplib.h"
#include <string>
namespace SocketClient {
// filename , filepath, fileremoteurl, type ,size 
httplib::Result sendDataToServer(std::string filename, std::string filepath,
                      std::string fileremoteurl, std::string type,
                      std::string size);
}
// namespace SocketClient
