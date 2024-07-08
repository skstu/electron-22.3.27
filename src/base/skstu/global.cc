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

using namespace skstu;

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