#define HMAP_SET_DATA() {                                                              \
  switch( data_type ){                                                                \
      case HMAP_DATA_TYPE_INT:                                                        \
          new_tuple->vals.val_int = va_arg(args, int);                                \
          HMAP_DEBUG("add[%d] data[%d]\n", index, new_tuple->vals.val_int);                \
          break;                                                                      \
      case HMAP_DATA_TYPE_UINT:                                                       \
          new_tuple->vals.val_uint = va_arg(args, unsigned int);                      \
          HMAP_DEBUG("add[%d] data[%u]\n", index, new_tuple->vals.val_uint);               \
          break;                                                                      \
      case HMAP_DATA_TYPE_INT64:                                                      \
          new_tuple->vals.val_int64 = va_arg(args, long int);                         \
          HMAP_DEBUG("add[%d] data[%ld]\n", index, new_tuple->vals.val_int64);             \
          break;                                                                      \
      case HMAP_DATA_TYPE_UNIT64:                                                     \
          new_tuple->vals.val_uint64 = va_arg(args, unsigned long int);               \
          HMAP_DEBUG("add[%d] data[%lu]\n", index, new_tuple->vals.val_uint64);            \
          break;                                                                      \
      case HMAP_DATA_TYPE_DOUBLE:                                                     \
          new_tuple->vals.val_double = va_arg(args, double);                          \
          HMAP_DEBUG("add[%d] data[%f]\n", index, new_tuple->vals.val_double);             \
          break;                                                                      \
      case HMAP_DATA_TYPE_CHARS:                                                      \
          data  = va_arg(args, char *);                                               \
          d_len = va_arg(args, int);                                                  \
          new_tuple->vals.val_chars = (char *)calloc(1, d_len+1);                     \
          if(new_tuple->vals.val_chars == NULL) {                                     \
              free(new_tuple);                                                        \
              return HMAP_ALLOCATED_ERROR;                                            \
          }                                                                           \
          if( memcpy( new_tuple->vals.val_chars, data, d_len ) == NULL ){            \
              free(new_tuple->vals.val_chars);                                        \
              free(new_tuple);                                                        \
              return  HMAP_FAILED;                                                    \
          }                                                                           \
          ((char *)new_tuple->vals.val_chars)[d_len] = 0;                             \
          new_tuple->data_len = d_len;                                                \
          HMAP_DEBUG("add[%d] data[%s]\n", index, (char *)new_tuple->vals.val_chars);      \
          break;                                                                      \
      case HMAP_DATA_TYPE_CUSTOM:                                                     \
          new_tuple->vals.val_custom  = va_arg(args, void *);                         \
          HMAP_DEBUG("add[%d] data[%p]\n", index, new_tuple->vals.val_custom);             \
          break;                                                                      \
      default :                                                                       \
        return HMAP_DATA_TYPE_INVALID;                                                \
  }                                                                                   \
} 

inline static int
hmap_cmp(void *v1, int v1_len, void *v2, int v2_len) {
    if(v1_len != v2_len) {
        return -1;
    }
    return memcmp(v1, v2, v1_len);
}

inline static int
hash (void *key, int len) {
   unsigned int hash, i;

   for (hash = i = 0; i < len; ++i)
   {
      hash += ((unsigned char *)key)[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
   }
   hash += (hash << 3);
   hash ^= (hash >> 11);
   hash += (hash << 15);
   return hash & 0x7FFFFFFF;
}
