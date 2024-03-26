#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

#include "./toml.hpp"
#include "qiniu/http.h"
#include <qiniu/multipart_upload.h>
#include <toml++/impl/table.hpp>
#include <vector>

namespace SocketClient {
struct SocketServerConfig {
  std::string_view name;
  std::string_view ipv4;
  std::int64_t port;
};
struct SocketClientConfig {
  std::string key;
  int64_t heartbeat_interval;
};
struct QiNiuYunConfig {
  std::string uptoken;
  std::string bucket;
};
struct UploadConfig {
  std::string upload_local_folder;
  std::int64_t upload_monitor_interval_seconds = 30;
  std::int64_t upload_monitor_upload_max_err_files = 100;
  QiNiuYunConfig *qi_niu_yun_config;
};
struct LoggerConfig {
  std::string_view log_filepath;
};
struct DetectConfig {
  std::string model_path{};
  std::vector<int> img_size{};
  std::vector<std::string> classes{};
  std::vector<int> model_shape{};
  float model_confidense_threshold{};
  float model_nms_threshold{};
  bool letter_box_for_square = true;
  std::string detect_img_save_file_path;
};
class Configuration {
private:
  static Configuration *instance;
  Configuration(){};
  Qiniu_Client client;
  toml::table toml_table = Toml::readTomlConfig();
  SocketServerConfig *ssc = nullptr;
  SocketClientConfig *scc = nullptr;
  QiNiuYunConfig *qi_niu_yun_config = nullptr;
  UploadConfig *upload_config = nullptr;
  DetectConfig *detect_config = nullptr;

public:
  static Configuration *get_instance();
  void init();
  Qiniu_Client getQiNiuYunClient();
  SocketServerConfig *getSocketConfig();
  SocketClientConfig *getClientConfig();
  DetectConfig *getDetectConfig();
  UploadConfig *getUploadConfig();
  void QiNiuYunInit();
  ~Configuration();
};
} // namespace SocketClient
