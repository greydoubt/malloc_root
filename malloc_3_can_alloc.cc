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
