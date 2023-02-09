#include "LinkedList.h"

/* this struct is a node in the list */
typedef struct Node_t
{
    Element data;
    struct Node_t *next;
}Node;

/* hold 2 pointers to head and tail, the functions from user, and size */
struct LinkedList_t
{
    CopyFunction copyElement;
    FreeFunction freeElement;
    PrintFunction printNode;
    EqualFunction isEqualKey;
    Node* head;
    Node* tail;
    int size;
};

List createLinkedList(CopyFunction copyfunction, FreeFunction freefunction, PrintFunction printfunction, EqualFunction isEqualKey)
{
    /* check the pointers */
    if(copyfunction == NULL || freefunction == NULL || printfunction == NULL || isEqualKey == NULL)
        return NULL;
    /* init new list */
    List list = NULL;
    list = (List)malloc(sizeof(struct LinkedList_t));
    if(list == NULL)
        return NULL;
    list->copyElement = copyfunction;
    list->freeElement = freefunction;
    list->printNode = printfunction;
    list->isEqualKey = isEqualKey;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;

}
/* destroy list */
status destroyList(List list)
{
    /* nothing to do */
    if(list == NULL)
        return failure;
    /* list is empty */
    if(list->size == 0)
    {
        list->head = NULL;
        list->tail = NULL;
        free(list);
        return success;
    }
    /* starts from head */
    Node* current = list->head;
    Node* next = list->head->next; // the next after
    /* run on list */
    for(int i=0; i<list->size; i++)
    {
        /* delete from head until the tail */
        list->freeElement(current->data);
        free(current);
        /* changing pointers */
        current = next;
        if(current == NULL)
            break;
        next = next->next;

    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    free(list);
    return success;
}
/* add a new element to list */
status appendNode(List list, Element elem)
{
    /* check the pointers */
    if(list == NULL || elem == NULL)
        return failure;
    /* malloc for a new node in list */
    Node* newNode = NULL;
    newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL)
        return failure;
    if(list->size == 0)
    {
        /* use copy function to insert element to list */
        newNode->data = list->copyElement(elem);
        if(newNode->data == NULL)
        {
            free(newNode);
            return failure;
        }
        newNode->next = NULL;
        list->head = newNode;
        /* pointers to head and tail */
        list->tail = list->head;
    }
    else
    {
        /* this case if it's not the first element */
        newNode->data = list->copyElement(elem);
        if(newNode->data == NULL)
        {
            free(newNode);
            return failure;
        }
        newNode->next = NULL;
        /* move tail to the last element */
        list->tail->next = newNode;
        list->tail = newNode;

    }
    /* increase the size */
    list->size++;
    return success;
}
/* delete an element from list */
status deleteNode(List list, Element elem)
{
    if(list == NULL || elem == NULL)
        return failure;
    if(list->size == 0)
        return failure;
    /* check if the element we want to delete is the head of list */
    Node* ptr = list->head;
    if(list->isEqualKey(ptr->data, elem) == true)
    {
        list->head = ptr->next; // if it's the last element its null
        list->freeElement(ptr->data);
        free(ptr);
        list->size--;
        if(list->size == 1)
            list->tail = list->head; // if it's the last element than head = tail
        return success;
    }
    Node* current = ptr; // saving second node in list
    /* we check the head already so start from 1 */
    ptr = ptr->next;
    int numofelem = list->size;
    for(int i=1; i<numofelem; i++)
    {
        /* check if we arrived the tail */
        if(list->isEqualKey(ptr->data, list->tail->data) == true)
        {
            /* need to delete the tail */
            if(list->isEqualKey(ptr->data, elem) == true)
            {
                list->freeElement(ptr->data);
                free(ptr);
                current->next = NULL;
                list->tail = current;
                list->size--;
                return success;
            }
        }
        /* the element is in the middle */
        if(list->isEqualKey(ptr->data, elem) == true)
        {
            current->next = ptr->next;
            if(current->next == NULL)
                list->tail = current;
            list->freeElement(ptr->data);
            free(ptr);
            list->size --;
            return success;
        }
        current = ptr;
        ptr = ptr->next;
    }
    return failure;

}
/* print all list */
void displayList(List list)
{
    Node* current = NULL;
    current = list->head;
    for(int i=0; i<list->size; i++)
    {
        list->printNode(current->data);
        current = current->next;
    }
}
/* return data in a specific index */
Element getDataByIndex(List list, int index)
{
    if(list == NULL)
        return NULL;
    Node* current = list->head;
    /* run on list and search the correct data */
    for(int i=1; i<=list->size; i++)
    {
        if(i == index)
            return current->data;
        current = current->next;
    }
    return NULL;
}
/* return size of list */
int getLengthList(List list)
{
    return list->size;
}
/* check by the isEqualKey function if some info is in some node, and return the node */
Element searchByKeyInList(List list, Element elem) {
    Node *ptr = list->head;
    while (ptr != NULL)
    {
        if (list->isEqualKey(ptr->data, elem) == true)
            return ptr->data;
        ptr = ptr->next;
    }
    return NULL;
}
