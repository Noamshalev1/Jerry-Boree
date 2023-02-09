#ifndef DEFS_H_
#define DEFS_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum e_bool { false, true } bool;
typedef enum e_status { success, failure } status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif /* DEFS_H_ */
