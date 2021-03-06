// RUN: %clangxx_msan -m64 -O0 %s -o %t && not %t >%t.out 2>&1
// RUN: FileCheck %s < %t.out
// RUN: %clangxx_msan -m64 -O3 %s -o %t && not %t >%t.out 2>&1
// RUN: FileCheck %s < %t.out

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>

volatile int z;

int main(void) {
  struct addrinfo *ai;
  struct addrinfo hint;
  int res = getaddrinfo("localhost", NULL, NULL, &ai);
  if (ai) z = 1; // OK
  res = getaddrinfo("localhost", NULL, &hint, &ai);
  // CHECK: UMR in __interceptor_getaddrinfo at offset 0 inside
  // CHECK: WARNING: MemorySanitizer: use-of-uninitialized-value
  // CHECK: #0 {{.*}} in main {{.*}}getaddrinfo-positive.cc:[[@LINE-3]]
  return 0;
}
