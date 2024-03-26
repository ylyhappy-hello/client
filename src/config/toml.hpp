#pragma once
// Declare anything that needs to be shared between b.cpp and d.cpp

#include "../utils/log.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <toml++/impl/forward_declarations.hpp>
#include <toml++/impl/node.hpp>
#include <toml++/impl/node_view.hpp>
#include <toml++/toml.hpp>

namespace SocketClient {
const std::string toml_path = "config/conf.toml";

class Toml {
private:
  Toml();

public:
  static toml::table readTomlConfig();

  template <typename T>
  static std::vector<T>
  convertArraysNodeNumber(toml::node_view<toml::node> node_view) {
    std::vector<T> res;
    if (toml::array *arr = node_view.as_array()) {
      arr->for_each([&res](auto &&el) {
        if constexpr (toml::is_number<decltype(el)>)
          res.push_back(el.get());
      });
    }
    return res;
  };

  template <typename T>
  static std::vector<T>
  convertArraysNodeString(toml::node_view<toml::node> node_view) {
    std::vector<T> res;
    if (toml::array *arr = node_view.as_array()) {
      arr->for_each([&res](auto &&el) {
        if constexpr (toml::is_string<decltype(el)>)
          res.push_back(el.get());
      });
    }
    return res;
  };
};
} // namespace SocketClient
