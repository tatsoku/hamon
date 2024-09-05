#ifdef _WIN32

#include <stdio.h>
#include <windows.h>

#include <hamon_error.h>

void win_perror(const char *msg) {
  LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                0, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, 0);

  printf("%s: %s\n", msg, lpMsgBuf);
  LocalFree(lpMsgBuf);
}
#endif
