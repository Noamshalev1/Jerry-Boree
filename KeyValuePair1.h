#ifndef KEY_VALUE_PAIR_H
#define  KEY_VALUE_PAIR_H
#include "Defs.h"

typedef struct Pair_t* Pair;

Pair createKeyValuePair(Element key, Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction isEqualKey);
status destroyKeyValuePair(Pair); // delete the pair
void displayValue(Pair); // print the value only
void displayKey(Pair); // print the key
Element getValue(Pair); // return the value
Element getKey(Pair); // return the key
bool isEqualKey(Pair, Element); // check if the element is equal the key


#endif
