#ifndef __SKSTU_H_20240708_915__
#define __SKSTU_H_20240708_915__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "base/base_export.h"

namespace skstu {
enum SKRES {
  SK_OK = 0,
  SK_NO = 1,
};
enum ELECTRON_PROCESS_TYPE {
  ELECTRON_PROCESS_MAIN = 0,
  ELECTRON_PROCESS_CHILD = 1,
};
BASE_EXPORT char* malloc_s(size_t);
BASE_EXPORT void free_s(void**);
BASE_EXPORT int read_file(const char*, char**, size_t*);
BASE_EXPORT int write_file(const char*, const char*, size_t);
BASE_EXPORT int write_file_addto(const char*, const char*, size_t);
BASE_EXPORT int get_current_process_path(char**, size_t*);
BASE_EXPORT int wide_to_ascii(const wchar_t*, size_t, char**, size_t*);
BASE_EXPORT int ascii_to_wide(const char*, size_t, wchar_t**, size_t*);
BASE_EXPORT int utf8_to_wide(const char*, size_t, wchar_t**, size_t*);
BASE_EXPORT int wide_to_utf8(const wchar_t*, size_t, char**, size_t*);
BASE_EXPORT int utf8_to_ascii(const char*, size_t, char**, size_t*);
BASE_EXPORT int ascii_to_utf8(const char*, size_t, char**, size_t*);
BASE_EXPORT int base64_encode(const char*, size_t, char**, size_t*);
BASE_EXPORT int base64_decode(const char*, size_t, char**, size_t*);

BASE_EXPORT void on_electron_startup(ELECTRON_PROCESS_TYPE);
BASE_EXPORT void on_electron_shutdown(ELECTRON_PROCESS_TYPE);
BASE_EXPORT void on_electron_ipc_message(const char* channel,
                                         size_t channel_len,
                                         const char* msg,
                                         size_t msg_len,
                                         char** reply,
                                         size_t* reply_len);

}  // namespace skstu

#endif  //__SKSTU_H_20240708_915__