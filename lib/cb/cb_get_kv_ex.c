void
cb_get_kv_ex(void* kv, void** value) {
  char* key = (char*)kv;
  size_t keylen = str_len(key) + 1;
  *value = key + keylen;
}
