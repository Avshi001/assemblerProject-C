/* Avshalom Madmoni - 203245097 */


#ifndef ASSEMBLERPROTOTYPES_H

#define ASSEMBLERPROTOTYPES_H

/* Headers */
#include "assemblerStructData.h"


/* First pass funcs */
void firstPass(FILE *);
void firstPassLineParser(char *);
void commandsProcessor(int, char *);
int commandAdditionalWords(boolean, int, boolean, int);
boolean CommandNumOfOperandsValid(int, boolean, boolean);
boolean CommandAddressingModeValid(int, int, int);
void directivesProcessor(int, char *);
void dataDirectiveProcessor(char *);
void stringDirectiveProcessor(char *);
void structDirectiveProcessor(char *);
void entryDirectiveProcessor(char *);
void externDirectiveProcessor(char *);
boolean isLabelDeclaration(char *, boolean *);
unsigned int encodeFirstWord(int, boolean, int, boolean, int);
int findAddressingMode(char *);


/* Second pass funcs */
void secondPass(FILE *, char *);
void secondPassLineParser(char *);
void commandsProcessorSecond(int, char *);
void encodeWords(char *, boolean, char *, boolean);
void encodeWord(int, char *);
void encodeInteger(char *);
void encodeLabel(char *);
void encodeStruct(char *);
void encode2Registers(char *, char *);
void encodeRegisterSource(char *);
void encodeRegisterDestination(char *);
void createFiles(char *);
void createObFile(FILE *);
void createEntryFile(FILE *);
void createExternFile(FILE *);
void printToObFile(FILE *, char *, char *, int, unsigned int, boolean);
boolean is2AddressingModesRegister(int, int);
boolean isSourceOp(int);
boolean isDestinationOp(int);

#endif
