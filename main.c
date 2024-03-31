/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdio.h>
#include <stdlib.h>

/* Headers */
#include "assemblerStructData.h"
#include "programPrototypes.h"
#include "preAssemblerPrototypes.h"
#include "assemblerPrototypes.h"


/* Global vars */

/* Counters */
int IC;
int DC;

/* Arrays */
unsigned int data[MEMORY];
unsigned int instructions[MEMORY];

/* Linked Lists */
labelP labelsList = NULL;
externalP externalsList = NULL;

/* Error Vars */
int errorHolder = NO_ERROR;
boolean errorExists = FALSE;

/* Macro Flag */
boolean macroExists = FALSE;
 
/* Const Data */
const char * commands[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

const char * directives[] = {".data", ".string", ".struct", ".entry", ".extern"};

const char base32[32] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};


/* Program management */
int main(int argc, char * argv[])
{	
	/* Vars */
	FILE * fp;
	char * fileName;
	int i;
	
	for(i = 1; i < argc; i++)
	{	
		resetGlobals(); /* Reseting the globals vars before each file, otherwise can cause bugs in the program */
		
		fileName = addExtension(INPUT_F, argv[i]); /* Add .as to fileName */
		fp = fopen(fileName, "r"); /* Open .as file */
		
		if(fp) /* if fp != NULL */
		{	
			printf("\n==================================================\n==================================================\n");								
			
			macroExists = isMacroExists(fp); /* Check if macroExists == TRUE, macro exists inside the current file */
			/**********************Pre assembler**********************/
			if(macroExists) /* If macro exists */
			{	
				printf("\nMacro exists in \"%s\"!.\n\nStarting \"%s\" pre assembling process.\n\nCreating macro file...\n", fileName, fileName);
				preAssembler(fp, argv[i]); /* Start pre assembling process & create macro file */
				free(fileName); /* Free memory allocation */
				fileName = addExtension(MACRO_F, argv[i]); /* Add .am to fileName */
				
				if(errorExists) /* If we found an error during pre assembler process */
				{
					printf("\nError Was Found in \"%s\" during pre assembling process!\n\nProcess has been stopped!\n", fileName);
					printf("\n\"%s\" has been deleted!\n", fileName);
					remove(fileName); /* Delete the file */
					free(fileName); /* Free memory allocation */
					continue; /* Exit current loop iteration */
				}
				
				else
				{	
					printf("\nPre assembling process was completed successfully!\n");
					printf("\n\"%s\" was created!\n", fileName);
					fp = fopen(fileName, "r"); /* Open .am file */
				}
			}
			
			else
				printf("\nStarting \"%s\" assembling process\n", fileName);
			/**********************First pass**********************/
			printf("\nStarting first pass process!\n");
			firstPass(fp); /* Strat first pass process */
		
			if(!errorExists)
			{	
				printf("\n\"%s\" first pass process was completed successfully!\n\nStarting second pass process!\n", fileName);
				rewind(fp); /* Move to the beginning of the file */
				secondPass(fp, argv[i]); /* Strat second pass process */
			}

			else
			{	
				printf("\nError Was Found in \"%s\" during first pass process!\n\nProcess has been stopped!\n", fileName);
				free(fileName); /* Free memory allocation */
				continue; /* Exit current loop iteration */
			}
			/**********************Second pass**********************/
			if(!errorExists)
				printf("\n\"%s\" second pass process was completed successfully!\n", fileName);
			
			else
			{
				printf("\nError Was Found in \"%s\" during second pass process!\n\nProcess has been stopped!\n", fileName);
				free(fileName); /* Free memory allocation */
				continue; /* Exit current loop iteration */
			}		
		}		
		
		else
			writeError(FAILED_TO_OPEN_FILE_FOR_READING); /* If we could not open the file write error */
		
		free(fileName); /* Free memory allocation */
	}
	
	printf("\n\nEnd of program!\n");
	
	return 0;
}
