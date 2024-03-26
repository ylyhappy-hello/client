#pragma once
#include <atomic>
#include <filesystem>
#include <queue>
#include <string>
#include <thread>

#include <cstdio>
#include <math.h>


#include <qiniu/io.h>
#include <qiniu/multipart_upload.h>

#include "./config/configuration.hpp"
#include "./utils/log.hpp"


namespace SocketClient {
struct WaitUploadFile {
  std::string local_file;
  std::string remote_file_name;
};
class Upload {

public:
  void init();
  void uploadFile(std::string local_file, std::string remote_file_name);
  void start_polling();
  void join_polling_thead();
  void stop_polling();
  void monitor_disk_capacity();// 监控磁盘容量，超过10G，报警
  void send_file_load_message_success_server();
  void send_file_load_message_warning_server();
  static Upload* get_instance();

private:
  SocketClient::Configuration *c = SocketClient::Configuration::get_instance();
  std::queue<WaitUploadFile *> wait_queue;
  static Upload* instance;
  bool _uploadFile(std::string &local_file, std::string &remote_file_name);
  void _upload_file_success(std::string &local_file, std::string &remote_file_url);
  void _upload_file_fail();
  Upload(){};
  void monitor_upload_fold();
  int auto_load_file();
  std::atomic<bool> exitFlag = false;
  std::thread pollThread;
  void load_config();
  UploadConfig* upload_config;
  std::string local_wait_upload_folder;
};
} // namespace SocketClient
