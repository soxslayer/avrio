#include <stdlib.h>

#include "list.h"

void list_init(list_t *list)
{
  list->prev = NULL;
  list->next = NULL;
}

void list_insert(list_t **list, list_t *node)
{
  list_init(node);

  if (!(*list)) {
    *list = node;
    return;
  }

  node->prev = *list;
  node->next = (*list)->next;

  if ((*list)->next)
    (*list)->next->prev = node;

  (*list)->next = node;
}

void list_remove(list_t **list)
{
  if (!(*list))
    return;

  if ((*list)->prev)
    (*list)->prev->next = (*list)->next;

  if ((*list)->next)
    (*list)->next->prev = (*list)->prev;

  if (!(*list)->prev && !(*list)->next)
    *list = NULL;
}
