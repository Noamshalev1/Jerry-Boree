#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s
{
    Element *array;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
    int hashNumber;

};

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber)
{
    if(copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || transformIntoNumber == NULL || hashNumber <= 0)
        return NULL;
    /* create new hash table*/
    hashTable hashtable = (hashTable)malloc(sizeof(struct hashTable_s));
    if(hashtable == NULL)
        return NULL;
    /* init the array */
    hashtable->array = (Element*)malloc(sizeof(Element)*hashNumber);
    if(hashtable->array == NULL)
    {
        free(hashtable);
        return NULL;
    }
    /* init the cells in array to know if the cell is available */
    for(int i=0; i<hashNumber; i++)
        hashtable->array[i] = NULL;
    hashtable->hashNumber = hashNumber;
    hashtable->transformIntoNumber = transformIntoNumber;
    hashtable->equalKey = equalKey;
    hashtable->printValue = printValue;
    hashtable->freeValue = freeValue;
    hashtable->copyValue = copyValue;
    hashtable->printKey = printKey;
    hashtable->freeKey = freeKey;
    hashtable->copyKey = copyKey;
    return hashtable;

}
/* function return an index to array */
int hashfunction(hashTable t, Element x)
{
    int index = t->transformIntoNumber(x) % t->hashNumber;
    return index;
}
/* copy function - sent to linkedlist ADT */
Element copyPair(Element pair)
{
    /* send a pointer to pair to avoid duplications */
    if(pair == NULL)
        return NULL;
    return (Pair)pair;
}
/* print function - sent to linkedlist ADT */
status printPair(Element pair)
{
    if(pair == NULL)
        return failure;
    Pair newPair = (Pair)pair;
    /* these functions print the key and then the value */
    displayKey(newPair);
    displayValue(newPair);
    return success;

}
status destroyHashTable(hashTable t)
{
    if(t == NULL)
        return failure;
    /* run on array of hashtable */
    for(int i=0; i<t->hashNumber; i++)
    {
        /* if there is a list in the cell than free it */
        if(t->array[i] != NULL)
            destroyList(t->array[i]);
    }
    free(t->array);
    free(t);
    return success;

}
/* add a new key,value to hashtable */
status addToHashTable(hashTable t, Element key,Element value)
{
    if(t == NULL || key == NULL || value == NULL)
        return failure;
    /* get an index from hash function */
    int index = hashfunction(t,key);
    Pair newPair = NULL;
    /* create a new pair */
    newPair = createKeyValuePair(key, value, t->copyKey, t->freeKey, t->printKey, t->copyValue, t->freeValue, t->printValue, t->equalKey);
    if(newPair == NULL)
        return failure;
    /* check if the cell is available, if yes - create a new LinkedList */
    if(t->array[index] == NULL)
        t->array[index] = createLinkedList(copyPair, (FreeFunction) destroyKeyValuePair, printPair,(EqualFunction) isEqualKey);
    /* after malloc */
    if(t->array[index] == NULL)
    {
        destroyKeyValuePair(newPair);
        return failure;
    }
    /* insert the new Pair to list */
    status result = appendNode(t->array[index], newPair);
    return result;
}
/* search for some value in the hash by the key */
Element lookupInHashTable(hashTable t, Element key)
{
    if(t == NULL || key == NULL)
        return NULL;
    /* get an index from hash function */
    int index = hashfunction(t, key);
    if(t->array[index] == NULL)
        return NULL;
    /* search the pair by key */
    Pair node = searchByKeyInList(t->array[index], key);
    if(node == NULL)
        return NULL;
    Element result = getValue(node);
    return result;

}
/* delete an element from hash (key, value) */
status removeFromHashTable(hashTable t, Element key)
{
    if(t == NULL || key == NULL)
        return failure;
    /* get an index from hash function */
    int index = hashfunction(t, key);
    if(t->array[index] == NULL)
        return failure;
    /* search the node by the key to insure is exist */
    Pair result = lookupInHashTable(t, key);
    if(result == NULL)
        return failure;
    /* delete the node from list in cell[index] by the key */
    status deletion = deleteNode(t->array[index], key);
    if(getLengthList(t->array[index]) == 0)
    {
        free(t->array[index]);
        t->array[index] = NULL; // null is to know there is an empty cell for a new list
    }
    return deletion;

}
status displayHashElements(hashTable t)
{
    if(t == NULL)
        return failure;
    /* scan the hashtable and for any not null cell - print the list inside */
    for(int i=0; i<t->hashNumber; i++)
    {
        if(t->array[i] != NULL)
            displayList(t->array[i]);
    }
    return success;

}
