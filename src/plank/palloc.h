#ifndef PALLOC_H
#define PALLOC_H

#include <stdlib.h>

#define PALLOC_ACTIVE

void pfree(void *ptr);
void *_palloc(size_t size, const char *type);

void *palloc_malloc(size_t size);

#define palloc(T) \
  (T*)_palloc(sizeof(T), #T)

#endif
