#include "upload.hpp"
#include "socketmain.hpp"
#include "config/configuration.hpp"
#include "utils/log.hpp"
#include "utils/err.hpp"
#include "utils/utils.hpp"
#include <cstdlib>
#include <unordered_map>
#include <string>
namespace SocketClient {

Upload *Upload::instance = nullptr;

std::unordered_map<std::string, char> file_mapper;

Upload *Upload::get_instance() {
  if (instance == nullptr) {
    instance = new Upload();
  }
  return instance;
}
void Upload::init() {
  LogInfo("Upload init");
  load_config();
  start_polling();
}

void Upload::load_config() {
  LogInfo("加载上传配置");
  this->local_wait_upload_folder =
      Configuration::get_instance()->getUploadConfig()->upload_local_folder;
  this->upload_config = Configuration::get_instance()->getUploadConfig();
}
void Upload::monitor_upload_fold() {
  LogInfo("上传线程开始polling, 退出标志是%s", b2s(this->exitFlag == true));
  while (!this->exitFlag) {
    while (!this->wait_queue.empty()) {
      WaitUploadFile *t = wait_queue.front();
      bool sucess = _uploadFile(t->local_file, t->remote_file_name);
      if (sucess) {
        wait_queue.pop();
      }
    }
    auto_load_file();
    std::this_thread::sleep_for(std::chrono::seconds(
        this->upload_config->upload_monitor_interval_seconds));
  }
}
void Upload::start_polling() {
  this->pollThread = std::thread(&Upload::monitor_upload_fold, this);
}

int Upload::auto_load_file() {
  std::string directory_path = this->local_wait_upload_folder;
  int fcount = 0;
  if (std::filesystem::exists(directory_path) &&
      std::filesystem::is_directory(directory_path)) {
    for (const auto &entry :
         std::filesystem::directory_iterator(directory_path)) {
      this->uploadFile(entry.path().string(), entry.path().filename().string());
      fcount++;
    }
  } else {
    LogError("%s does not exist or is not a directory.",
             directory_path.c_str());
  }
  LogInfo("%s 文件夹有 %d 个文件被加入到待上传队列中", directory_path.c_str(), fcount);
  if (fcount > upload_config->upload_monitor_upload_max_err_files) {
    LogError("%s 文件夹超过 %d 个文件，达到系统限制， 可能出现错误");
  }

  return fcount;
}
void Upload::join_polling_thead() {
  LogInfo("上传线程是否可以join %b", this->pollThread.joinable());
  if (this->pollThread.joinable()) {
    this->pollThread.join();
  }
}
void Upload::stop_polling() { this->exitFlag = true; };
void Upload::uploadFile(std::string local_file, std::string remote_file_name) {
  if (file_mapper.count(local_file)) return;
  file_mapper[local_file] = 1;
  wait_queue.push(new WaitUploadFile{.local_file = local_file,
                                     .remote_file_name = remote_file_name});
}
bool Upload::_uploadFile(std::string &local_file,
                         std::string &remote_file_name) {

  Qiniu_Io_PutRet putRet;
  Qiniu_Io_PutExtra putExtra;
  Qiniu_Zero(putExtra);
  Qiniu_Client clinet = c->getQiNiuYunClient();
  std::string uptoken = this->upload_config->qi_niu_yun_config->uptoken;
  std::string bucket = this->upload_config->qi_niu_yun_config->bucket;

  Qiniu_Error error =
      Qiniu_Io_PutFile(&clinet, &putRet, uptoken.c_str(),
                       remote_file_name.c_str(), local_file.c_str(), &putExtra);
  if (error.code != 200) {
    LogError("七牛云上传文件出错, 错误信息: %s, 错误码: %d" , error.message,
             error.code);
  } else {
    /*200, 正确返回了, 你可以通过statRet变量查询一些关于这个文件的信息*/
    std::string remote_file_url = formatPrint("%s/%s", bucket.c_str(), putRet.key);
    _upload_file_success(local_file, remote_file_url);
    LogInfo("%s上传成功, 上传到 %s, hash值: %s", local_file.c_str(), remote_file_url.c_str(), putRet.hash);
  }
  return error.code = 200;
}

void Upload::_upload_file_success(std::string &local_file, std::string &remote_file_url) {
  sendDataToServer(local_file, local_file, remote_file_url, "1", "100");
  if (std::remove(local_file.c_str()) != 0) {
    LogError("系统删除文件%s失败", local_file.c_str());
    return;
  }
  LogInfo("系统删除了文件%s", local_file.c_str());
}

} // namespace SocketClient
