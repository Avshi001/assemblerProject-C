/* Avshalom Madmoni - 203245097 */


#ifndef ASSEMBLERSTRUCTDATA_H

#define ASSEMBLERSTRUCTDATA_H

/* Headers */
#include "programData.h"


typedef struct structLabel * labelP;
typedef struct structLabel
{
	char name[MAX_LABEL_LENGTH];
	unsigned int address;
	boolean usedInCommandLine;
	boolean entry;
	boolean external;
	labelP next;
}label;


typedef struct structExternal * externalP;
typedef struct structExternal
{
    char name[MAX_LABEL_LENGTH];
    unsigned int address;
    externalP next;
}external;

#endif
