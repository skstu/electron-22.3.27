#include "martell.h"
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

namespace sk {

static std::string CurrentTimes();
static long long CurrentTimestampMS();
static std::string CurrentProcessDir();
/* Unused-function
static std::string StringFormat(const char* format, ...);*/

class Logger {
 public:
  Logger(const std::string& logFile);

 private:
  ~Logger();
  void Init();
  void UnInit();

 public:
  void Printf(const char* fmt, ...);
  void Release() const;

 private:
  const std::string log_file_;
  std::atomic_bool shutdown_ = false;
  std::queue<std::string> line_s_;
  std::vector<std::thread> threads_;
  std::shared_ptr<std::mutex> mutex_ = std::make_shared<std::mutex>();
};

static Logger* __gpLogger = nullptr;

namespace chromium {}  // namespace chromium

/*
NativeLibrary LoadNativeLibrary(const FilePath& library_path,
                                NativeLibraryLoadError* error)

// Loads a native library from disk.  Release it with UnloadNativeLibrary when
// you're done.  Returns NULL on failure.
// If |error| is not NULL, it may be filled in on load error.
BASE_EXPORT NativeLibrary LoadNativeLibrary(const FilePath& library_path,
                                            NativeLibraryLoadError* error);
// Unloads a native library.
BASE_EXPORT void UnloadNativeLibrary(NativeLibrary library);

// Gets a function pointer from a native library.
BASE_EXPORT void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                                      const char* name);

*/
namespace ele {

void on_startup(PROCESS_TYPE ptype) {
  std::string current_process_dir = CurrentProcessDir();
  switch (ptype) {
    case PROCESS_TYPE::PROCESS_CHILD: {
      __gpLogger = new Logger(current_process_dir + "/devlog/child_" +
                              std::to_string(CurrentTimestampMS()) + ".log");
    } break;
    case PROCESS_TYPE::PROCESS_MAIN: {
      __gpLogger = new Logger(current_process_dir + "/devlog/main_" +
                              std::to_string(CurrentTimestampMS()) + ".log");

      __gpLogger->Printf("%s", "ele main startup.");
    } break;
    default:
      break;
  }
}
void on_shutdown(PROCESS_TYPE ptype) {
  switch (ptype) {
    case PROCESS_TYPE::PROCESS_CHILD: {
    } break;
    case PROCESS_TYPE::PROCESS_MAIN: {
      __gpLogger->Printf("%s", "ele main shutdown.");
    } break;
    default:
      break;
  }
  if (__gpLogger) {
    __gpLogger->Release();
    __gpLogger = nullptr;
  }
}
void on_ipc_read(const char* src, size_t srcLen, char** dst, size_t* dstLen) {
  int result = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::string msg(src, srcLen);
    write_file_addto("c:\\electorn.log", src, srcLen);

    std::string log("ele ipc read: \n");
    log.append(src, srcLen);
    log.append("\n");
    write_file_addto("c:\\electorn.log", log.data(), log.size());
    result = 1;
  } while (0);
  if (result) {
  }
}

}  // namespace ele

char* malloc_s(size_t len) {
  char* result = NULL;
  result = (char*)malloc(len);
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
int get_current_process_path(char** path, size_t* pathLen) {
  int result = SK_NO;
  *path = nullptr;
  *pathLen = 0;
  do {
    base::FilePath executableDir;
    base::PathService::Get(base::DIR_EXE, &executableDir);
    std::string executableDirA = executableDir.MaybeAsASCII();
    *path = malloc_s(executableDirA.size());
    if (!*path)
      break;
    memcpy(*path, executableDirA.data(), executableDirA.size());
    *pathLen = executableDirA.size();
    result = SK_OK;
  } while (0);
  return result;
}
int read_file(const char* file_path, char** dst, size_t* dstLen) {
  int result = SK_NO;
  *dst = NULL;
  *dstLen = 0;
  do {
    if (!file_path)
      break;
    std::fstream of(file_path, static_cast<std::ios_base::openmode>(
                                   std::ios::in | std::ios::binary));
    if (!of.is_open())
      break;
    of.seekg(0, of.end);
    *dstLen = static_cast<size_t>(of.tellg());
    if (*dstLen <= 0)
      break;
    *dst = malloc_s(*dstLen);
    of.seekg(0, of.beg);
    of.read(*dst, static_cast<std::streamsize>(*dstLen));
    of.close();
    result = SK_OK;
  } while (0);
  return result;
}
int write_file(const char* file_path, const char* src, size_t srcLen) {
  int result = SK_NO;
  do {
    if (!file_path || !src || srcLen <= 0)
      break;
    std::fstream of(file_path, static_cast<std::ios_base::openmode>(
                                   static_cast<int>(std::ios::binary) |
                                   static_cast<int>(std::ios::out) |
                                   static_cast<int>(std::ios::trunc)));
    if (!of.is_open())
      break;
    of << std::string(src, srcLen);
    of.close();
    result = SK_OK;
  } while (0);
  return result;
}
int write_file_addto(const char* file_path, const char* src, size_t srcLen) {
  int result = SK_NO;
  do {
    if (!file_path || !src || srcLen <= 0)
      break;
    std::ofstream of(file_path,
                     std::ios::binary | std::ios::out | std::ios::app);
    if (!of.is_open())
      break;
    of << std::string(src, srcLen);
    of.close();
    result = SK_OK;
  } while (0);
  return result;
}
int wide_to_ascii(const wchar_t* src,
                  size_t srcLen,
                  char** dst,
                  size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::u16string u16str;
    if (!base::WideToUTF16(src, srcLen, &u16str))
      break;
    std::string str = base::UTF16ToASCII(base::StringPiece16(u16str));
    if (str.empty())
      break;
    *dstLen = str.size();
    *dst = malloc_s(*dstLen);
    memcpy(*dst, str.data(), *dstLen);
    result = SK_OK;
  } while (0);
  return result;
}
int ascii_to_wide(const char* src,
                  size_t srcLen,
                  wchar_t** dst,
                  size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::string str(src, srcLen);
    std::u16string u16str = base::ASCIIToUTF16(base::StringPiece(str));
    std::wstring wstr = base::UTF16ToWide(base::StringPiece16(u16str));
    if (wstr.empty())
      break;
    *dstLen = wstr.size();
    *dst = (wchar_t*)malloc_s((*dstLen) * sizeof(char));
    memcpy(*dst, (char*)wstr.data(), (*dstLen) * sizeof(char));
    result = SK_OK;
  } while (0);
  return result;
}
int utf8_to_wide(const char* src,
                 size_t srcLen,
                 wchar_t** dst,
                 size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::string u8str(src, srcLen);
    std::wstring wstr = base::UTF8ToWide(base::StringPiece(u8str));
    if (wstr.empty())
      break;
    *dstLen = wstr.size();
    *dst = (wchar_t*)malloc_s((*dstLen) * sizeof(char));
    memcpy(*dst, (char*)wstr.data(), (*dstLen) * sizeof(char));
    result = SK_OK;
  } while (0);
  return result;
}
int wide_to_utf8(const wchar_t* src,
                 size_t srcLen,
                 char** dst,
                 size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::wstring wstr(src, srcLen);
    std::string str = base::WideToUTF8(base::WStringPiece(wstr));
    if (str.empty())
      break;
    *dstLen = str.size();
    *dst = malloc_s(*dstLen);
    memcpy(*dst, str.data(), *dstLen);
    result = SK_OK;
  } while (0);
  return result;
}
int utf8_to_ascii(const char* src, size_t srcLen, char** dst, size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::string u8str(src, srcLen);
    std::u16string u16str = base::UTF8ToUTF16(base::StringPiece(u8str));
    if (u16str.empty())
      break;
    std::string str = base::UTF16ToASCII(base::StringPiece16(u16str));
    if (str.empty())
      break;
    *dstLen = str.size();
    *dst = malloc_s(*dstLen);
    memcpy(*dst, str.data(), *dstLen);
    result = SK_OK;
  } while (0);
  return result;
}
int ascii_to_utf8(const char* src, size_t srcLen, char** dst, size_t* dstLen) {
  int result = SK_NO;
  *dst = nullptr;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    std::string str(src, srcLen);
    std::u16string u16str = base::ASCIIToUTF16(base::StringPiece(str));
    if (u16str.empty())
      break;
    std::string u8str = base::UTF16ToUTF8(base::StringPiece16(u16str));
    if (u8str.empty())
      break;
    *dstLen = u8str.size();
    *dst = malloc_s(*dstLen);
    memcpy(*dst, u8str.data(), *dstLen);
    result = SK_OK;
  } while (0);
  return result;
}

int base64_encode(const char* src, size_t srcLen, char** dst, size_t* dstLen) {
  int result = SK_NO;
  *dst = NULL;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    *dstLen = modp_b64_encode_len(srcLen);
    *dst = malloc_s(*dstLen);
    if (!*dst)
      break;
    *dstLen = modp_b64_encode(*dst, src, srcLen);
    if (*dstLen <= 0)
      break;
    result = SK_OK;
  } while (0);
  if (result != SK_OK) {
    *dstLen = 0;
  }
  return result;
}
int base64_decode(const char* src, size_t srcLen, char** dst, size_t* dstLen) {
  int result = SK_NO;
  *dst = NULL;
  *dstLen = 0;
  do {
    if (!src || srcLen <= 0)
      break;
    *dst = malloc_s(modp_b64_decode_len(srcLen));
    if (!*dst)
      break;
    *dstLen = modp_b64_decode(*dst, src, srcLen);
    if (*dstLen == MODP_B64_ERROR)
      break;
    result = SK_OK;
  } while (0);
  if (result != SK_OK) {
    *dstLen = 0;
  }
  return result;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Logger::Logger(const std::string& logFile) : log_file_(logFile) {
  Init();
}
Logger::~Logger() {
  UnInit();
}
void Logger::Init() {
  threads_.emplace_back([this]() {
    do {
      if (!line_s_.empty()) {
        std::ofstream of(log_file_,
                         std::ios::binary | std::ios::out | std::ios::app);
        if (of.is_open()) {
          do {
            if (line_s_.empty())
              break;
            std::string line = line_s_.front();
            std::string final_line = CurrentTimes();
            final_line.append(" ").append(line).append("\r\n");
            of << final_line;
            line_s_.pop();
          } while (1);
          of.close();
        }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (shutdown_.load())
        break;
    } while (1);
  });
}
void Logger::UnInit() {
  shutdown_.store(true);
  for (auto& t : threads_)
    t.join();
  threads_.clear();
}
void Logger::Printf(const char* fmt, ...) {
  std::lock_guard<std::mutex> lock{*mutex_};
  if (!fmt)
    return;
  va_list ap;
  va_start(ap, fmt);
  std::string log_data;
  base::StringAppendV(&log_data, fmt, ap);
  va_end(ap);
  if (log_data.empty())
    return;
  line_s_.push(log_data);
}
void Logger::Release() const {
  delete this;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CurrentTimes() {
  std::string result;
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t tt = std::chrono::system_clock::to_time_t(now);
  tm* timeInfo = std::localtime(&tt);
  std::stringstream ss;
  ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now - std::chrono::system_clock::from_time_t(tt));
  ss << "." << std::setfill('0') << std::setw(3) << ms.count();
  result = ss.str();
  return result;
}
std::string CurrentProcessDir() {
  base::FilePath executableDir;
  base::PathService::Get(base::DIR_EXE, &executableDir);
  return executableDir.MaybeAsASCII();
}
long long CurrentTimestampMS() {
  std::chrono::milliseconds timestamp =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return timestamp.count();
}
/* Unused-function
std::string StringFormat(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string result;
  base::StringAppendV(&result, format, ap);
  va_end(ap);
  return result;
}*/
}  // namespace sk