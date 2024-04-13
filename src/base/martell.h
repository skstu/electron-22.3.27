#ifndef BASE_MARTELL_H_
#define BASE_MARTELL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "base/base_export.h"

namespace sk {

enum SKRES {
  SK_OK = 0,
  SK_NO = 1,
};

namespace ele {
enum PROCESS_TYPE {
  PROCESS_MAIN = 0,
  PROCESS_CHILD = 1,
};
BASE_EXPORT void on_startup(PROCESS_TYPE);
BASE_EXPORT void on_shutdown(PROCESS_TYPE);
BASE_EXPORT void on_ipc_read(const char*, size_t, char**, size_t*);

}  // namespace ele

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
}  // namespace sk

#endif  // BASE_MARTELL_H_