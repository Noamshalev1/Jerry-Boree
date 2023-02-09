/* Jerry.h - Jerry module header file
 * This module defines an objects type and implements some manipulation functions. */
#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"

/* this struct describe a planet with name and an array of coordinates. */
typedef struct Planet_t
{
    char *name; // uniq
    float crd[3]; // coordinates array for x, y, z

}Planet; 

/* this struct describe an origin include a pointer to planet and a name. */
typedef struct Origin_t
{
    Planet *planet;
    char *name;
}Origin;

/* this struct describe a physical characteristic include a name and a value. */
typedef struct PhysicalCharacteristics
{
    char *name; // uniq
    float value; 
}Phc;

/* this struct describe a jerry include a pointer to his origin, id, mood,
 * an array of physical characteristics, and it's size. */
typedef struct Jerry_t
{
    Origin *org;
    char *id; // uniq
    int mood; // happiness - 0 to 100 only
    Phc **pPhc; // array of pointers to all phc.
    int numPhc; // number of characteristics

}Jerry;

/* All these functions are create an object in Jerry's module. More details will be provided at Jerry.c */
Jerry* createJerry( char*, int, Planet*, char* );
Planet* createPlanet( char *, float, float, float );
Phc* createPhc( char *, float );
Origin* createOrigin( Planet*, char * );
/* A function that check if a physical characteristic is existed in jerry
 * return true if its existed, and false if it doesn't */
bool isExistPhc( Jerry*, char * );
/* These functions responsible to add or delete a physical characteristic from jerry
 * return success if it worked, and failre if something goes wrong */
status addPhc( Jerry*, char*, float );
status deletePhc( Jerry*, char * );
/* Print functions */
status printJerry( Jerry* ); // print all info about jerry.
status printPlanet( Planet* ); // print all info about planet
/* Destroy functions
 * These functions used to clear the hole memory */
void freeJerry( Jerry* );
void freePlanet( Planet* );
void freePhc( Phc* );
void freeOrg( Origin* );
/* get the jerrie's id */
char* getId(Jerry *);
float getValofphc(Jerry *, char*);

#endif
