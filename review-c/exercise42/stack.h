#ifndef stack_h
#define stack_h

#include "list.h"

typedef ListNode StackNode;
typedef List Stack;

#define Stack_create List_create
#define Stack_destroy List_destroy
#define Stack_clear List_clear
#define Stack_clear_destroy List_clear_destroy

#define Stack_count List_count
#define Stack_peek List_last

#define Stack_push List_push
#define Stack_pop List_pop

#define STACK_FOREACH(L, V)  LIST_FOREACH(L, first, next, V) 

#endif