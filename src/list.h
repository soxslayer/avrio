#ifndef __LIST__
#define __LIST__

typedef struct _list
{
  struct _list *prev;
  struct _list *next;
} list_t;

void list_insert(list_t **list, list_t *node);
void list_remove(list_t **list);

#endif /* __LIST__ */
