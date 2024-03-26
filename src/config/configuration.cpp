#include "configuration.hpp"
#include <cstdint>

namespace SocketClient {
Configuration *Configuration::instance = nullptr;

Configuration *Configuration::get_instance() {
  if (instance == nullptr) {
    instance = new Configuration();
  }
  return instance;
}

void Configuration::init() {
  QiNiuYunInit();
  getSocketConfig();
  getClientConfig();
  getUploadConfig();
};
SocketServerConfig *Configuration::getSocketConfig() {
  if (this->ssc == nullptr) {
    this->ssc = new SocketServerConfig{
        .ipv4 = *toml_table["server"]["ipv4"].value<std::string_view>(),
        .port = *toml_table["server"]["port"].value<std::int64_t>(),
    };
  }
  return this->ssc;
}
SocketClientConfig *Configuration::getClientConfig() {
  if (this->scc == nullptr) {
    this->scc = new SocketClientConfig{
        .key = *toml_table["client"]["key"].value<std::string>(),
        .heartbeat_interval =
            *toml_table["client"]["hearbeat_packet_sending_interval"]
                 .value<int64_t>(),
    };
  }
  return this->scc;
}
Qiniu_Client Configuration::getQiNiuYunClient() {
  return std::ref(this->client);
}
void Configuration::QiNiuYunInit() {
  Qiniu_Global_Init(-1);
  Qiniu_Client_InitNoAuth(&client, 1024);
  Qiniu_Client_EnableAutoQuery(&client, Qiniu_True);
}
UploadConfig *Configuration::getUploadConfig() {
  if (this->qi_niu_yun_config == nullptr) {
    this->qi_niu_yun_config = new QiNiuYunConfig{
        .uptoken =
            *toml_table["upload"]["qiniu"]["uptoken"].value<std::string>(),
        .bucket = *toml_table["upload"]["qiniu"]["bucket"].value<std::string>(),
    };
  }
  if (this->upload_config == nullptr) {
    this->upload_config = new UploadConfig{
        .upload_local_folder =
            *toml_table["upload"]["upload_local_folder"].value<std::string>(),
        .upload_monitor_interval_seconds =
            *toml_table["upload"]["upload_monitor_interval_seconds"]
                 .value<std::int64_t>(),
        .upload_monitor_upload_max_err_files =
            *toml_table["upload"]["upload_monitor_upload_max_err_files"]
                 .value<std::int64_t>(),
        .qi_niu_yun_config = this->qi_niu_yun_config,
    };
  }
  return this->upload_config;
}
DetectConfig *Configuration::getDetectConfig() {
  if (this->detect_config == nullptr) {
    this->detect_config = new DetectConfig{
        .model_path = *toml_table["detect"]["model_path"].value<std::string>(),
        .img_size = Toml::convertArraysNodeNumber<int>(
            toml_table["detect"]["img_size"]),
        .classes = Toml::convertArraysNodeString<std::string>(
            toml_table["detect"]["classes"]),
        .model_shape = Toml::convertArraysNodeNumber<int>(
            toml_table["detect"]["model_shape"]),
        .model_confidense_threshold =
            *toml_table["detect"]["model_confidense_threshold"].value<float>(),
        .model_nms_threshold =
            *toml_table["detect"]["model_nms_threshold"].value<float>(),
        .letter_box_for_square =
            *toml_table["detect"]["letter_box_for_square"].value<bool>(),
        .detect_img_save_file_path =
            *toml_table["detect"]["detect_img_save_path"].value<std::string>()};
  }
  return this->detect_config;
}

Configuration::~Configuration() {
  Qiniu_Client_Cleanup(&client); /* 每个HTTP客户端使用完后释放 */
  Qiniu_Servend_Cleanup();
}
} // namespace SocketClient
