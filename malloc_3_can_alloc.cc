// libstdc++-v3/libsupc++/eh_alloc.cc
pool::pool() noexcept {
  // ...
  arena_size = buffer_size_in_bytes(obj_count, obj_size);
  if (arena_size == 0)
    return;
  arena = (char *)malloc (arena_size);
  if (!arena) {
    // If the allocation failed go without an emergency pool.
    arena_size = 0;
    return;
  }

  // Populate the free-list with a single entry covering the whole arena
  first_free_entry = reinterpret_cast <free_entry *> (arena);
  new (first_free_entry) free_entry;
  first_free_entry->size = arena_size;
  first_free_entry->next = NULL;
}

  // ...

extern "C" void *
__cxxabiv1::__cxa_allocate_exception(std::size_t thrown_size) noexcept {
  // ..
  void *ret = malloc (thrown_size);

#if USE_POOL
  if (!ret)
    ret = emergency_pool.allocate (thrown_size);
#endif
  // ...
}


// The size of the buffer is N * (S * P + R + D), where:
// N == The number of objects to reserve space for.
//      Defaults to EMERGENCY_OBJ_COUNT, defined below.
// S == Estimated size of exception objects to account for.
//      This size is in units of sizeof(void*) not bytes.
//      Defaults to EMERGENCY_OBJ_SIZE, defined below.
// P == sizeof(void*).
// R == sizeof(__cxa_refcounted_exception).
// D == sizeof(__cxa_dependent_exception).


// ...
#define EMERGENCY_OBJ_SIZE 6
#define EMERGENCY_OBJ_COUNT (4 * __SIZEOF_POINTER__ * __SIZEOF_POINTER__)

// Manualy tune the object size (obj_size) and number of objects (obj_count) using GLIBCXX_TUNABLES environment variable. 
// Then confirm the initial allocation is used for the emergency pool by changing the number of objects in the pool. 
// The initial allocation size descreases after changing the number of objects

$ GLIBCXX_TUNABLES=glibcxx.eh_pool.obj_count=10 \
  LOG_ALLOC=log.txt \
  LD_PRELOAD=/home/joel/mymalloc/libmymalloc.so \
  ls
$ head -n 1 log.txt
2880

//the emergency pool can also be disabled (i.e., not allocated), by setting the number of objects to 0
//Alternatively, you can opt-in to use a fixed-size static buffer for the emergency pool by configuring --enable-libstdcxx-static-eh-pool when building libstdc++.

  
// Valgrind and Memory Leaks

  /*
==1174489== HEAP SUMMARY:
==1174489==     in use at exit: 0 bytes in 0 blocks
==1174489==   total heap usage: 1 allocs, 1 frees, 73,728 bytes allocated

“still reachable” rather than properly freed. 
While “still reachable” memory isn’t technically a leak (the program still has references to it), 
This will result in a 71 KB allocation instead of 72 KB
Later versions of Valgrind patched this away

==8511== HEAP SUMMARY:
==8511==     in use at exit: 72,704 bytes in 1 blocks
==8511==   total heap usage: 1 allocs, 0 frees, 72,704 bytes allocated
==8511==
==8511== LEAK SUMMARY:
==8511==    definitely lost: 0 bytes in 0 blocks
==8511==    indirectly lost: 0 bytes in 0 blocks
==8511==      possibly lost: 0 bytes in 0 blocks
==8511==    still reachable: 72,704 bytes in 1 blocks
==8511==         suppressed: 0 bytes in 0 blocks

coregrind/vg_preloaded.c:

/* g++ mangled __gnu_cxx::__freeres yields -> _ZN9__gnu_cxx9__freeresEv */
extern void _ZN9__gnu_cxx9__freeresEv(void) __attribute__((weak));
if (((to_run & VG_RUN__GNU_CXX__FREERES) != 0) &&
  (_ZN9__gnu_cxx9__freeresEv != NULL)) {
  _ZN9__gnu_cxx9__freeresEv();
}

// ...

namespace __gnu_cxx {
  __attribute__((cold)) void __freeres() noexcept {
#ifndef _GLIBCXX_EH_POOL_STATIC
    if (emergency_pool.arena) {
      ::free(emergency_pool.arena);
      emergency_pool.arena = 0;
    }
#endif
  }
}



