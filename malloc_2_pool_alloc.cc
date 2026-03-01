char* __pool_alloc_base::_M_allocate_chunk(size_t __n, int& __nobjs) {
  // ...
  __try {
    _S_start_free = static_cast<char*>(::operator new(__bytes_to_get));
  } __catch(const std::bad_alloc&) { /* ... */ }
  // ...
}
