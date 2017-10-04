#ifndef PALLOC_H
#define PALLOC_H

#include <stdlib.h>

#define PALLOC_ACTIVE

void pfree(void *ptr);
void *_palloc(size_t size, const char *type);

#define palloc(T) \
  (T*)_palloc(sizeof(T), #T)

#endif
