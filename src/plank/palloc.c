#ifndef AMALGAMATION
  #include "palloc.h"
#endif

#ifdef _WIN32
  #include <windows.h>
#else
  #include <fcntl.h>
  #include <signal.h>
  #include <stdio.h>
  #include <string.h>
  #include <sys/mman.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>

static int registered;
static size_t allocations;
static size_t allocationSize;
static size_t totalAllocations;
static size_t totalSize;
static size_t unmanagedDeletions;

struct PoolEntry
{
  void *ptr;
  size_t size;
  char *type;
  struct PoolEntry *next;
};

struct PoolEntry *poolHead;

void pool_cleanup(void)
{
  struct PoolEntry *curr = poolHead;

  printf("[plank]Statistics\n");
  printf("  Allocations: %i\n", (int)allocations);
  printf("  Allocated Size: %i\n", (int)allocationSize);
  printf("  Total Allocations: %i\n", (int)totalAllocations);
  printf("  Total Size: %i\n", (int)totalSize);
  printf("  Unmanaged Deletions: %i\n", (int)unmanagedDeletions);

  while(curr)
  {
    struct PoolEntry *tmp = curr;

    printf("[plank]Leak\n");
    printf("  Type: %s\n", curr->type);
    printf("  Size: %i\n", (int)curr->size);

    curr = curr->next;

    /* free(tmp->ptr); */
    free(tmp->type);
    free(tmp);
  }

  poolHead = NULL;
}

#ifdef PALLOC_ACTIVE
void pfree(void *ptr)
{
  struct PoolEntry *entry = poolHead;
  struct PoolEntry *prev = NULL;

  while(entry)
  {
    if(entry->ptr == ptr)
    {
#ifdef _WIN32
      DWORD OldProtect = 0;
      if(!VirtualProtect(entry->ptr, entry->size, PAGE_NOACCESS, &OldProtect))
      {
        printf("VirtualProtect failed\n");
        abort();
      }
#else
      if(mprotect(entry->ptr, entry->size, PROT_NONE) == -1)
      {
        printf("mprotect failed\n");
        abort();
      }
#endif

      if(!prev)
      {
        poolHead = entry->next;
      }
      else
      {
        prev->next = entry->next;
      }

      allocations -= 1;
      allocationSize -= entry->size;

      free(entry->type);
      free(entry);

      return;
    }

    prev = entry;
    entry = entry->next;
  }

  unmanagedDeletions++;
}
#else
void pfree(void *ptr)
{
  free(ptr);
}
#endif

#ifdef PALLOC_ACTIVE
void *_palloc(size_t size, const char *type)
{
  struct PoolEntry *entry = poolHead;

  if(!registered)
  {
    registered = 1;
    atexit(pool_cleanup);
    printf("[plank]Initialized\n");
  }

  entry = (struct PoolEntry *)calloc(1, sizeof(*entry));

  if(!entry)
  {
    return NULL;
  }

  entry->type = (char *)calloc(strlen(type) + 1, sizeof(char));

  if(!entry->type)
  {
    free(entry);
    return NULL;
  }

#ifdef _WIN32
  entry->ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#else
  entry->ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

  if(!entry->ptr)
  {
    free(entry->type);
    free(entry);
    return NULL;
  }

  entry->size = size;
  strcpy(entry->type, type);

  entry->next = poolHead;
  poolHead = entry;

  allocations += 1;
  allocationSize += size;
  totalAllocations += 1;
  totalSize += size;

  return entry->ptr;
}
#else
void *_palloc(size_t size, const char *type)
{
  void *rtn = NULL;

  rtn = calloc(1, size);

  return rtn;
}
#endif

void *palloc_malloc(size_t size)
{
  return _palloc(size, "native");
}
