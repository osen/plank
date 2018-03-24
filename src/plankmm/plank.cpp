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
//#define PLANK_MAX_ZOMBIES_SIZE 64 * 1024 * 1024
//#define PLANK_MAX_ZOMBIES 64 * 1024

size_t allocations;
size_t allocatedSize;
size_t totalAllocations;
size_t totalSize;
size_t unmanagedDeletions;

#if defined(PLANK_MAX_ZOMBIES_SIZE) || defined(PLANK_MAX_ZOMBIES)
size_t totalZombies;
size_t totalZombieSize;
size_t releasedZombies;
size_t releasedZombieSize;
#endif

bool firstAlloc;

struct plankentry
{
  void *ptr;
  size_t size;
  plankentry* next;
};

plankentry* entries;
plankentry* zombies;

void plank_stats()
{
  printf("[plankmm]Statistics\n");
  printf("  Current Allocations: %i [%i KBytes]\n", (int)allocations, (int)allocatedSize/1024);
  printf("  Total Allocations: %i [%i KBytes]\n", (int)totalAllocations, (int)totalSize/1024);
  printf("  Unmanaged Deletions: %i\n", (int)unmanagedDeletions);
#if defined(PLANK_MAX_ZOMBIES_SIZE) || defined(PLANK_MAX_ZOMBIES)
  printf("  Released Zombies: %i [%i KBytes]\n", (int)releasedZombies, (int)releasedZombieSize/1024);
#endif
}

#if defined(PLANK_MAX_ZOMBIES_SIZE) || defined(PLANK_MAX_ZOMBIES)
void plank_refresh()
{
  while(true)
  {
    if(!zombies) break;
    bool ex = true;
#ifdef PLANK_MAX_ZOMBIES_SIZE
    if(totalZombieSize > PLANK_MAX_ZOMBIES_SIZE) ex = false;
#endif
#ifdef PLANK_MAX_ZOMBIES
    if(totalZombies > PLANK_MAX_ZOMBIES) ex = false;
#endif
    if(ex) break;
    plankentry* curr = zombies;
    zombies = curr->next;

    totalZombieSize -= curr->size;
    totalZombies --;
    releasedZombieSize += curr->size;
    releasedZombies ++;

    mprotect(curr->ptr, curr->size, PROT_WRITE);
    //free(curr->ptr);
    munmap(curr->ptr, curr->size);
    free(curr);
  }
}
#endif

void* do_new(size_t size)
{
  void *rtn = NULL;

  if(!firstAlloc)
  {
    firstAlloc = true;
    atexit(plank_stats);

    printf("[plankmm]Initialized\n");
  }

  if(!size) size = 4;

#ifdef _WIN32
  rtn = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#else
  rtn = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
/*
  size = getpagesize() * ((size / getpagesize()) + 1);
  //rtn = calloc(1, size);
  if(posix_memalign(&rtn, getpagesize(), size) != 0)
  {
    rtn = NULL;
  }
*/
#endif

  if(!rtn)
  {
    printf("[plankmm]NULL\n");
    return NULL;
  }

  plankentry* newEntry = (plankentry*)malloc(sizeof(*newEntry));

  if(!newEntry) return NULL;

  newEntry->ptr = rtn;
  newEntry->size = size;
  newEntry->next = entries;
  entries = newEntry;

  allocations++;
  totalAllocations++;
  allocatedSize += size;
  totalSize += size;

#if defined(PLANK_MAX_ZOMBIES_SIZE) || defined(PLANK_MAX_ZOMBIES)
  plank_refresh();
#endif

  return rtn;
}

void do_delete(void* ptr)
{
  if(!ptr)
  {
    return;
  }

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

#if defined(PLANK_MAX_ZOMBIES_SIZE) || defined(PLANK_MAX_ZOMBIES)
      curr->next = zombies;
      zombies = curr;
      totalZombies++;
      totalZombieSize += curr->size;
#else
      free(curr);
#endif
      break;
    }

    prev = curr;
    curr = curr->next;
  }

  if(!found)
  {
    printf("[plankmm]Unmanaged deletion [%p]\n", ptr);
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
