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

#include <new>

#include <stdlib.h>
#include <stdio.h>

#define PLANK_ENABLE

size_t allocations;
size_t allocatedSize;
size_t totalAllocations;
size_t totalSize;
size_t unmanagedDeletions;
bool firstAlloc;

struct plankentry
{
  void *ptr;
  size_t size;
  plankentry* next;
};

plankentry* entries;

void plankstats()
{
  printf("Plank Memory Checker - Statistics\n");
  printf("  Allocations: %i\n", (int)allocations);
  printf("  Allocated Size: %i\n", (int)allocatedSize);
  printf("  Total Allocations: %i\n", (int)totalAllocations);
  printf("  Total Size: %i\n", (int)totalSize);
  printf("  Unmanaged Deletions: %i\n", (int)unmanagedDeletions);
}

void* do_new(size_t size)
{
  void *rtn = NULL;

  if(!firstAlloc)
  {
    firstAlloc = true;
    atexit(plankstats);

    printf("Plank Memory Checker - Initialized\n");
  }

  if(!size) size = 1;

#ifdef _WIN32
  rtn = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#else
  rtn = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

  if(!rtn)
  {
    return NULL;
  }

  plankentry* newEntry = (plankentry*)malloc(sizeof(*newEntry));
  newEntry->ptr = rtn;
  newEntry->size = size;
  newEntry->next = entries;
  entries = newEntry;

  allocations++;
  totalAllocations++;
  allocatedSize += size;
  totalSize += size;

  return rtn;
}

void do_delete(void* ptr)
{
  plankentry* prev = NULL;
  plankentry* curr = entries;
  size_t size = 0;
  bool found = false;

  while(curr)
  {
    if(curr->ptr == ptr)
    {
      found = true;
      size = curr->size;

      if(prev)
      {
        prev->next = curr->next;
      }
      else
      {
        entries = curr->next;
      }

      free(curr);
      break;
    }

    prev = curr;
    curr = curr->next;
  }

  if(!found)
  {
    unmanagedDeletions++;
    return;
  }

  allocations--;
  allocatedSize -= size;

#ifdef _WIN32
  DWORD OldProtect = 0;
  if(!VirtualProtect(ptr, size, PAGE_NOACCESS, &OldProtect))
  {
    printf("VirtualProtect failed\n");
    abort();
  }
#else
  if(mprotect(ptr, size, PROT_NONE) == -1)
  {
    printf("mprotect failed\n");
    abort();
  }
#endif
}

#ifdef PLANK_ENABLE
void *operator new[](size_t s) throw(std::bad_alloc)
{
  void *rtn = do_new(s);

  if(!rtn) throw std::bad_alloc();

  return rtn;
}

void operator delete[](void *p) throw()
{
  do_delete(p);
}

void *operator new(size_t s) throw(std::bad_alloc)
{
  void *rtn = do_new(s);

  if(!rtn) throw std::bad_alloc();

  return rtn;
}

void operator delete(void *p) throw()
{
  do_delete(p);
}
#endif
