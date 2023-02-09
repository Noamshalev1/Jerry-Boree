#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "KeyValuePair.h"

/* the struct is exactly the same as hashtable */
struct multihashTable_s
{
    hashTable hashtable;
    PrintFunction printkey;

};

multiHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int multihashNumber)
{
    /* create hashtable function will check all the pointers, so we avoid double-checking */
    multiHash multi = (multiHash)malloc(sizeof(struct multihashTable_s));
    if(multi == NULL)
        return NULL;
    /* create a new hash table */
    multi->hashtable = createHashTable(copyKey, freeKey, printKey, copyValue, freeValue, printValue, equalKey, transformIntoNumber, multihashNumber);
    if(multi->hashtable == NULL)
        return NULL;
    multi->printkey = printKey;
    /* return a pointer to new multi-hashtable */
    return (multiHash)multi;
}
status destroyMultiValueHashTable(multiHash multi)
{
    if(multi == NULL)
        return failure;
    status result = destroyHashTable((hashTable) multi->hashtable);
    free(multi);
    return result;

}
status addToMultiValueHashTable(multiHash multi, Element key, Element value) // value is a list
{
    status result = success;
    if(multi == NULL ||key == NULL || value == NULL)
        return failure;
    List values = lookupInMultiValueHashTable(multi, key);
    /* if list is empty - we need to add this key and values to hash */
    if(values == NULL)
    {
        result = addToHashTable(multi->hashtable, key, value);
        return result;
    }
    else
    {
        /* search the value in values list */
        if(searchByKeyInList(values, value) == NULL)
            result = appendNode(values, value);
        return result;
    }
}
List lookupInMultiValueHashTable(multiHash multi, Element key)
{
    if(multi == NULL || key == NULL)
        return NULL;
    /* search the values in hashtable */
    List values = lookupInHashTable(multi->hashtable, key);
    if(values == NULL)
        return NULL;
    return values; // list of values
}
status removeFromMultiValueHashTable(multiHash multi, Element key, Element val)
{
    if(multi == NULL || key == NULL || val == NULL)
        return failure;
    status result;
    List values = lookupInMultiValueHashTable(multi, key);
    if(values == NULL)
        return failure;
    /* if there is at list 1 value */
    result = deleteNode(values, val);
    /* if there is no values in list we should delete the key from multihash */
    if(getLengthList(values) == 0)
    {
        result = removeFromHashTable(multi->hashtable,key);
        return result;
    }
    return result;
}
status displayMultiValueHashElementsByKey(multiHash multi, Element key)
{
    /* lookupinhashtable will check the pointers */
    List values = lookupInMultiValueHashTable(multi, key);
    if(values == NULL)
        return failure;
    multi->printkey(key);
    displayList(values);
    return success;
}
