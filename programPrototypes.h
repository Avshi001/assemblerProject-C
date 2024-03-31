/* Avshalom Madmoni - 203245097 */


#ifndef PROGRAMPROTOTYPES_H

#define PROGRAMPROTOTYPES_H

/* Headers */
#include "preAssemblerStructData.h"
#include "assemblerStructData.h"


/* Label funcs */
void updateLabelAddress(labelP, int);
boolean isValidLabel(char *, boolean, boolean);
boolean isLabelExists(labelP, char *);
unsigned int getLabelAddress(labelP, char *);
labelP getLabel(labelP, char *);
labelP addLabelToList(labelP *, char *);
void freeLabelsList(labelP *);
boolean deleteLabelFromList(labelP *, char *);
void printLabelsList(labelP);
boolean isLabelExternal(labelP, char *);
boolean isEntryLabelExists(labelP);
boolean updateLabelToEntry(labelP, char *);


/* External funcs */
externalP addLabelToExternalsList(externalP *, unsigned int, char *);
void printExternalsList(externalP);
void freeExternalsList(externalP *);
boolean isExternalLabelExists(externalP);


/* Macro funcs */
macroP addMacroToList(macroP *, char *, char *);
boolean isMacroName(macroP, char *, char *);
void freeMacrosList(macroP *);


/* Useful funcs */
void writeError(int);
boolean isInteger(char *);
boolean isString(char *);
boolean isRegister(char *);
boolean checkIfDirectiveName(char *);
boolean checkIfCommandName(char *);
char * addExtension(int, char *);
FILE * createFile(int, char *);
void insertWordToInstructions(unsigned int);
void insertNumToData(int);
void insertStringToData(char *);
void insertStructToData(int, char *);
int checkIfCommand(char *);
int checkIfDirective(char *);
void resetGlobals();


/* Bits funcs */
unsigned int insertAreBitsToWord(unsigned int, int);
char * convertWordToBase32(unsigned int);
unsigned int getFiveBits(unsigned int, boolean);
unsigned int getAddressingModeBits(unsigned int, boolean);


/* Parsing funcs */
char * skipSpaces(char *);
void copyNote(char *, char *);
char * nextNote(char *);
boolean isEndOfLine(char *);
boolean isEmptyLine(char *);
boolean isCommentLine(char *);
boolean isFirstCharValid(char *);
char * copyUntilComma(char *, char *);
char * copyUntilDot(char *, char *);

#endif
