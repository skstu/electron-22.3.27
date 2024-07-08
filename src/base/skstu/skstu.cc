#include "skstu.h"
#include "base/base64.h"
#include "base/base_export.h"
#include "base/command_line.h"
#include "base/containers/span.h"
#include "base/files/file_enumerator.h"
#include "base/files/file_util.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/process/process.h"
#include "base/strings/string_piece.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "third_party/modp_b64/modp_b64.h"
#include "third_party/rapidjson/include/rapidjson/document.h"
#include "third_party/rapidjson/include/rapidjson/rapidjson.h"
#include "third_party/rapidjson/include/rapidjson/reader.h"
#include "third_party/rapidjson/include/rapidjson/stream.h"
#include "third_party/rapidjson/include/rapidjson/stringbuffer.h"
#include "third_party/rapidjson/include/rapidjson/writer.h"

#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

namespace skstu {
char* malloc_s(size_t len) {
  char* result = NULL;
  result = (char*)malloc(len);
  memset(result, 0x00, len);
  return result;
}
void free_s(void** p) {
  do {
    if (!(*p))
      break;
    free(*p);
    *p = NULL;
  } while (0);
}
//!@ C:\Developer\electron-dev\src\content\app\content_main.cc -- line(346)
void on_electron_startup(ELECTRON_PROCESS_TYPE) {}
//!@ C:\Developer\electron-dev\src\content\app\content_main.cc -- line(364)
void on_electron_shutdown(ELECTRON_PROCESS_TYPE) {}
//!@ C:\Developer\electron-dev\src\electron\shell\browser\api\electron_api_web_contents.cc -- line(1913)
void on_electron_ipc_message(const char* channel,
                             size_t channel_len,
                             const char* msg,
                             size_t msg_len,
                             char** reply,
                             size_t* reply_len) {}
}  // namespace skstu