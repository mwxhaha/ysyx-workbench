#ifndef queue_h
#define queue_h

#include "list.h"

typedef ListNode QueueNode;
typedef List Queue;

#define Queue_create List_create
#define Queue_destroy List_destroy
#define Queue_clear List_clear
#define Queue_clear_destroy List_clear_destroy

#define Queue_count List_count
#define Queue_peek List_first

#define Queue_send List_push
#define Queue_recv List_shift

#define QUEUE_FOREACH(L, V)  LIST_FOREACH(L, first, next, V) 

#endif