#include "toml.hpp"
#include <toml++/impl/array.hpp>
#include <toml++/impl/node.hpp>
#include <toml++/impl/node_view.hpp>

#include "../utils/log.hpp"

namespace SocketClient {
toml::table Toml::readTomlConfig() {
  LogInfo("Toml 配置文件位置是 %s ", toml_path.c_str());
  return toml::parse_file(toml_path); };

} // namespace SocketClient
