#include "Jerry.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include <math.h>

int numberOfPlanets = 0;
Planet** planetsArray = NULL;
hashTable jerriesHashtable = NULL;
multiHash physicalCharMultihash = NULL;
List jerries = NULL;
void memoryclear();
status deleteJerryFromdaycare(Jerry* );
int searchPrimeNumber(int);
bool isPrime(int);

/* functions for jerriesHashtable
 * the idea is to make the id of jerry into key and the hole pointer to jerry into value */
// ---- for the key (Jerry's id)
Element copyJerryid(Element);
status freeJerryid(Element);
status printJerryid(Element);
bool isEquakeyid(Element, Element);
int transformIntoNumber(Element);
int hashnum = 0;
int multihashnum = 0;
// ---- for the value (Jerry)
Element copyJerry(Element);
// free and print took from Jerry.h

/* functions for physicalCharMultihash
 * the idea is to make the name of the physicalChar into key, and the jerries pointers connect to it into value */
// ---- for the key (physicalChar's name)
Element copyphysicalCharname(Element);
status freePhcname(Element);
status printPhcname(Element);
bool isEqualphcname(Element, Element);
// ---- for values list
Element copyval(Element);
status freeval(Element);
status printval(Element);
status freeJerryptr(Element);
bool isEqualjerryid(Element, Element);

int main(int argc, char *argv[] )
{

    /* take num planets from file */
    numberOfPlanets = atoi(argv[1]);
    planetsArray = (Planet **) malloc(numberOfPlanets * sizeof(Planet *)); // array for pointers to planets
    if (planetsArray == NULL)
    {
        numberOfPlanets = 0;
        memoryclear();
        printf("Memory Problem");
        exit(1);
    }
    /* function that collect the file */
    FILE *configuration_file = fopen(argv[2], "r");
    /* a buffer for all the inputs chars. */
    char buf[300];
    fscanf(configuration_file, "%*[^\n]\n"); // skip first line
    fgets(buf, sizeof(buf), configuration_file); // take the first line
    /* This part is to collect all planets */
    char *input; // collect the first element from a line
    float x, y, z;
    for (int i = 0; i < numberOfPlanets; i++) // a loop that takes all planets
    {
        input = strtok(buf, ","); // planet's name
        /* this function change from string to float (used for the coordinates */
        x = atof(strtok(NULL, ","));
        y = atof(strtok(NULL, ","));
        z = atof(strtok(NULL, ","));
        /* create a new planet */
        planetsArray[i] = createPlanet(input, x, y, z);
        /* Check if malloc did a good job */
        if (planetsArray[i] == NULL)
        {
            /* if something goes wrong */
            memoryclear();
            fclose(configuration_file);
            printf("Memory Problem");
            exit(1);
        }
        fgets(buf, sizeof(buf), configuration_file); // next line -- at the end contains "Jerries"
    }
    /* This part is to collect all jerries */
    //fgets(buf, sizeof(buf),configuration_file); // ignore the line contains "Jerries"
    /* all these vales are for help */
    char *org, *planet;
    float val;
    int mood;
    Planet *ptr = NULL;
    Jerry *newJerry = NULL;
    status result = success;
    int numofPhc = 0;
    /* new linked list to collect jerries */
    jerries = createLinkedList(copyJerry, (FreeFunction) freeJerry, (PrintFunction) printJerry, isEqualjerryid);
    if (jerries == NULL)
    {
        /* if something goes wrong */
        memoryclear();
        fclose(configuration_file);
        printf("Memory Problem");
        exit(1);
    }
    fgets(buf, sizeof(buf), configuration_file); // next line
    //int c = getc(configuration_file); // check if done
    /* run on file and collects jerry until it's done */
    while (!feof(configuration_file))
    {
        input = strtok(buf, ","); // Jerry's id
        org = strtok(NULL, ","); // Jerry's name of origin
        planet = strtok(NULL, ","); // jerry's planet
        mood = atoi(strtok(NULL, ",")); // Jerry's mood
        for (int j = 0; j < numberOfPlanets; j++) // find the planet
        {
            if (strcmp(planet, planetsArray[j]->name) == 0)
            {
                ptr = planetsArray[j];
                break;
            }
        }
        /* create a new jerry */
        newJerry = createJerry(input, mood, ptr, org);
        if (newJerry == NULL)
        {
            /* if something goes wrong */
            memoryclear();
            fclose(configuration_file);
            printf("Memory Problem");
            exit(1);
        }
        fgets(buf, sizeof(buf), configuration_file); // next line
        while (buf[0] == '\t') // in case there are physical-characteristic
        {
            memmove(buf, buf + 1, strlen(buf)); // this function ignore the '\t'
            input = strtok(buf, ":"); // name of physical-characteristic
            val = atof(strtok(NULL, ":")); // value
            /* create and add a new physical-characteristic
            /* in addPhc there is a check for the pointer */
            result = addPhc(newJerry, input, val);
            /* Check if malloc did a good job and the characteristic added */
            if (result == failure)
            {
                /* if something goes wrong */
                memoryclear();
                fclose(configuration_file);
                printf("Memory Problem");
                exit(1);
            }
            fgets(buf, sizeof(buf), configuration_file); // next line
        }
        /* add the new jerry to list */
        result = appendNode(jerries, newJerry);
        /* Check if malloc did a good job */
        if (result == failure)
        {
            /* if something goes wrong */
            memoryclear();
            printf("Memory Problem");
            exit(1);
        }

    } // end while loop
    /* create hashtable and multihash - the size decided by the number of jerries. Look for the closest prime one */
    if(isPrime(getLengthList(jerries)))
        hashnum = getLengthList(jerries);
    else
        hashnum = searchPrimeNumber(getLengthList(jerries));
    jerriesHashtable = createHashTable(copyJerryid, freeJerryid, printJerryid, copyJerry, freeJerryptr,
                                       (PrintFunction) printJerry, isEquakeyid, transformIntoNumber, hashnum);
    /* Check if malloc did a good job */
    if (jerriesHashtable == NULL)
    {
        /* if something goes wrong */
        memoryclear();
        fclose(configuration_file);
        printf("Memory Problem");
        exit(1);
    }
    /* forward on list, collect jerries to hashtable and physical chars to multi hash */
    Jerry *tmp = NULL;
    char *namePhc = NULL;
    for (int i = 1; i <= getLengthList(jerries); i++)
    {
        tmp = getDataByIndex(jerries, i);
        if (tmp == NULL)
        {
            /* if something goes wrong */
            memoryclear();
            fclose(configuration_file);
            printf("Memory Problem");
            exit(1);
        }
        /* count the number of physical characteristic for every jerry */
        numofPhc += tmp->numPhc;
        /* jerry's id = key, jerry = value */
        result = addToHashTable(jerriesHashtable, tmp->id, tmp);
        if (result == failure)
        {
            /* if something goes wrong */
            memoryclear();
            fclose(configuration_file);
            printf("Memory Problem");
            exit(1);
        }
    }
    /* now we determine the size of multi hash */
    if (numofPhc > 0)
    {
        if(isPrime(hashnum + numofPhc))
            multihashnum = hashnum + numofPhc;
        else
            multihashnum = searchPrimeNumber(hashnum + numofPhc);

    }
    else
        multihashnum = hashnum;
    /* create a new multihash to save physical-characters of jerries */
    physicalCharMultihash = createMultiValueHashTable(copyphysicalCharname, freePhcname, printPhcname, copyval,
                                                      freeval, printval, isEqualphcname, transformIntoNumber,
                                                      multihashnum);
    for (int i = 1; i <= getLengthList(jerries); i++)
    {
        /* get a pointer to jerry */
        tmp = getDataByIndex(jerries, i);
        for (int j = 0; j < tmp->numPhc; j++)
        {
            /* add jerry to multi hash by his physical-characteristics */
            result = addToMultiValueHashTable(physicalCharMultihash, tmp->pPhc[j]->name, tmp);
            if (result == failure)
            {
                /* if something goes wrong */
                memoryclear();
                fclose(configuration_file);
                printf("Memory Problem");
                exit(1);
            }
        }

    }


    /* MAIN PROGRAM */
    bool check = false;
    char ch[300], ch_7[300], test;
    char jerry_id[300], planet_name[300], origin[300], physicalCharname[300];
    float value, tmpVal;
    int happiness_level, index, max_sad;
    int count;
    double min_dist = 0;
    Jerry *jerry = NULL;
    Jerry *tmpJerry = NULL;
    List values = NULL;
    status returned_val;
    while (true) {
        check = false;
        returned_val = failure;
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        scanf(" %s", ch);
        /* check if the input length larger than 1 */
        if(strlen(ch) > 1)
            strcpy(ch, "a");
        /* if true, the input is wrong */
        test = ch[0];
        switch (test) {
            case '1': /* Take this Jerry away from me */
                printf("What is your Jerry's ID ? \n");
                scanf("%s", jerry_id);
                /* search jerry in hashtable by id */
                jerry = lookupInHashTable(jerriesHashtable, jerry_id);
                if (jerry != NULL) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    continue;
                }
                /* take his planet name */
                printf("What planet is your Jerry from ? \n");
                scanf("%s", planet_name);
                for (int i = 0; i < numberOfPlanets; i++) {
                    if (strcmp(planetsArray[i]->name, planet_name) == 0) {
                        check = true;
                        index = i;
                        break;
                    }
                }
                if (!check) {
                    printf("%s is not a known planet ! \n", planet_name);
                    continue;
                }
                /* take origin and happiness level */
                printf("What is your Jerry's dimension ? \n");
                scanf("%s", origin);
                printf("How happy is your Jerry now ? \n");
                scanf("%d", &happiness_level);
                jerry = createJerry(jerry_id, happiness_level, planetsArray[index], origin);
                if (jerry == NULL) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                /* first, add new jerry to list */
                returned_val = appendNode(jerries, jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                /* Then, add him to hashtable */
                returned_val = addToHashTable(jerriesHashtable, jerry_id, jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                printJerry(jerry);
                break;
            case '2': /* I think I remember something about my Jerry */
                printf("What is your Jerry's ID ? \n");
                scanf("%s", jerry_id);
                /* search jerry in hashtable by id */
                jerry = lookupInHashTable(jerriesHashtable, jerry_id);
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    continue;
                }
                printf("What physical characteristic can you add to Jerry - %s ? \n", jerry_id);
                scanf("%s", physicalCharname);
                for (int i = 0; i < jerry->numPhc; i++) {
                    if (strcmp(physicalCharname, jerry->pPhc[i]->name) == 0) {
                        printf("The information about his LimbsNumber already available to the daycare ! \n");
                        check = true;
                        break;
                    }
                }
                if (check)
                    continue;
                printf("What is the value of his %s ? \n", physicalCharname);
                scanf("%f", &value);
                /* add a new physical characteristic to jerry */
                returned_val = addPhc(jerry, physicalCharname, value);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                /* Then, add it's physical characteristic to multihashtable */
                returned_val = addToMultiValueHashTable(physicalCharMultihash, physicalCharname, jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                displayMultiValueHashElementsByKey(physicalCharMultihash, physicalCharname);
                break;
            case '3': /* Oh wait. That can't be right */
                printf("What is your Jerry's ID ? \n");
                scanf("%s", jerry_id);
                /* search jerry in hashtable by id */
                jerry = lookupInHashTable(jerriesHashtable, jerry_id);
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    continue;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", jerry_id);
                scanf("%s", physicalCharname);
                for (int i = 0; i < jerry->numPhc; i++) {
                    if (strcmp(physicalCharname, jerry->pPhc[i]->name) == 0) {
                        check = true;
                        index = i;
                        break;
                    }
                }
                if (!check) {
                    printf("The information about his %s not available to the daycare ! \n", physicalCharname);
                    continue;
                }
                /* remove the physical characteristic from jerry */
                returned_val = deletePhc(jerry, physicalCharname);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                /* update multihash */
                returned_val = removeFromMultiValueHashTable(physicalCharMultihash, physicalCharname, jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                printJerry(jerry);
                break;
            case '4': /* I guess I will take back my Jerry now */
                printf("What is your Jerry's ID ? \n");
                scanf("%s", jerry_id);
                /* search jerry in hashtable by id */
                jerry = lookupInHashTable(jerriesHashtable, jerry_id);
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    continue;
                }
                /* delete jerry from everywhere */
                returned_val = deleteJerryFromdaycare(jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;
            case '5': /* I can't find my Jerry. Just give me a similar one */
                printf("What do you remember about your Jerry ? \n");
                scanf("%s", physicalCharname);
                /* search jerries in multi hashtable by physical characteristic */
                values = lookupInMultiValueHashTable(physicalCharMultihash, physicalCharname);
                if (values == NULL) {
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n", physicalCharname);
                    continue;
                }
                printf("What do you remember about the value of his %s ? \n", physicalCharname);
                scanf("%f", &value);
                /* run on list */
                tmpJerry = getDataByIndex(values, 1); // head of list
                tmpVal = getValofphc(tmpJerry, physicalCharname); // O(total number of his physical characteristics)
                min_dist = tmpVal;
                jerry = tmpJerry;
                for (int i = 2; i <= getLengthList(values); i++) {
                    tmpJerry = getDataByIndex(values, i); // O(number of jerries with physical characteristic)
                    tmpVal = getValofphc(tmpJerry, physicalCharname); // O(total number of their physical characteristics)
                    /* fabs return the abs of float */
                    if (fabs(value - tmpVal) < min_dist) {
                        min_dist = fabs(value - tmpVal);
                        jerry = tmpJerry;
                    }
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerry);
                /* delete the jerry from daycare */
                returned_val = deleteJerryFromdaycare(jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;
            case '6': /* I lost a bet. Give me your saddest Jerry */
                if (getLengthList(jerries) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    continue;
                }
                /* take the first jerry from list */
                tmpJerry = getDataByIndex(jerries, 1);
                max_sad = tmpJerry->mood;
                jerry = tmpJerry;
                /* run on rest of list */
                for (int i = 2; i <= getLengthList(jerries); i++) {
                    tmpJerry = getDataByIndex(jerries, i);
                    if (tmpJerry->mood < max_sad) {
                        max_sad = tmpJerry->mood;
                        jerry = tmpJerry;
                    }
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerry);
                /* delete the jerry from daycare */
                returned_val = deleteJerryFromdaycare(jerry);
                if (returned_val == failure) {
                    /* if something goes wrong */
                    memoryclear();
                    fclose(configuration_file);
                    printf("Memory Problem");
                    exit(1);
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                continue;
            case '7': /* Show me what you got */
                printf("What information do you want to know ? \n");
                printf("1 : All Jerries \n");
                printf("2 : All Jerries by physical characteristics \n");
                printf("3 : All known planets \n");
                scanf(" %s", ch_7);
                /* check if the input length larger than 1 */
                if(strlen(ch_7) > 1)
                    strcpy(ch_7, "a");
                /* if true, the input is wrong */
                test = ch_7[0];
                switch (test) {
                    case '1':
                        if (getLengthList(jerries) == 0) {
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            continue;
                        }
                        displayList(jerries);
                        continue;
                    case '2':
                        printf("What physical characteristics ? \n");
                        scanf("%s", physicalCharname);
                        values = lookupInMultiValueHashTable(physicalCharMultihash, physicalCharname);
                        if (values == NULL) {
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", physicalCharname);
                            continue;
                        }
                        displayMultiValueHashElementsByKey(physicalCharMultihash, physicalCharname);
                        continue;
                    case '3':
                        for (int i = 0; i < numberOfPlanets; i++)
                            printPlanet(planetsArray[i]);
                        continue;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        continue;
                }
            case '8':
                if (getLengthList(jerries) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    continue;
                }
                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                scanf(" %s", ch_7);
                /* check if the input length larger than 1 */
                if(strlen(ch_7) > 1)
                    strcpy(ch_7, "a");
                /* if true, the input is wrong */
                test = ch_7[0];
                switch (test) {
                    case '1': /* Interact with fake Beth */
                        for (int i = 1; i <= getLengthList(jerries); i++) {
                            tmpJerry = getDataByIndex(jerries, i);
                            if (20 <= tmpJerry->mood && tmpJerry->mood <= 85){
                                tmpJerry->mood += 15;
                                continue;
                            }
                            if (tmpJerry->mood > 85)
                            {
                                tmpJerry->mood = 100;
                                continue;
                            }
                            if (5 <= tmpJerry->mood && tmpJerry->mood < 20)
                            {
                                tmpJerry->mood -= 5;
                                continue;
                            }
                            else
                                tmpJerry->mood = 0;
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        continue;
                    case '2': /* Play golf */
                        for (int i = 1; i <= getLengthList(jerries); i++) {
                            tmpJerry = getDataByIndex(jerries, i);
                            if (50 <= tmpJerry->mood && tmpJerry->mood <= 90)
                            {
                                tmpJerry->mood += 10;
                                continue;
                            }
                            if (tmpJerry->mood > 90)
                            {
                                tmpJerry->mood = 100;
                                continue;

                            }
                            if (10 <= tmpJerry->mood && tmpJerry->mood < 50)
                            {
                                tmpJerry->mood -= 10;
                                continue;

                            }
                            else
                                tmpJerry->mood = 0;
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        continue;
                    case '3': /* Adjust the picture settings on the TV */
                        for (int i = 1; i <= getLengthList(jerries); i++) {
                            tmpJerry = getDataByIndex(jerries, i);
                            if (tmpJerry->mood <= 80)
                            {
                                tmpJerry->mood += 20;
                                continue;

                            }
                            else
                                tmpJerry->mood = 100;
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        continue;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        continue;
                }
            case '9': /* I had enough. Close this place */
                memoryclear();
                fclose(configuration_file);
                printf("The daycare is now clean and close ! \n");
                exit(0);
            default:
                printf("Rick this option is not known to the daycare ! \n");
                break;

        }
    }

    return 0;
}

/* FOR LIST OF VALUES IN MULTI HASH
 * this function create a new list and add a mew value */
Element copyval(Element value)
{
    if(value == NULL)
        return NULL;
    List newlistval = createLinkedList(copyJerry, freeJerryptr, (PrintFunction) printJerry, isEqualjerryid);
    if(newlistval != NULL)
        appendNode(newlistval, value);
    return newlistval;
}
/* returned a casting to pointer to jerry */
Element copyJerry(Element jerry)
{
    if(jerry == NULL)
        return NULL;
    return (Jerry*)jerry;

}
/* it's just a casting to pointer so nothing to free */
status freeval(Element values)
{
    if(values == NULL)
        return failure;
    status result = destroyList((List)values);
    return result;
}
/* print list of values of multi hashtable */
status printval(Element values)
{
    if(values != NULL)
        displayList((List)values);
    return success;
}

/* FOR NODE IN LIST & HASH TABLE
 * deep copy of jerrie's id because it's a string */
Element copyJerryid(Element id)
{
    if(id == NULL)
        return NULL;
    char* newId = (char*)malloc(sizeof(char) * strlen((char*)id)+1);
    if(newId == NULL)
        return NULL;
    strcpy(newId, (char*)id);
        return newId;

}
/* we did copy then free it */
status freeJerryid(Element id)
{
    if(id != NULL)
        free((char*)id);
    return success;
}
/* just a 'null' function to keep the struct correct */
status printJerryid(Element id)
{
    /* find jerry by his id and print it */
    if(id == NULL)
        return failure;
    return success;
}
/* compare between 2 jerries ids */
bool isEqualjerryid(Element jerry1, Element jerry2)
{
    if(jerry1 == NULL || jerry2 == NULL)
        return false;
    char* jerryId1 = getId((Jerry*)jerry1);
    char* jerryId2 = getId((Jerry*)jerry2);
    if(strcmp(jerryId1, jerryId2) == 0)
        return true;
    return false;
}
/* sum the letters in the key (a string) and return it */
int transformIntoNumber(Element elem)
{
    if(elem == NULL)
        return 0;
    int sum = 0;
    char* str = (char*)elem;
    for(int i=0; i< strlen(str); i++)
        sum += str[i];
    return sum;
}

/* destroy pointer to jerry (just null) */
status freeJerryptr(Element jerry)
{
    jerry = NULL;
    return success;
}
/* FOR KEYS IN MULTI HASH
 * deep copy for the name (key) because it's a string */
Element copyphysicalCharname(Element name)
{
    if(name == NULL)
        return NULL;
    char* namePhc = (char*)malloc(sizeof(char)*strlen((char*)name)+1);
    strcpy(namePhc, (char*)name);
    if(namePhc != NULL)
        return namePhc;
    return NULL;
}
status freePhcname(Element name)
{
    if(name != NULL)
        free((char*)name);
    return success;
}
/* print the name of physical characteristic */
status printPhcname(Element name)
{
   if(name == NULL)
       return failure;
   printf("%s : \n", (char*)name);
   return success;
}
/* compare between 2 keys (names of physical characteristic) */
bool isEqualphcname(Element key, Element name)
{
    if(key == NULL || name == NULL)
        return false;
    if(strcmp((char *)key, (char*)name) == 0)
        return true;
    return false;
}
/* compare between 2 keys (ids) */
bool isEquakeyid(Element key, Element id)
{
    if(key == NULL || id == NULL)
        return false;
    if(strcmp((char*)key, (char*)id) == 0)
        return true;
    return false;
}
/* SEARCH FOR THE CLOSEST PRIME NUMBER */
int searchPrimeNumber(int x)
{
    if(x <= 1)
        return 2;
    int prime = x;
    bool result = false;

    while(!result)
    {
        prime ++;
        /* check if we have a prime yet */
        if(isPrime(prime))
            result = true;
    }
    return prime;
}

bool isPrime(int x)
{
    if(x == 1)
        return false;
    if(x <= 3)
        return true;
    /* check if the number divide in 2 and 3 */
    if(x%2 == 0 || x%3 == 0)
        return false;
    /* check if x is prime number by doing modulo on the numbers until sqrt of x */
    for(int i=5; i*i<=x; i+=6)
        if(x%i == 0 || x%(i+2) == 0)
            return false;
    return true;
}

/* DELETE A JERRY FROM DAYCARE
 * this is a helper function that delete a jerry from daycare in the correct order */
status deleteJerryFromdaycare(Jerry* jerry)
{
    if (jerry == NULL)
        return failure;
    Jerry* tmp = (Jerry*) jerry;
    status returned_val;
    /* remove jerry from multihash - we need to delete it from lists of values */
    for (int i = 0; i < jerry->numPhc; i++)
    {
        returned_val = removeFromMultiValueHashTable(physicalCharMultihash, tmp->pPhc[i]->name, tmp);
        if (returned_val == failure)
            return failure;
    }
    /* remove jerry from hashtable - by the id */
    returned_val = removeFromHashTable(jerriesHashtable, tmp->id);
    if (returned_val == failure)
        return failure;
    /* remove jerry from list - absolute free to jerry */
    returned_val = deleteNode(jerries, jerry);
    if (returned_val == failure)
        return failure;
    return success;

}
/* CLEAR ALL */
void memoryclear()
{
    destroyMultiValueHashTable(physicalCharMultihash);
    destroyHashTable(jerriesHashtable);
    destroyList(jerries);
    for (int i = 0; i < numberOfPlanets; i++)
        freePlanet(planetsArray[i]);
    free(planetsArray);

}
