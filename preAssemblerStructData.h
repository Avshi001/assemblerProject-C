/* Avshalom Madmoni - 203245097 */


#ifndef PREASSEMBLERSTRUCTDATA_H

#define PREASSEMBLERSTRUCTDATA_H

/* Headers */
#include "programData.h"


typedef struct structMacro * macroP;
typedef struct structMacro
{

	char name[LINE_LENGTH];
	char line[LINE_LENGTH];
	macroP next;
	
}macro;

#endif
