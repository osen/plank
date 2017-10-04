#ifndef AMALGAMATION
  #include "vector.h"
  #include "palloc.h"
#endif

#include <string.h>

void *_VectorNew(size_t size, const char *type)
{
  struct _Vector *rtn = NULL;
  char typeStr[256] = {0};

  strcpy(typeStr, "vector(");
  strcat(typeStr, type);
  strcat(typeStr, ")");
  rtn = (struct _Vector *)_palloc(sizeof(*rtn), typeStr);

  strcpy(typeStr, "vector_header(");
  strcat(typeStr, type);
  strcat(typeStr, ")");
  rtn->vh = (struct _VectorHeader *)_palloc(sizeof(*rtn->vh), typeStr);
  rtn->vh->entrySize = size;

  return rtn;
}

int _VectorOobAssert(void *_vh, size_t idx)
{
  struct _VectorHeader *vh = (struct _VectorHeader *)_vh;

  if(vh->size <= idx)
  {
    printf("Error: Index out of bounds\n");
    abort();
    /* return 1; */
  }

  return 0;
}

void _VectorErase(void *_vh, void *_v, size_t idx)
{
  struct _VectorHeader *vh = (struct _VectorHeader *)_vh;
  struct _Vector *v = (struct _Vector *)_v;
  size_t restSize = (vh->size - (idx + 1)) * vh->entrySize;

  _VectorOobAssert(_vh, idx);

  if(restSize > 0)
  {
    char *element = (char *)v->data + idx * vh->entrySize;
    char *rest = element + vh->entrySize;
    memmove(element, rest, restSize);
  }

  vh->size --;
}

void _VectorResize(void *_vh, void *_v, size_t size)
{
  struct _Vector *v = (struct _Vector *)_v;
  struct _VectorHeader *vh = (struct _VectorHeader *)_vh;

  if(size == 0)
  {
    if(v->data)
    {
      free(v->data);
      v->data = NULL;
      vh->totalSize = 0;
    }
  }
  else if(vh->totalSize < size)
  {
    char *cur = NULL;
    char *last = NULL;

    v->data = realloc(v->data, (size * 2) * vh->entrySize);

    if(!v->data)
    {
      /* TODO: Should cache and revert */
      printf("Error: Failed to reallocate\n");
      abort();
    }

    vh->totalSize = size * 2;
    cur = (char *)v->data;
    cur += vh->size * vh->entrySize;
    last = (char *)v->data;
    last += vh->totalSize * vh->entrySize;

    while(cur != last)
    {
      *cur = 0;
      cur++;
    }
  }

  vh->size = size;
}

size_t _VectorSize(void *_vh)
{
  struct _VectorHeader *vh = (struct _VectorHeader *)_vh;

  return vh->size;
}

void _VectorDelete(void *_vh, void *_v)
{
  struct _Vector *v = (struct _Vector *)_v;
  struct _VectorHeader *vh = (struct _VectorHeader *)_vh;

  if(v->vh != vh)
  {
    printf("Error: Invalid vector\n");
  }

  if(v->data)
  {
    free(v->data);
  }

  pfree(vh);
  pfree(v);
}

