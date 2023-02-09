/* This file contains all the functions jerry need. Here we create all the structs and connect between them.
 * Also, we got here print functions and free function for every struct.*/
#include "Jerry.h"
/* function which create a new jerry
 * arguments: Jerrie's id, mood, planet and an origin name
 * return: a pointer to a new jerry */
Jerry* createJerry( char *id, int mood, Planet *p, char *org )
{
    /* pre-check if all the values are not null */
    if(id == NULL || p == NULL || org == NULL )
    {
        return NULL;
    }
    /* malloc for the returned value - a pointer to struct Jerry */
    Jerry *j = (Jerry *)malloc(sizeof(Jerry));
    /* Check if malloc do a good job */
    if(j == NULL)
    {
        return NULL;
    }
    /* malloc for the id */
    j->id = (char *)malloc((strlen(id)+1)*sizeof(char));
    /* Check if malloc do a good job */
    if(j->id == NULL)
    {
        free(j);
        return NULL;
    }
    /* copy the information from the pointer we got to the new one */
    strcpy(j->id, id);
    /* Use another function to create the Jerrie's origin - insert the planet and the origin name */
    j->org = createOrigin(p, org);
    /* Check if malloc do a good job */
    if(j->org == NULL)
    {
        free(j->id);
        free(j);
        return NULL;
    }
    /* insert the mood we got (an integer between 0 and 100) */
    j->mood = mood;
    /* initialize the physical-characteristics number to 0 - we don't have any of them yet. */
    j->numPhc=0;
    /* Return a pointer to the new Jerry*/
    return j;
}

/* function which create a new planet
 * arguments: Planet's name, coordinates - x,y,z (floats)
 * return: a pointer to Planet */
Planet* createPlanet( char *name, float x, float y, float z)
{
    /* The name is the only one which can be null because it's a pointer to char */
    if(name == NULL)
        return NULL;
    /* malloc for the returned value - a pointer to Planet */
    Planet* planet = (Planet *)malloc(sizeof(Planet));
    /* Check if malloc do a good job */
    if(planet == NULL)
    {
        return NULL;
    }
    /* malloc for the Planet's name */
    planet->name = (char *)malloc((strlen(name)+1)*sizeof(char));
    /* Check if malloc do a good job */
    if(planet->name == NULL)
    {
        free(planet);
        return NULL;
    }
    /* copy the name we got to the planet's name */
    strcpy(planet->name,name);
    /* insert the coordinates to their places in the array */
    planet->crd[0] = x;
    planet->crd[1] = y;
    planet->crd[2] = z;
    /* Return a pointer to a new Planet */
    return planet;

}

/* function which create a new physical character
 * arguments: physical-characteristic's name and value (float)
 * return: a pointer to physical-characteristic */
Phc* createPhc( char *name, float value )
{
    /* The name is the only one which can be null because it's a pointer to char */
    if(name == NULL)
        return NULL;
    Phc* phc = (Phc *)malloc(sizeof(Phc));
    /* Check if malloc do a good job */
    if(phc == NULL)
    {
        return NULL;
    }
    phc->name = (char *)malloc((strlen(name)+1)*sizeof(char));
    /* Check if malloc do a good job */
    if(phc->name == NULL)
    {
        free(phc);
        return NULL;
    }
    /* copy the name we got to the physical-characteristic's name */
    strcpy(phc->name,name);
    /* insert the value */
    phc->value = value;
    /* Return a pointer to a new physical-characteristic */
    return phc;
}

/* function which create a new origin
 * arguments: pointer to a planet and an origin's name
 * return: a pointer to origin */
Origin* createOrigin( Planet *planet, char *name )
{
    /* pre-check if all the values are not null */
    if(name == NULL || planet == NULL)
        return NULL;
    /* malloc for the returned value - a pointer to origin */
    Origin *org = (Origin *)malloc(sizeof(Origin));
    /* Check if malloc do a good job */
    if(org == NULL)
    {
        return NULL;
    }
    org->planet = planet;
    /* malloc for the origin's name */
    org->name = (char *)malloc((strlen(name)+1)*sizeof(char));
    /* Check if malloc do a good job */
    if(org->name == NULL)
    {
        free(org);
        return NULL;
    }
    /* copy the name we got to the origin's name */
    strcpy(org->name, name);
    /* Return a pointer to a new origin */
    return org;
}

/* function which check if the physical character is in this jerry list
 * arguments: a jerry and a physical-characteristic's name
 * return: true - if the physical-characteristic belongs to jerry and false if not.  */
bool isExistPhc( Jerry *j, char *name )
{
    /* pre-check if all the values are not null */
    if(j == NULL || name == NULL)
        return false;
    /* len will use for the physical-characteristics array */
    int len = j->numPhc;
    for(int i=0; i<len; i++)
    {
        /* compare between the names */
        if(strcmp(name, j->pPhc[i]->name) == 0)
            return true; // found
    }
    /* Return false if not found */
    return false;
}

/* function which add a new physical character to jerry
 * arguments: a jerry, name and value for a new physical-characteristic
 * return: status - success if we successfully insert the new physical-characteristic, and failure else. */
status addPhc( Jerry *j, char *name, float value) {
    /* At first, we check if the jerry already has this physical-characteristic */
    bool result = isExistPhc(j, name);
    /* means nothing to add to jerry */
    if (result == true)
        return failure;
    /* check if it's the first one */
    if (j->numPhc == 0)
    {
        j->pPhc = (Phc **) malloc(sizeof(Phc *));
        if(j->pPhc == NULL)
            return failure;
        j->pPhc[0] = createPhc(name, value);
        if (j->pPhc[0] == NULL)
        {
            free(j->pPhc);
            return failure;
        }
        j->numPhc++;
        return success;
    }
    j->pPhc = (Phc **) realloc(j->pPhc, sizeof(Phc*)*(j->numPhc+1));
    if (j->pPhc == NULL)
        return failure;
    j->pPhc[j->numPhc] = createPhc(name, value);
    if (j->pPhc[j->numPhc] == NULL)
    {
        /* in case of problem make the array smaller again */
        j->pPhc = (Phc **) realloc(j->pPhc, sizeof(Phc*)*(j->numPhc));
        return failure;
    }
    /* increase the number of physical-characteristics if all goes well */
    j->numPhc++;
    return success;
}

/* function which check if the character exist, and delete it
 * arguments: a jerry and a physical-characteristics name
 * return: status - success if we delete the physical-characteristic from jerry, else failure */
status deletePhc( Jerry *j, char *name)
{
    if(j->numPhc == 0)
        return failure;
    // a pointer to the physical-characteristic we want to delete
    int index;
   /* check if the physical-characteristic exist in jerry */
   if(isExistPhc(j,name) == false)
       return failure;
   /* now find the requested physical-characteristic
    * if there is only one, delete it (from the first cell) */
   if(j->numPhc == 1)
   {
        freePhc(j->pPhc[0]);
        free(j->pPhc);
        j->numPhc = 0;
        return success;
   }
    /* loop find the physical-characteristic */
   for(int i=0; i<j->numPhc; i++)
   {
       if(strcmp(j->pPhc[i]->name, name) == 0)
       {
           /* delete the physical-characteristic */
           freePhc(j->pPhc[i]);
           index = i;
           break;
       }
   }
   /* if the physical-characteristic is in the last cell */
   if(index == j->numPhc -1){
       j->pPhc = (Phc **)realloc(j->pPhc, sizeof(Phc*)*(j->numPhc-1));
       if(j->pPhc == NULL)
           return failure;
       j->numPhc--;
       return success;
   }
   /* forward every physical-characteristic to the left cell, start with the one we want to delete */
   for(int i=index; i<j->numPhc-1; i++){
       j->pPhc[i] = j->pPhc[i+1];
   }
   /* Now we are ready to make the array smaller */
   j->pPhc = (Phc **)realloc(j->pPhc, sizeof(Phc*)*(j->numPhc-1));
   if(j->pPhc == NULL)
       return failure;
   /* decrease the number of physical-characteristics in 1 */
   j->numPhc--;
   return success;
}

/* function which print a Jerry
 * argument: a pointer to jerry
 * return: success if we print the info about it, else failre. */
status printJerry(Jerry *j)
{
    /* pre-check if the value is not null */
    if(j == NULL)
        return failure;
    printf("Jerry , ID - %s : \nHappiness level : %d \nOrigin : %s \n", j->id, j->mood, j->org->name);
    /* similar function - just for planets */
    printPlanet(j->org->planet);
    /* if jerry has any physical-characteristic */
    if(j->numPhc != 0)
    {
        printf("Jerry's physical Characteristics available : \n\t");
        for(int i=0; i<j->numPhc; i++)
        {
            if(i == j->numPhc - 1)
            {
                printf("%s : %.2f \n", j->pPhc[i]->name, j->pPhc[i]->value);
                break;
            }
            /* this print line for the last physical-characteristic */
            printf("%s : %.2f , ", j->pPhc[i]->name, j->pPhc[i]->value);
        }
    }
    return success;
}

/* function which print a Planet
 * argument: a pointer to planet
 * return: success if we print the info about it, else failre. */
status printPlanet(Planet *p)
{
    /* pre-check if the value is not null */
    if(p == NULL)
        return failure;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", p->name, p->crd[0], p->crd[1], p->crd[2]);
    return success;
}

/* these all functions used for clean the memory
 * the description of them: start from the inside and at the end free the hole struct */
/* free jerry */
void freeJerry(Jerry *j)
{
    /* there is nothing to free */
    if(j == NULL)
        return;
    /* function used to free an origin */
    freeOrg(j->org);
    if(j->id != NULL)
        free(j->id);
    /* free all the physical characteristics */
    for(int i = 0; i < j->numPhc; i++)
        freePhc(j->pPhc[i]);
    if(j->numPhc > 0)
        free(j->pPhc);
    free(j);

}
/* free planet */
void freePlanet(Planet *p )
{
    /* there is nothing to free */
    if(p == NULL)
        return;
    if(p->name != NULL)
        free(p->name);
    free(p);
}
/* free physical characteristic */
void freePhc(Phc *phc )
{
    /* there is nothing to free */
    if(phc == NULL)
        return;
    if(phc->name != NULL)
        free(phc->name);
    free(phc);
}
/* free origin */
void freeOrg(Origin *org )
{
    if(org == NULL)
        return;
    if(org->name != NULL)
        free(org->name);
    if(org->planet != NULL)
        org->planet = NULL;
    if(org != NULL)
        free(org);

}

/* get the jerrie's id */
char* getId(Jerry* j)
{
    return j->id;
}

float getValofphc(Jerry * j, char* namePhc)
{
    if(j == NULL || namePhc == NULL)
        return -1;
    for(int i=0; i<j->numPhc; i++)
    {
        if(strcmp(j->pPhc[i]->name , namePhc) == 0)
            return j->pPhc[i]->value;
    }
    return -1;
}
