/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Headers */
#include "programData.h"
#include "programExternalVars.h"
#include "programPrototypes.h"



/* The function inserts words into the instruction array */
void insertWordToInstructions(unsigned int word)
{
    instructions[IC] = word;
    IC++; /* Increases the instruction counter by 1 */
    
    return;
}



/* This func inserts a given number into the data array */
void insertNumToData(int num)
{
    data[DC] = (unsigned int) num; /* Inserting.. */
    DC++; /* Increase the data counter by 1 */
    
    return;
}




/* This func inserts a given string into the data array */
void insertStringToData(char * string)
{
    while(*string != '\0')
    {	
        data[DC] = (unsigned int) *string; /* Inserting.. */
        DC++; /* Increase the data counter by 1 */
        string++; /* Move to next char */
    }
    
    data[DC] = '\0';/* Adding NULL to data counter after the string */
    DC++; /* Increase the data counter by 1 */
    
    return;
}




/* This func inserts a given string and num into the data array */
void insertStructToData(int num, char * string)
{
	insertNumToData(num); /* Inserting num to data array */
	insertStringToData(string); /* Inserting string to data array */
	
	return;
}




/* This func creating new files for writing with relevant extension by given file type,
And returns a pointer to the file */
FILE * createFile(int fileType, char * originalName)
{	
	FILE * newFileName;
	
	/* Add file extension according to fileType */
	originalName = addExtension(fileType,originalName);
	
	newFileName = fopen(originalName, "w"); /* Trying to open new file with the new name */
	
	free(originalName); /* Free memory allocation */
	
	if(!newFileName) /* Checking if fopen succeeded */
	{
		errorHolder = FAILED_TO_OPEN_NEW_FILE_FOR_WRITING;
		return NULL;
	}
	
	return newFileName;
}




/* This func adds to a given file name the relevant extension according to the file type */
char * addExtension(int fileType, char * fileName)
{	
	/* Vars */
	int fileNameLength;
	char * updatedName;
	
	fileNameLength = strlen(fileName);
	
	/* Memory allocation */
	updatedName = (char *) malloc(fileNameLength + MAX_EXTENSION);
	
	/* Checking memory allocation */
	if(!updatedName) 
	{
		printf("Memory allocation failed!");
        exit(ERROR);
	}
	
	/* Copy the name of the original file into updatedName 
	strncpy(updatedName, fileName, fileNameLength);*/
	strcpy(updatedName, fileName);
	
	/* Adding the desired extension for the file name according to the file type */
	switch (fileType)
    {
        case INPUT_F:
        	
        	strcat(updatedName, ".as");
            break;
            
        case MACRO_F:
        	
        	strcat(updatedName, ".am");
			break;			

        case OBJECT_F:
            
            strcat(updatedName, ".ob");
            break;

        case ENTRY_F:
            
            strcat(updatedName, ".ent");
            break;

        case EXTERN_F:
            
            strcat(updatedName, ".ext");

    }
    
    /* Returns a pointer to the updated filename with the relevant extension */
    return updatedName;
}




/* This func checks if the given string is a command name */
boolean checkIfCommandName(char * string)
{
	int i; /* Index */	
	
	for(i = 0; i < TOTAL_COMMANDS; i++)
    {	
    	/* Comparing strings and returns TRUE in case that we have a match */
        if(!strcmp(string, commands[i])) 
        	return TRUE; 
	}
	
	return FALSE;
}




/* This func checks if the given string is a directive name */
boolean checkIfDirectiveName(char * string)
{
	int i; /* Index */	
	
	for(i = 0; i < TOTAL_DIRECTIVES; i++)
    {	
    	/* Comparing strings and returns TRUE in case that we have a match */
        if(!strcmp(string, (directives[i]))) /* Check with '.' */
        	return TRUE;

		/* Comparing strings and returns TRUE in case that we have a match */
        if(!strcmp(string, (directives[i]+1))) /* Check without '.' */
        	return TRUE; 
	}
	
	return FALSE;
}




/* This func checks if a given string is a valid register name */
boolean isRegister(char * string)
{
	int stringLength;
	
	stringLength = strlen(string); /* Get string length */
	
	/* A valid register name must start with 'r' and after a number between 0-7 */
    if(stringLength != REGISTER_LENGTH || string[0] != 'r' || string[1] < '0' || string[1] > '7')
    	return FALSE; 
    	
    return TRUE;
}




/* This func checks if the given note is a valid string */
boolean isString(char * note)
{
	
	if(note == NULL) /* "Empty" note */
		return FALSE;
	
	if(*note != '"') /* A string must start with a ' " ' */
		return FALSE;
		
	note++;
	
	while(*note != '"' && *note != '\0') /* Continue until end of note or ' " ' */
		note++;
	
	if(*note == '\0') /* If we stopped because end of note */
		return FALSE;
	
	/* We stopped because of quotation mark */
	
	note++; /* Move to next char */
	
	if(*note != '\0') /* Last char must be NULL */
		return FALSE;
	
	return TRUE;
}




/* This func checks if a given string is an integer */
boolean isInteger(char * string)
{
	if(isEndOfLine(string)) /* End of string/line */
		return FALSE;
	
	if(*string == '+' || *string == '-') /* Skips '+' or '-' in case that exists */
		string++; /* Move to the next char */
	
	if(!isdigit(*string)) /* Checks if the first char is a digit */
		return FALSE;
	
	string++; /* Move to the next char */
	
	while(!isEndOfLine(string)) /* As long as we haven't reached the end of the line/string continue */
	{
		if(!isdigit(*string)) /* Checks if the next chars are digits only */
			return FALSE;
		
		string++;
	}
	
	return TRUE;
}




/* This func checks if a given string is a valid command from an array of strings and returns is decimal value */
int checkIfCommand(char * string)
{
	int i; /* Index */
	
	if(string == NULL || !isalpha(*string)) /* Valid command must start with a letter */
		return NOT_FOUND;
	
	for(i = 0; i < TOTAL_COMMANDS; i++)
    {	
    	/* Comparing strings and returns command number in case that we have a match */
        if(!strcmp(string, commands[i]))
        	return i; /* Command number */
	}
	
	return NOT_FOUND;
}




/* This func checks if a given string is a valid directive from an array of strings and returns is decimal value*/
int checkIfDirective(char * string)
{
	int i; /* Index */	
	
	if(string == NULL || *string != '.') /* Valid directive must start with '.' */
		return NOT_FOUND;
	
	for(i = 0; i < TOTAL_DIRECTIVES; i++)
    {	
    	/* Comparing strings and returns directive number in case that we have a match */
        if(!strcmp(string, directives[i])) 
        	return i; /* Directive number */
	}
	
	return NOT_FOUND;
}



/* This func resets all the global vars to prevent bugs when sending more then 1 file through the terminal */
void resetGlobals()
{
	/* Counters */
	IC = 0;
	DC = 0;

	/* Linked Lists */
	labelsList = NULL;
	externalsList = NULL;

	/* Error Vars */
	errorHolder = NO_ERROR;
	errorExists = FALSE;

	/* Macro Flag */
	macroExists = FALSE;
	
	return;
}



/* This func prints to the screen all of the possible errors */
void writeError(int lineNumber)
{
	fprintf(stderr,  "\nERROR! " );
	fprintf(stderr,  "line %d:  " , lineNumber);
	
	switch(errorHolder)
	{
		case FAILED_TO_OPEN_FILE_FOR_READING:
			fprintf(stderr,  "Filed to open original file for reading. \n" );
			break;

		case FAILED_TO_OPEN_NEW_FILE_FOR_WRITING:
			fprintf(stderr,  "Filed to open new file for writing. \n" );
			break;

		case MACRO_NAME_CAN_NOT_BE_REGISTER:
			fprintf(stderr,  "Macro name can not be a register name. \n" );
			break;

		case MACRO_NAME_CAN_NOT_BE_COMMAND:
			fprintf(stderr,  "Macro name can not be a command name. \n" );
			break;

		case MACRO_NAME_CAN_NOT_BE_DIRECTIVE:
			fprintf(stderr,  "Macro name can not be a directive name. \n" );
			break;

		case LABEL_NAME_INVALID_LENGTH:
			fprintf(stderr,  "Label name valid length is 1-30 characters. \n" );
			break;

		case LABEL_NAME_MUST_START_WITH_A_LETTER:
			fprintf(stderr,  "Label name must start with a letter. \n" );
			break;

		case LABEL_NAME_CAN_NOT_BE_REGISTER:
			fprintf(stderr,  "Label name can not be a register name. \n" );
			break;

		case LABEL_NAME_CAN_NOT_BE_COMMAND:
			fprintf(stderr,  "Label name can not be a command name. \n" );
			break;

		case LABEL_NAME_CAN_NOT_BE_DIRECTIVE:
			fprintf(stderr,  "Label name can not be a directive name. \n" );
			break;

		case LABEL_NAME_SHOULD_CONTAIN_ALPHANUMERIC_ONLY:
			fprintf(stderr,  "Label name should contain only letters and numbers. \n" );
			break;

		case LABEL_NAME_ALREADY_EXISTS:
			fprintf(stderr,  "Label name already exists. \n" );
			break;

		case LABEL_NAME_DOES_NOT_EXIST:
			fprintf(stderr,  "Label name does not exist. \n" );
			break;

		case LABEL_CAN_NOT_BE_ENTRY_AND_EXTERN:
			fprintf(stderr,  "Label can not be defined as entry and external. \n" );
			break;

		case LABEL_DECLARATION_MUST_END_WITH_A_COLON:
			fprintf(stderr,  "Label declaration must end with a colon. \n" );
			break;

		case LABEL_DECLARATION_INVALID_LENGTH:
			fprintf(stderr,  "Label declaration valid length is 2-31 characters. \n" );
			break;

		case LABEL_DECLARATION_LINE_INVALID:
			fprintf(stderr,  "Label declaration line - unknown command or directive.  \n" );
			break;
		
		case LABEL_DECLARATION_LINE_ONLY_LABEL:
			fprintf(stderr,  "Label declaration line - empty - command or directive is missing. \n" );
			break;		

		case LABEL_DECLARTION_NAME_MUST_START_WITH_A_LETTER:
			fprintf(stderr,  "Label declaration name must start with a letter. \n" );
			break;

		case LABEL_DECLARATION_NAME_CAN_NOT_BE_REGISTER:
			fprintf(stderr,  "Label declaration name can not be a register name. \n" );
			break;

		case LABEL_DECLARATION_NAME_CAN_NOT_BE_COMMAND:
			fprintf(stderr,  "Label declartion name can not be a command name. \n" );
			break;
		
		case LABEL_DECLARATION_NAME_CAN_NOT_BE_DIRECTIVE:
			fprintf(stderr,  "Label declaration name can not be a directive name. \n" );
			break;		

		case LABEL_DECLARATION_NAME_SHOULD_CONTAIN_ALPHANUMERIC_ONLY:
			fprintf(stderr,  "Label declaration name should contain only letters and numbers. \n" );
			break;
		
		case LINE_INVALID_FIRST_CHAR:
			fprintf(stderr,  "Line first non-blank character must be a letter or a dot. \n" );
			break;

		case LINE_INVALID:
			fprintf(stderr,  "Line error - command, directive or label declaration is expected first. \n" );
			break;

		case COMMAND_FIRST_CHAR_COMMA:
			fprintf(stderr,  "Command line - first character can not be a comma. \n" );
			break;

		case COMMAND_NO_COMMA_BETWEEN_OPERANDS:
			fprintf(stderr,  "Command line - a comma is expected between the operands. \n" );
			break;
		
		case COMMAND_NO_OPERAND_AFTER_COMMA:
			fprintf(stderr,  "Command line - no operand after comma. \n" );
			break;

		case COMMAND_COMMAS_IN_A_ROW:
			fprintf(stderr,  "Command line - commas in a row. \n" );
			break;

		case COMMAND_TOO_MANY_OPERANDS:
			fprintf(stderr,  "Command line - too many operands (Maximum 2 operands). \n" );
			break;

		case COMMAND_INVALID_NUM_OF_OPERANDS:
			fprintf(stderr,  "Command line - invalid number of operands for command type. \n" );
			break;

		case COMMAND_INVALID_OPERANDS_ADDRESSING_MODE:
			fprintf(stderr,  "Command line - invalid addressing mode for command type. \n" );
			break;

		case DIRECTIVE_WITHOUT_PARAMETERS:
			fprintf(stderr,  "Directive line without parameters. \n" );
			break;

		case DATA_FIRST_CHAR_COMMA:
			fprintf(stderr,  "Data directive line - first character can not be a comma. \n" );
			break;

		case DATA_INVALID_PARAMETER:
			fprintf(stderr,  "Data directive line - only accepts integers. \n" );
			break;

		case DATA_COMMAS_IN_A_ROW:
			fprintf(stderr,  "Data directive line - commas in a row. \n" );
			break;

		case DATA_NO_COMMA_BETWEEN_NUMBERS:
			fprintf(stderr,  "Data directive line - a comma is expected between the integers. \n" );
			break;

		case DATA_LAST_PARAMETER_COMMA:
			fprintf(stderr,  "Data directive line - last character can not be a comma. \n" );
			break;

		case STRING_INVALID:
			fprintf(stderr,  "String directive line - invalid string. \n" );
			break;

		case STRING_TOO_MANY_PARAMETERS:
			fprintf(stderr,  "String directive line - only accepts one string. \n" );
			break;

		case STRUCT_FIRST_CHAR_COMMA:
			fprintf(stderr,  "Struct directive line - first character can not be a comma. \n" );
			break;

		case STRUCT_FIRST_PARAMETER_MUST_BE_NUMBER:
			fprintf(stderr,  "Struct directive line - first parameter must be an integer. \n" );
			break;

		case STRUCT_MISSING_COMMA_BETWEEN_PARAMETERS:
			fprintf(stderr,  "Struct directive line - a comma is expected between the 2 parameters. \n" );
			break;

		case STRUCT_MISSING_SECOND_PARAMETER:
			fprintf(stderr,  "Struct directive line - string is expected as second parameter. \n" );
			break;

		case STRUCT_COMMAS_IN_A_ROW:
			fprintf(stderr,  "Struct directive line - commas in a row. \n" );
			break;

		case STRUCT_TOO_MANY_PARAMETERS:
			fprintf(stderr,  "Struct directive line - only accepts 2 parameters. \n" );
			break;

		case STRUCT_SECOND_PARAMETER_MUST_BE_STRING:
			fprintf(stderr,  "Struct directive line - second parameter must be a string. \n" );
			break;

		case ENTRY_TOO_MANY_PARAMETERS:
			fprintf(stderr,  "Entry directive line - accept only 1 parameter and it must be a valid label name.\n" );
			break;

		case EXTERN_TOO_MANY_PARAMETERS:
			fprintf(stderr,  "Extern directive line - accept only 1 parameter and it must be a valid label name. \n" );
			break;
	}
}
