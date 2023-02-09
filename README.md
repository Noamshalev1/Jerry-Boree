# Jerry-Boree
continue of "Manage jerries"
3 Main Generic ADTs: LinkedList, KeyValuePair and HashTable.
The MultiValueHash ADT is combine of the three of them. 
The size of hashtable and multi hashtable decided by the closest prime number of the number of jerries (by a help function I created under main).
Similary, the size of multihash decided by the number of jerries + number of physical characteristics.
All the functions that sended to them was created in the JerryBoreeMain (under main). 
The main idea was to collect all jerries to linkedlist, then add from the list to hash and multihash. 
The function that decide about the index also created in the main and it sum the letters of the key, then do modulo by the size of the table. 
There is also one more help function that delete a jerry from all the ADTs containes a pointer to it. 
The last helper function called "memoryclear" and it responsable to exit saftly from the program. 
