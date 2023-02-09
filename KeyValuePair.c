#include "KeyValuePair.h"

struct Pair_t{
    Element key;
    Element value;
    CopyFunction copyKey;
    CopyFunction copyValue;
    FreeFunction freeKey;
    FreeFunction freeValue;
    PrintFunction printKey;
    PrintFunction printValue;
    EqualFunction isEqualKey;
};

Pair createKeyValuePair(Element key, Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction isEqualKey)
{
    if(key == NULL || value == NULL || copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || isEqualKey == NULL)
        return NULL;
    /* create a new pair */
    Pair newPair = NULL;
    newPair = (Pair)malloc(sizeof(struct Pair_t));
    if(newPair == NULL)
        return NULL;
    /* init fields */
    newPair->copyKey = copyKey;
    newPair->freeKey = freeKey;
    newPair->copyValue = copyValue;
    newPair->freeValue = freeValue;
    newPair->printKey = printKey;
    newPair->printValue = printValue;
    newPair->isEqualKey = isEqualKey;
    /* insert data to key and value by copy function */
    newPair->key = newPair->copyKey(key);
    newPair->value = newPair->copyValue(value);
    if(newPair->key == NULL || newPair->value == NULL)
    {
        free(newPair);
        return NULL;
    }
    return newPair;
}
status destroyKeyValuePair(Pair pair)
{
    if(pair == NULL)
        return failure;
    if(pair->key != NULL)
        pair->freeKey(pair->key); // function that free the key
    if(pair->value != NULL)
        pair->freeValue(pair->value); // function that free the value
    free(pair);
    return success;
}
void displayValue(Pair pair)
{
    if(pair == NULL)
        return;
    pair->printValue(pair->value); // function that print the value
}
void displayKey(Pair pair)
{
    if(pair == NULL)
        return;
    pair->printKey(pair->key); // function that print the key
}
Element getValue(Pair pair)
{
    if(pair == NULL)
        return NULL;
    return(pair->value); // return the value of pair
}
Element getKey(Pair pair)
{
    return(pair->key); // return the key of pair
}
bool isEqualKey(Pair pair, Element key)
{
    if(pair == NULL || key == NULL)
        return false;
    return(pair->isEqualKey(pair->key, key)); // check if the key is equal to the pair_key
}
