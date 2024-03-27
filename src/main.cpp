#include "./lib/httplib.h"
#include "./socketmain.hpp"
#include "./upload.hpp"
//#include "detect/detect.hpp"
#include "utils/dateformat.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"
#include <iostream>
//#include <opencv2/core/mat.hpp>

void testSocket() {
  SocketClient::SocketClinetMain *socketmain =
      SocketClient::SocketClinetMain::get_instance();
  socketmain->init();
}

void testUpload() {
  SocketClient::Configuration *c = SocketClient::Configuration::get_instance();
  c->init();
  SocketClient::Upload *upload = SocketClient::Upload::get_instance();
  upload->init();
  upload->join_polling_thead();
  LogError("程序结束了");
}

// void testDetect() {
//   SocketClient::Detect *detect = SocketClient::Detect::get_instanse();
//   detect->init();
//   cv::Mat frame;
//   LogInfo(SocketClient::timeFormatStrAddTimestamp());
//   detect->start();
//   LogInfo("Detect pass ok");
// }
//
//
void testConfig() {
  SocketClient::Configuration *c = SocketClient::Configuration::get_instance();
  c->init();
  LogInfo("%d", c->getDetectConfig()->img_size[0]);
  LogInfo("%s", c->getDetectConfig()->classes[0].c_str());
}

int main(int argc, char *argv[]) {
  testUpload();

  return 0;
}
