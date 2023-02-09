#ifndef MULTI_HASH
#define MULTI_HASH
/* this statement is for lookup function that return a list */
#include "LinkedList.h"

typedef struct multihashTable_s *multiHash;

multiHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int multihashNumber);
status destroyMultiValueHashTable(multiHash); // delete the multi hashtable
status addToMultiValueHashTable(multiHash, Element key, Element value); // insert a new (key,value) to struct
List lookupInMultiValueHashTable(multiHash, Element key); // search for values of key
status removeFromMultiValueHashTable(multiHash, Element key, Element); // delete a value from key in struct
status displayMultiValueHashElementsByKey(multiHash, Element key); // print the multi hashtable

#endif
