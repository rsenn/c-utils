#include "unit_test.h"
#include "../lib/dlist.h"

typedef struct {
  node link;
  int v;
} item;

static item
make(int v) {
  item i;
  i.link.prev = i.link.next = NULL;
  i.v = v;
  return i;
}

static int
is_two(void* data) {
  return *(int*)data == 2;
}

static int
is_nine(void* data) {
  return *(int*)data == 9;
}

/* the values from the head, then the values from the tail, as digits */
static void
dump(dlist* l, char* out) {
  node* n;

  for(n = l->head; n; n = n->next)
    *out++ = (char)('0' + ((item*)n)->v);
  *out++ = '|';
  for(n = l->tail; n; n = n->prev)
    *out++ = (char)('0' + ((item*)n)->v);
  *out = '\0';
}

/*
 * void dlist_init(dlist*);
 * void dlist_push(dlist*, node*);
 * void dlist_unshift(dlist*, node*);
 * size_t dlist_length(dlist*);
 */
TEST(test_dlist_push_unshift) {
  item a = make(1), b = make(2), c = make(3);
  dlist l;
  char out[16];

  dlist_init(&l);
  ASSERT_EQ(NULL, l.head);
  ASSERT_EQ(NULL, l.tail);
  ASSERT_EQ(0, dlist_length(&l));

  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);
  dlist_unshift(&l, &a.link);
  ASSERT_EQ(3, dlist_length(&l));
  dump(&l, out);
  ASSERT_STR_EQUAL("123|321", out);
}

/*
 * void dlist_remove(dlist*, node*);
 */
TEST(test_dlist_remove) {
  item a = make(1), b = make(2), c = make(3);
  dlist l;
  char out[16];

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);

  dlist_remove(&l, &b.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("13|31", out);
  ASSERT_EQ(NULL, b.link.next);
  ASSERT_EQ(NULL, b.link.prev);

  dlist_remove(&l, &a.link); /* the head */
  dump(&l, out);
  ASSERT_STR_EQUAL("3|3", out);

  dlist_remove(&l, &c.link); /* the last one */
  ASSERT_EQ(NULL, l.head);
  ASSERT_EQ(NULL, l.tail);
}

/*
 * void dlist_add_after(dlist*, node*, node*);
 * void dlist_add_before(dlist*, node*, node*);
 */
TEST(test_dlist_add_after_before) {
  item a = make(1), b = make(2), c = make(3), d = make(4), e = make(5), f = make(6);
  dlist l;
  char out[16];

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &c.link);

  dlist_add_after(&l, &b.link, &a.link); /* in the middle */
  dump(&l, out);
  ASSERT_STR_EQUAL("123|321", out);

  dlist_add_after(&l, &d.link, &c.link); /* after the tail */
  dump(&l, out);
  ASSERT_STR_EQUAL("1234|4321", out);

  dlist_add_before(&l, &e.link, &c.link); /* in the middle */
  dump(&l, out);
  ASSERT_STR_EQUAL("12534|43521", out);

  dlist_add_before(&l, &f.link, &a.link); /* before the head */
  dump(&l, out);
  ASSERT_STR_EQUAL("612534|435216", out);
}

/*
 * node* dlist_find(dlist*, int (*)(void*));
 * node* dlist_find_delete(dlist*, int (*)(void*));
 */
TEST(test_dlist_find) {
  item a = make(1), b = make(2), c = make(3);
  dlist l;
  char out[16];

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);

  ASSERT_EQ((void*)&b, (void*)dlist_find(&l, is_two));
  ASSERT_EQ(NULL, dlist_find(&l, is_nine));

  ASSERT_EQ((void*)&b, (void*)dlist_find_delete(&l, is_two));
  dump(&l, out);
  ASSERT_STR_EQUAL("13|31", out);
  ASSERT_EQ(NULL, dlist_find_delete(&l, is_nine));
  dump(&l, out);
  ASSERT_STR_EQUAL("13|31", out);
}

/*
 * void dlist_swap(dlist*, node*, node*);
 */
TEST(test_dlist_swap) {
  item a = make(1), b = make(2), c = make(3), d = make(4);
  dlist l;
  char out[16];

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);
  dlist_push(&l, &d.link);

  /* not neighbours, with the head and the tail */
  dlist_swap(&l, &a.link, &d.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("4231|1324", out);
  dlist_swap(&l, &d.link, &a.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("1234|4321", out);

  /* neighbours */
  dlist_swap(&l, &b.link, &c.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("1324|4231", out);
  dlist_swap(&l, &c.link, &b.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("1234|4321", out);

  dlist_swap(&l, &a.link, &b.link); /* neighbours at the head */
  dump(&l, out);
  ASSERT_STR_EQUAL("2134|4312", out);
  dlist_swap(&l, &c.link, &d.link); /* neighbours at the tail */
  dump(&l, out);
  ASSERT_STR_EQUAL("2143|3412", out);

  /* the same node twice */
  dlist_swap(&l, &a.link, &a.link);
  dump(&l, out);
  ASSERT_STR_EQUAL("2143|3412", out);
}

/*
 * void dlist_move_head(dlist*, dlist*);
 * void dlist_move_tail(dlist*, dlist*);
 */
TEST(test_dlist_move) {
  item a = make(1), b = make(2), c = make(3), d = make(4);
  dlist from, to;
  char out[16];

  /* to the tail of to */
  dlist_init(&from);
  dlist_init(&to);
  dlist_push(&to, &a.link);
  dlist_push(&to, &b.link);
  dlist_push(&from, &c.link);
  dlist_push(&from, &d.link);
  dlist_move_tail(&from, &to);
  dump(&to, out);
  ASSERT_STR_EQUAL("1234|4321", out);
  ASSERT_EQ(NULL, from.head);
  ASSERT_EQ(NULL, from.tail);

  /* to the head of to */
  dlist_init(&from);
  dlist_init(&to);
  a = make(1); b = make(2); c = make(3); d = make(4);
  dlist_push(&to, &c.link);
  dlist_push(&to, &d.link);
  dlist_push(&from, &a.link);
  dlist_push(&from, &b.link);
  dlist_move_head(&from, &to);
  dump(&to, out);
  ASSERT_STR_EQUAL("1234|4321", out);
  ASSERT_EQ(NULL, from.head);
  ASSERT_EQ(NULL, from.tail);

  /* into an empty list */
  dlist_init(&from);
  dlist_init(&to);
  a = make(1); b = make(2);
  dlist_push(&from, &a.link);
  dlist_push(&from, &b.link);
  dlist_move_tail(&from, &to);
  dump(&to, out);
  ASSERT_STR_EQUAL("12|21", out);
  ASSERT_EQ(NULL, from.head);
}

/*
 * dlist_foreach_down / dlist_foreach_up / dlist_data
 */
TEST(test_dlist_foreach) {
  item a = make(1), b = make(2), c = make(3);
  dlist l;
  node* n;
  int up = 0, down = 0;

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);

  dlist_foreach_down(&l, n) down = down * 10 + dlist_data(n, int);
  dlist_foreach_up(&l, n) up = up * 10 + dlist_data(n, int);
  ASSERT_EQ(123, down);
  ASSERT_EQ(321, up);
}

/* the safe walks allow to take the current node out */
TEST(test_dlist_foreach_safe) {
  item a = make(1), b = make(2), c = make(3);
  dlist l;
  node *n, *m;
  int seen = 0;

  dlist_init(&l);
  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);

  dlist_foreach_down_safe(&l, n, m) {
    seen = seen * 10 + dlist_data(n, int);
    dlist_remove(&l, n);
  }
  ASSERT_EQ(123, seen);
  ASSERT_EQ(0, dlist_length(&l));

  dlist_push(&l, &a.link);
  dlist_push(&l, &b.link);
  dlist_push(&l, &c.link);
  seen = 0;
  dlist_foreach_up_safe(&l, n, m) {
    seen = seen * 10 + dlist_data(n, int);
    dlist_remove(&l, n);
  }
  ASSERT_EQ(321, seen);
  ASSERT_EQ(0, dlist_length(&l));
}

#define RUN_DLIST_TESTS() \
  RUN(test_dlist_push_unshift); \
  RUN(test_dlist_remove); \
  RUN(test_dlist_add_after_before); \
  RUN(test_dlist_find); \
  RUN(test_dlist_swap); \
  RUN(test_dlist_move); \
  RUN(test_dlist_foreach); \
  RUN(test_dlist_foreach_safe)

TESTS(dlist) { RUN_DLIST_TESTS(); }
