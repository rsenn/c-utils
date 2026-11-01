#include "unit_test.h"
#include "../lib/byte.h"
#include "../lib/alloc.h"
#include "../lib/slist.h"
#include "../lib/str.h"

typedef struct {
  slink link;
  int v;
} item;

static item
make(int v) {
  item i;
  i.link.next = NULL;
  i.v = v;
  return i;
}

static int
is_two(slink* l) {
  return ((item*)l)->v == 2;
}

static int
is_nine(slink* l) {
  return ((item*)l)->v == 9;
}

/* the values in the list, as digits */
static void
dump(slink* head, char* out) {
  for(; head; head = head->next)
    *out++ = (char)('0' + ((item*)head)->v);
  *out = '\0';
}

/*
 * void slist_init(slink**);
 * slink** slist_push(slink**, void*);
 * size_t slist_length(slink**);
 */
TEST(test_slist_push_length) {
  item a = make(1), b = make(2), c = make(3);
  slink* head = (slink*)0x1;
  char out[8];

  slist_init(&head);
  ASSERT_EQ(NULL, head);
  ASSERT_EQ(0, slist_length(&head));

  slist_push(&head, &a);
  slist_push(&head, &b);
  slist_push(&head, &c);
  ASSERT_EQ(3, slist_length(&head));
  ASSERT_EQ(3, slist_size(head));
  dump(head, out);
  ASSERT_STR_EQUAL("123", out);
}

/*
 * void slist_add_after(slink**, slink*, slink*);
 * void slist_add_before(slink**, slink*, slink*);
 * slink* slist_add / slist_insert
 */
TEST(test_slist_add) {
  item a = make(1), b = make(2), c = make(3), d = make(4), e = make(5);
  slink* head = NULL;
  char out[8];

  slist_add(&head, &a.link);
  slist_add(&head, &b.link); /* the front */
  dump(head, out);
  ASSERT_STR_EQUAL("21", out);

  /* after b */
  slist_add_after(&head, &c.link, &b.link);
  dump(head, out);
  ASSERT_STR_EQUAL("231", out);

  /* after nothing: at the end */
  slist_add_after(&head, &d.link, NULL);
  dump(head, out);
  ASSERT_STR_EQUAL("2314", out);

  /* before a, and before the head */
  slist_add_before(&head, &e.link, &a.link);
  dump(head, out);
  ASSERT_STR_EQUAL("23514", out);
  {
    item f = make(6);

    slist_add_before(&head, &f.link, head);
    dump(head, out);
    ASSERT_STR_EQUAL("623514", out);
  }
}

TEST(test_slist_insert_macro) {
  item a = make(1), b = make(2);
  slink* head = NULL;
  slink** at = &head;
  char out[8];

  at = slist_insert(at, &a);
  at = slist_insert(at, &b);
  dump(head, out);
  ASSERT_STR_EQUAL("12", out);
  ASSERT_EQ(&b.link.next, at);
}

/*
 * slink** slist_find(slink**, int (*)(slink*));
 */
TEST(test_slist_find) {
  item a = make(1), b = make(2), c = make(3);
  slink* head = NULL;
  slink** r;

  slist_push(&head, &a);
  slist_push(&head, &b);
  slist_push(&head, &c);

  r = slist_find(&head, is_two);
  ASSERT_NE(NULL, r);
  ASSERT_EQ((void*)&b, (void*)*r);
  ASSERT_EQ(&a.link.next, r);

  /* no match: NULL, and it must come back */
  ASSERT_EQ(NULL, slist_find(&head, is_nine));

  head = NULL;
  ASSERT_EQ(NULL, slist_find(&head, is_two));
}

/*
 * slink* slist_find_delete(slink**, int (*)(slink*));
 */
TEST(test_slist_find_delete) {
  item a = make(1), b = make(2), c = make(3);
  slink *head = NULL, *r;
  char out[8];

  slist_push(&head, &a);
  slist_push(&head, &b);
  slist_push(&head, &c);

  r = slist_find_delete(&head, is_two);
  ASSERT_EQ((void*)&b, (void*)r);
  ASSERT_EQ(NULL, r->next);
  dump(head, out);
  ASSERT_STR_EQUAL("13", out);

  /* nothing to delete: NULL and the list is as it was */
  ASSERT_EQ(NULL, slist_find_delete(&head, is_nine));
  dump(head, out);
  ASSERT_STR_EQUAL("13", out);

  ASSERT_EQ(NULL, slist_find_delete(&head, is_nine));
}

/*
 * slink* slist_remove(slink**);
 * slink* slist_shift(slink**);
 */
TEST(test_slist_remove_shift) {
  item a = make(1), b = make(2), c = make(3);
  slink *head = NULL, *r;
  char out[8];

  slist_push(&head, &a);
  slist_push(&head, &b);
  slist_push(&head, &c);

  r = slist_remove(&a.link.next);
  ASSERT_EQ((void*)&b, (void*)r);
  ASSERT_EQ(NULL, r->next);
  dump(head, out);
  ASSERT_STR_EQUAL("13", out);

  r = slist_shift(&head);
  ASSERT_EQ((void*)&a, (void*)r);
  ASSERT_EQ(NULL, r->next);
  dump(head, out);
  ASSERT_STR_EQUAL("3", out);

  r = slist_shift(&head);
  ASSERT_EQ((void*)&c, (void*)r);
  ASSERT_EQ(NULL, head);
}

/*
 * int slist_pushs(slink**, const char*);
 * int slist_unshifts(slink**, const char*);
 * void* slist_pushb(slink**, const void*, size_t);
 * int slist_unshiftb(slink**, const void*, size_t);
 * int slist_shifts(slink**);
 * void slist_removes(slink**);
 */
TEST(test_slist_strings) {
  slink* head = NULL;

  ASSERT_EQ(1, slist_pushs(&head, "two"));
  ASSERT_EQ(1, slist_pushs(&head, "three"));
  ASSERT_EQ(1, slist_unshifts(&head, "one"));
  ASSERT_EQ(3, slist_size(head));

  ASSERT_STR_EQUAL("one", *(char**)slink_data(&head));
  ASSERT_STR_EQUAL("two", *(char**)slist_data(head->next));
  ASSERT_STR_EQUAL("three", *(char**)slist_data(head->next->next));

  /* the second one is removed with its string */
  slist_removes(&head->next);
  ASSERT_EQ(2, slist_size(head));
  ASSERT_STR_EQUAL("three", *(char**)slist_data(head->next));

  slist_shifts(&head);
  ASSERT_STR_EQUAL("three", *(char**)slink_data(&head));
  slist_shifts(&head);
  ASSERT_EQ(NULL, head);
}

TEST(test_slist_bytes) {
  slink* head = NULL;
  static const char x[4] = {1, 2, 3, 4};
  static const char y[4] = {5, 6, 7, 8};

  ASSERT_NE(NULL, slist_pushb(&head, x, 4));
  ASSERT_EQ(1, slist_unshiftb(&head, y, 4));
  ASSERT_EQ(2, slist_size(head));

  ASSERT_EQ(0, byte_diff(slist_data(head), 4, y));
  ASSERT_EQ(0, byte_diff(slist_data(head->next), 4, x));
  ASSERT_EQ(NULL, head->next->next);

  alloc_free(head->next);
  alloc_free(head);
}

/*
 * iterator helpers and foreach macros
 */
TEST(test_slist_iterate) {
  item a = make(1), b = make(2), c = make(3);
  slink* head = NULL;
  slink *n, *next;
  slink** it;
  int sum = 0;

  slist_push(&head, &a);
  slist_push(&head, &b);
  slist_push(&head, &c);

  slist_foreach(head, n) sum += ((item*)n)->v;
  ASSERT_EQ(6, sum);

  sum = 0;
  slist_foreach_safe(head, n, next) sum += ((item*)n)->v;
  ASSERT_EQ(6, sum);

  ASSERT_EQ(3, slink_size(&head));
  ASSERT_EQ(&c.link.next, slink_tail(&head));
  ASSERT_EQ((void*)&c, (void*)slist_end(head));
  ASSERT_EQ(0, slist_islast(head));
  ASSERT_NE(0, slist_islast(&c.link));

  it = &head;
  ASSERT_NE(0, slist_iterator_first(&head, it));
  slist_iterator_increment(&head, &it);
  ASSERT_EQ(0, slist_iterator_first(&head, it));
  ASSERT_EQ((void*)&b, (void*)slist_iterator_dereference(&head, it));
  ASSERT_EQ(1, slist_iterator_distance(&head, &head, it));
  ASSERT_EQ(0, slist_iterator_last(&head, it));
  slist_iterator_increment(&head, &it);
  ASSERT_NE(0, slist_iterator_last(&head, it));
  slist_iterator_increment(&head, &it);
  ASSERT_NE(0, slist_iterator_end(&head, it));
  ASSERT_NE(0, slist_iterator_equal(&head, it, slink_end(&head)));
}

#define RUN_SLIST_TESTS() \
  RUN(test_slist_push_length); \
  RUN(test_slist_add); \
  RUN(test_slist_insert_macro); \
  RUN(test_slist_find); \
  RUN(test_slist_find_delete); \
  RUN(test_slist_remove_shift); \
  RUN(test_slist_strings); \
  RUN(test_slist_bytes); \
  RUN(test_slist_iterate)

TESTS(slist) { RUN_SLIST_TESTS(); }
