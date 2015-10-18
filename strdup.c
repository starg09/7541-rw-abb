#include "strdup.h"
#include <string.h>
#include <stdlib.h>

char* strdup (const char* s) {
	char* dup = malloc(strlen (s) + 1);
	if (dup == NULL)
		return NULL;
	strcpy (dup,s);
	return dup;
}