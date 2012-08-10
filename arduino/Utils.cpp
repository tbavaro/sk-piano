#include "Utils.h"

#include <stdlib.h>

uint32_t Utils::availableMemory() {
  uint32_t size = 8192;
  void* buf;
  while ((buf = malloc(--size)) == NULL);
  free(buf);
  return size;
}

