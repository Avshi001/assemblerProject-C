/* Avshalom Madmoni - 203245097 */


#ifndef PREASSEMBLERPROTOTYPES_H

#define PREASSEMBLERPROTOTYPES_H

/* Headers */
#include "preAssemblerStructData.h"

/* Pre assembler funcs */
void preAssembler(FILE *, char *);
boolean isMacroDef(char *);
boolean isEndOfMacroDef(char *);
void getMacroName(char *, char *);
boolean isMacroNameValid(char *);
void putMacroContent(FILE *, macroP, char *);
boolean isMacroExists(FILE *);

#endif
