#ifndef PALLOC_VECTOR_H
#define PALLOC_VECTOR_H

#include <stdlib.h>
#include <stdio.h>

struct _VectorHeader
{
  size_t size;
  size_t entrySize;
  size_t totalSize;
};

struct _Vector
{
  struct _VectorHeader *vh;
  void *data;
};

#define vector(T) \
  T*

void *_VectorNew(size_t size, const char *type);
int _VectorOobAssert(void *_vh, size_t idx);
void _VectorErase(void *_vh, void *_v, size_t idx);
void _VectorResize(void *_vh, void *_v, size_t size);
size_t _VectorSize(void *_vh);
void _VectorDelete(void *_vh, void *_v);

#define vector_new(T) \
  (vector(T) *)_VectorNew(sizeof(T), #T)

#define vector_delete(V) \
  _VectorDelete(V[0], V)

#define vector_size(V) \
  _VectorSize(V[0])

#define vector_resize(V, S) \
  _VectorResize(V[0], V, S)

#define vector_set(V, I, D) \
  do { \
    if(vector_size(V) <= I) { \
      printf("Error: Index out of bounds\n"); \
    } \
    V[1][I] = D; \
  } while(0)

#define vector_at(V, I) \
  (_VectorOobAssert(V[0], I) ? V[1][I] : V[1][I])

/*
#define vector_raw(V) \
  V[1]
*/

#define vector_data(V) \
  V[1]

#define vector_push_back(V, D) \
  do { \
    vector_resize(V, vector_size(V) + 1); \
    vector_set(V, vector_size(V) - 1, D); \
  } while(0)

#define vector_clear(V) \
  vector_resize(V, 0)

#define vector_erase(V, I) \
  _VectorErase(V[0], V, I)

#endif
