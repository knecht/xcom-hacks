/* See README.md for documentation and instructions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __USE_GNU
#include <dlfcn.h>

#define dprintf(...)

static void* (*real_malloc)(size_t)=NULL;
static void* (*real_calloc)(size_t,size_t)=NULL;
static void* (*real_realloc)(void*,size_t)=NULL;
static void (*real_free)(void*)=NULL;

typedef unsigned char bool;
#define true 1
#define false 0

bool during_init = false;

static void mtrace_init(void) {
  during_init = true;
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if (NULL == real_malloc) {
    fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    abort();
  }
  real_free = dlsym(RTLD_NEXT, "free");
  real_calloc = dlsym(RTLD_NEXT, "calloc");
  real_realloc = dlsym(RTLD_NEXT, "realloc");
  during_init = false;
}

#define PREFIXLEN 128
#define SUFFIXLEN 128
#define SIGLEN 7
char* sig = "MeMhAcK";

char* findRealPtr(char* ptr) {
  if (NULL == ptr) return NULL;
  dprintf(stderr, "danger...");
  for (int i=0;i<SIGLEN;i++) {
    if (ptr[i-SIGLEN] != sig[i]) {
      dprintf(stderr, "not ours %d : %p\n", i, ptr);
      return ptr;
    }
  }
  dprintf(stderr, "ours\n");
  return ptr-PREFIXLEN;
}

void free(void* ptr) {
  if (ptr == NULL) return;
  dprintf(stderr, "free(%p)\n", ptr);
  if(real_free==NULL) {
    mtrace_init();
  }
  void *realptr = findRealPtr((char*)ptr);
  if (realptr == ptr) {
    dprintf(stderr, "not freeing foreign ptr %p\n", ptr);
  }  
  real_free(realptr);
}
void *malloc(size_t size)
{
  dprintf(stderr, "malloc(%ld) = ", size);
  return calloc(1, size);
}

char hackbuf1[32];

void *calloc(size_t nmemb, size_t size) {
  dprintf(stderr, "calloc(%ld,%ld) = ", nmemb, size);
  if (during_init) {
    if (nmemb != 1) abort();
    if (size != 32) abort();
    for (int i=0;i<32;i++) hackbuf1[i]=0;
    return hackbuf1;
  }
  if (real_malloc==NULL) {
    mtrace_init();
  }

  void *p = NULL;
    
  p = real_calloc(PREFIXLEN+(size*nmemb)+SUFFIXLEN,1);
  strncpy(p+PREFIXLEN-SIGLEN, sig, SIGLEN);
  dprintf(stderr, " %p -> %p\n", p, p+PREFIXLEN);
  return p+PREFIXLEN;
}
void* realloc(void *ptr, size_t size) {
  if (NULL == ptr) return malloc(size);
  if (size == 0) {
    free(ptr);
    return NULL;
  }
  dprintf(stderr, "realloc %p, %ld\n", ptr,size);
  void *realptr = findRealPtr((char*)ptr);
  if (realptr == ptr) {
    dprintf(stderr, "realloc foreign ptr %p\n", ptr);
    return real_realloc(ptr, size+SUFFIXLEN);
  }
  void *newptr= real_realloc(realptr, PREFIXLEN+size+SUFFIXLEN);
  dprintf(stderr, "realloc ret %p\n", newptr+PREFIXLEN);
  return newptr +PREFIXLEN;
}
