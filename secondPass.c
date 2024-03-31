/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Headers */
#include "programExternalVars.h"
#include "assemblerPrototypes.h"
#include "assemblerStructData.h"
#include "programData.h"
#include "programPrototypes.h"



/* This func handles the entire second pass */
void secondPass(FILE * fp, char * fileName)
{	
	/* Will contain the current line */
	char line[LINE_LENGTH];	

	/* Lines counter*/
	int lineNumber = 1;
	
	IC = 0; /* Instructions counter */
	
	/* Get lines until end of file */
	while(fgets(line, LINE_LENGTH, fp))
	{
		errorHolder = NO_ERROR; /* Reset errorHolder before parsing each line */
		
		if(!isEmptyLine(line) && !isCommentLine(line))
			secondPassLineParser(line); /* Line parser */
		
		/* If there was an error during the program, we will print the errors */
		if(errorHolder != NO_ERROR)
		{
			errorExists = TRUE; /* Global var */
			writeError(lineNumber);
		}
		
		lineNumber++; /* Add 1 to lines counter */
	}
	
	/* If we found any error on the second pass do not create output files */
	if(!errorExists)
		createFiles(fileName);
	
	/* Free memory allocation */
	freeLabelsList(&labelsList);
	freeExternalsList(&externalsList);
	
	return;	
}




/* This func parses a given line, checks for optional label, instruction line or directive line */
void secondPassLineParser(char * line)
{	
	/* Vars */	
	char curr[LINE_LENGTH];
	int currLength;
	int commandType;
	int directiveType;
	
	line = skipSpaces(line); /* Skipping spaces */
	if(isEndOfLine(line))
		return;

	copyNote(curr, line); /* Copy first note in line to curr */
	currLength = strlen(curr);
	
	/* Checking if the first note in line is label declaration (Skipping labels declaration) */
	if(curr[currLength-1] == ':')
	{
		line = nextNote(line); /* Move to the next note */
		copyNote(curr, line); /* Copy next note in line to curr */
	}
	
	/* Checking if curr is a command */
	if((commandType = checkIfCommand(curr)) != NOT_FOUND)
	{
		line = nextNote(line); /* Moving the pointer to the next note after command */
		commandsProcessorSecond(commandType, line); /* Encodes the operands */
		return;
	}
	
	/* Updating relevant labels to entry */
	if((directiveType = checkIfDirective(curr)) == ENTRY)
	{
		line = nextNote(line); /* Moving the pointer to the next note after entry declaration */
		copyNote(curr, line); /* Copy next note(label name) in line to curr */
		updateLabelToEntry(labelsList, curr); /* Search for the label name & update to entry */
		return;
	}
	
	return;
}




/* This func parses a command line by command type and encodes additional words into instructions array */
void commandsProcessorSecond(int commandType, char * line)
{	
	/* Vars */
	char firstOp[LINE_LENGTH];
	char secondOp[LINE_LENGTH];
	char * sourceOp = NULL;
	char * destinationOp = NULL;
	
	/* Operands flags */
	boolean sourceOpFlag = FALSE;
	boolean destinationOpFlag = FALSE;
	
	/* Checking which operands the command type accept */
	sourceOpFlag = isSourceOp(commandType);
	destinationOpFlag = isDestinationOp(commandType); 
	
	/* If both operands */
	if(sourceOpFlag && destinationOpFlag)
	{
		line = copyUntilComma(firstOp, line); /* Copy first operand to 'firstOp' */
		line = copyUntilComma(secondOp, line); /* 'SecondOp' == ',' and line points after ',' */
		copyUntilComma(secondOp, line); /* Copy second operand to 'secondOp' */
		
		/* Setting pointers */
		sourceOp = firstOp; 
		destinationOp = secondOp;
	}
	
	/* If only 1 operand it is destination operand */
	else if(destinationOpFlag)
	{	
		/* Copy first operand to 'firstOp' */
		copyUntilComma(firstOp, line);
		
		/* Setting pointer */
		destinationOp = firstOp;
	}

	/* Encode additional words to memory */
	encodeWords(sourceOp, sourceOpFlag, destinationOp, destinationOpFlag);
	
	return; 
}




/* This func encodes the additional words */
void encodeWords(char * sourceOp, boolean isSource, char * destinationOp, boolean isDestination)
{	
	/* Vars */
	int sourceOpAddressingMode = NOT_FOUND;
	int destinationOpAddresingMode = NOT_FOUND;
	
	if(isSource) /* If source operand exists get the addressing mode */
		sourceOpAddressingMode = getAddressingModeBits(instructions[IC], TRUE);
	
	if(isDestination) /* If destination operand exists get the addressing mode */
		destinationOpAddresingMode = getAddressingModeBits(instructions[IC], FALSE);
	
	IC++; /* We should move to the next index to insert the words */
	
	/* Encodes only one word to memory in case that both operands are registers */
	if(is2AddressingModesRegister(sourceOpAddressingMode, destinationOpAddresingMode))
		encode2Registers(sourceOp, destinationOp);
	
	else
	{
		if(isSource) /* Check if source operand exists */
		{	
			/* Handle source operand by addressing mode */
			if(sourceOpAddressingMode == REGISTER_M)
			{
				encodeRegisterSource(sourceOp); /* If addressing mode REGISTER */
			}
			
			else /* If addressing mode is (IMMEDIATE, DIRECT, STRUCT) */
				encodeWord(sourceOpAddressingMode, sourceOp); 
		}
		
		if(isDestination) /* Check if destination operand exists */
		{	
			/* Handle destination operand by addressing mode */
			if(destinationOpAddresingMode == REGISTER_M)
			{
				encodeRegisterDestination(destinationOp); /* If addressing mode REGISTER */
			}
			
			else /* If addressing mode is (IMMEDIATE, DIRECT, STRUCT) */
				encodeWord(destinationOpAddresingMode, destinationOp);
		}
	}
	
	return;
}




/* This func running the relevant encoding func by the relevant addressing mode (Except REGISTER!! ) */
void encodeWord(int addressingMode, char * op)
{
	switch(addressingMode)
	{
		case IMMEDIATE_M:
			encodeInteger(op+1); /* Skip the first char '#'  */
			break;

		case DIRECT_M:
			encodeLabel(op);
			break;
		
		case STRUCT_M:
			encodeStruct(op);
			break;
	}
}




/* This func encodes the integer to memory (IMMEDIATE)*/
void encodeInteger(char * op)
{	
	unsigned int word = 0;
	
	/* atoi function to convert string to integer */
	word = (unsigned int) atoi(op);
	
	/* Inserting ARE bits */
	word = insertAreBitsToWord(word, ABSOLUTE);
	
	/* Inserting word to instruction array */
	insertWordToInstructions(word);
	
	return;
}




/* This func encodes the label address to memory by a given label name (DIRECT)*/
void encodeLabel(char * labelName)
{	
	unsigned int word = 0;
	
	if(!isLabelExists(labelsList, labelName)) /* Checking if the label exists */
	{	
		errorHolder = LABEL_NAME_DOES_NOT_EXIST;
		IC++; 
		return;
	}
	
	/* Get the address of the label into word */
	word = getLabelAddress(labelsList, labelName); 
	
	/* If the label is external encode relevant ARE bits, add to externals list and set the address of use */
	if(isLabelExternal(labelsList, labelName))
	{	
		word = insertAreBitsToWord(word, EXTERNAL);
		addLabelToExternalsList(&externalsList, (IC + FIRST_MEMORY_ADDRESS), labelName);
	}
	/* Otherwise the label is local. encode relevant ARE for local(RELOCATABLE) */
	else
		word = insertAreBitsToWord(word, RELOCATABLE);
	
	insertWordToInstructions(word); /* Add to instructions array */
	
	return;
}




/* This func encodes the label address & the following integer to memory (STRUCT)*/
void encodeStruct(char * op)
{	
	/* Will hold the label name */
	char curr[LINE_LENGTH];
	
	/* Copy to curr the label name and returns a pointer to '.' */
	op = copyUntilDot(curr, op);
	
	op++; /* Move to the char after '.' to point the number*/
	
	/* Label is the first word to encode */
	encodeLabel(curr); 
	
	/* Integer is the second word to encode */
	encodeInteger(op);
	
	return;
}




/* This func encodes one word to memory in case that both operands are registers */
void encode2Registers(char * sourceOp, char * destinationOp)
{	
	/* Vars */
	unsigned int wordSource = 0;
	unsigned int wordDestination = 0;
	unsigned int finalWord = 0;
	
	wordSource = (unsigned int) atoi(sourceOp+1); /* Inserting bits of source register to wordSource (+1 - skipping 'r') */
	wordDestination = (unsigned int) atoi(destinationOp+1); /* Inserting bits of destination register to wordDestination (+1 - skipping 'r') */
	
	wordSource <<= (TOTAL_ADDRESSING_MODE_BITS*2); /* Inserting 0000 right to left */
	
	finalWord = (wordSource | wordDestination); /* "Turnning on" relevant bits from destination */
	
	finalWord = insertAreBitsToWord(finalWord, ABSOLUTE); /* Inserting 00 right to left (ARE bits) */
	
	insertWordToInstructions(finalWord); /* Inserting to instructions array */
	
	return;
}




/* This func encodes one word to memory in case that just source operand is register */
void encodeRegisterSource(char * op)
{	
	unsigned int word = 0;
	
	word = (unsigned int) atoi(op+1); /* Inserting bits of source register to wordSource (+1 - skipping 'r') */
	
	word <<= (TOTAL_ADDRESSING_MODE_BITS*2); /* Inserting 0000 right to left */
	
	word = insertAreBitsToWord(word, ABSOLUTE); /* Inserting 00 right to left (ARE bits) */
	
	insertWordToInstructions(word); /* Inserting to instructions array */
	
	return;
}




/* This func encodes one word to memory in case that just destination operand is register */
void encodeRegisterDestination(char * op)
{	
	unsigned int word = 0;

	word = (unsigned int) atoi(op+1); /* Inserting bits of destination register to wordDestination (+1 - skipping 'r') */
	
	word = insertAreBitsToWord(word, ABSOLUTE); /* Inserting 00 right to left (ARE bits) */
	
	insertWordToInstructions(word); /* Inserting to instructions array */
	
	return;
}




/* This func writes the relevant new files of the second pass */ 
void createFiles(char * fileName)
{
	FILE * fp;
	
	fp = createFile(OBJECT_F, fileName); /* Create ob file for writing */
	createObFile(fp);
	
	/* We should create ent file just if entry exists */
	if(isEntryLabelExists(labelsList)) 
	{
		fp = createFile(ENTRY_F, fileName); /* Create ent file for writing */
		createEntryFile(fp);
	}
	
	/* We should create ext file just if external exists */
	if(isExternalLabelExists(externalsList))
	{
		fp = createFile(EXTERN_F, fileName); /* Create ext file for writing */
		createExternFile(fp);
	}
	
	return;
}




/* This func creates the object file */
void createObFile(FILE * fp)
{
	/* Vars */
	char * first;
	char * second;
	
	/* First & second will hold the string that returns from the funcntion in base32 */
	first = convertWordToBase32(IC);
	second = convertWordToBase32(DC);
	
	/* Prints the first line in base32 (IC + DC) */
	fprintf(fp, "%s\t%s\n", first, second);
	fprintf(fp, "\n");
	
	/* Free memory allocation */
	free(first);
	free(second);
	
	/* Prints the instructions */
	printToObFile(fp, first, second, IC, FIRST_MEMORY_ADDRESS,TRUE);
	
	/* Prints the data */
	printToObFile(fp, first, second, DC, (FIRST_MEMORY_ADDRESS + IC), FALSE);
	
	/* Closing the file */
	fclose(fp);
}




/* This func creates the entry file */
void createEntryFile(FILE * fp)
{	
	/* Vars */
	labelP h;
	char * entryAddress;
	
	h = labelsList; /* Pointer to the beginning of the list  */
	
	/* As long as we did not reach the end of the list continue */
	while(h) 
	{
		if(h->entry) /* If the label is entry */
		{
			entryAddress = convertWordToBase32(h->address); /* Convert the address to base32  */
			
			/* Printing to file.ent */
			fprintf(fp, "%s\t%s", h->name, entryAddress);
			fprintf(fp, "\n");
			
			free(entryAddress); /* Free memory */
		}
		
		h = h->next; /* Move to next label */
	}
	
	fclose(fp); /* Closing the file */
	
	return;
}




/* This func creates the extern file */
void createExternFile(FILE * fp)
{	
	/* Vars */
	externalP h;
	char * externalAddress;
	
	h = externalsList; /* Pointer to the beginning of the list  */
	
	/* As long as we did not reach the end of the list continue */
	while(h) 
	{

		externalAddress = convertWordToBase32(h->address); /* Convert the address to base32  */
			
		/* Printing to file.ext */
		fprintf(fp, "%s\t%s", h->name, externalAddress);
		fprintf(fp, "\n");
			
		free(externalAddress); /* Free memory */
			
		h = h->next; /* Move to next label */
	
	}
	
	fclose(fp); /* Closing the file */
	
	return;
}




/* This func prints into object file the instrcutions part or the data part (if instructionsFlag=TRUE print instructions, if FALSE data) */
void printToObFile(FILE * fp, char * first, char * second, int counter, unsigned int address, boolean instructionsFlag)
{	
	int i; /* Index */
	
	for(i = 0; i < counter; i++)
	{	
		/* First & second will hold the string that returns from the funcntion in base32 */
		first = convertWordToBase32(address);
		
		if(instructionsFlag)		
			second = convertWordToBase32(instructions[i]);
		else
			second = convertWordToBase32(data[i]);
		
		/* Printing.. */
		fprintf (fp, "%s\t%s\n", first, second);
		
		/* Free memory allocation */
		free(first);
		free(second);
		
		/* Next address */
		address++;
	}
	
	return;
}




/* This func checks if the if source & destination operand are REGISTERS, if yes returns TRUE else returns FALSE */
boolean is2AddressingModesRegister(int source, int destination)
{
	if(source == REGISTER_M && destination == REGISTER_M)
		return TRUE;
		
	return FALSE;
}




/* This func checks by command type if source operand should exists, if yes return TRUE else return FALSE */ 
boolean isSourceOp(int commandType)
{	
	switch(commandType)
	{
		case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
        	return TRUE;
    }
    
	return FALSE;
}




/* This func checks by command type if destination operand should exists, if yes return TRUE else return FALSE */ 
boolean isDestinationOp(int commandType)
{	
	switch(commandType)
	{
		case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
        case NOT:
        case CLR:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case GET:
        case PRN:
        case JSR:
        	return TRUE;
	}
        	
	return FALSE;
}
