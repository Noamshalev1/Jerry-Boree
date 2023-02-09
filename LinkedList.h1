#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "Defs.h"

typedef struct LinkedList_t* List;

List createLinkedList(CopyFunction copyfunction, FreeFunction freefunction, PrintFunction printfunction, EqualFunction isEqualKey);
status destroyList(List); // get a list and delete it
status appendNode(List, Element); // get a list and element and add the element to list from the end
status deleteNode(List, Element); // get a list and element and delete the element from list
void displayList(List); // print the list
Element getDataByIndex(List, int); // return the data of node in place i
int getLengthList(List); // return the size of list
Element searchByKeyInList(List, Element); // search an element by some data about it

#endif
