/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Headers */
#include "programExternalVars.h"
#include "preAssemblerPrototypes.h"
#include "preAssemblerStructData.h"
#include "programData.h"
#include "programPrototypes.h"



/* This func handles the entire preAssembler process */
void preAssembler(FILE * fpOriginal, char * fileName)
{
	/* Vars */
	char line[LINE_LENGTH];
    char currMacroName[LINE_LENGTH];
    
    boolean macroFlag= FALSE;
    macroP macrosList = NULL;
	macroP ptr = NULL;

	/* Lines counter*/
    int lineNumber = 1; 
    
	/* Open .am file for writing */
    FILE * fpMacro = createFile(MACRO_F, fileName);
    
    /* Get lines until end of file */
    while(fgets(line, LINE_LENGTH, fpOriginal))
    {	
		errorHolder = NO_ERROR; /* Reset errorHolder before parsing each line */
		
    	/* MacroFlag is 'on' durring macro body */
    	if(macroFlag) 
    	{
    		if(!isEndOfMacroDef(line)) /* Checking if we have not reached the end of macro body */
    		{
				ptr = addMacroToList(&macrosList, currMacroName, line); /* Add line + name to macrosList */

				if(!ptr) /* Checking if memory allocation failed */
				{
					printf("\nMemory allocation failed!\n");
					exit(ERROR);
				}
			}

			else /* We have reached the end of macro body turn flag off */
				macroFlag = FALSE; 
    	}
    	
    		
    	else if(isMacroDef(line)) /* Checking if we have reached the beginning of macro body */
    	{
    		getMacroName(currMacroName, line); /* Copy macro name into currMacroName */
    		
    		if(isMacroNameValid(currMacroName)) /* Check if macro name is valid */
    			macroFlag = TRUE; /* We have reached the beginning of macro body turn flag on */
    	}
    	
		/* MacroFlag is 'off' outside macro body */
    	if(!macroFlag) 
    	{
    		if(isMacroName(ptr, currMacroName, line)) /* Check if the current line is macro name */
    			putMacroContent(fpMacro, macrosList, currMacroName); /* Put relevant macro content into file */
			
			/* Otherwise check if the current line is diffrent then  end of macro , if so put the line into the file*/					    			
    		else if(!isEndOfMacroDef(line)) 
    			fprintf(fpMacro, "%s", line);
    	}

		/* If there was an error during the program, we will print the errors */
        if(errorHolder != NO_ERROR)
        {
            errorExists = TRUE; /* Global var */
            writeError(lineNumber); 
        }
        
        lineNumber++; /* Add 1 to lines counter */
    	
    }
    
    rewind(fpMacro); /* Move the pointer to the beginning */
    
    fclose(fpOriginal); /* Close file */
	
	fclose(fpMacro); /* Close file */
	
    freeMacrosList(&macrosList); /* Free macros list */
    
    return;
}



/* This func checks if the given line is the beginning of macro body */
boolean isMacroDef(char * line)
{
	
	char curr[LINE_LENGTH];
	
	line = skipSpaces(line); /* Skip spaces */
	
	if(isEndOfLine(line)) /* Empty line */
		return FALSE;
	
	copyNote(curr,line); /* Copy first note in line to curr */
	
	if(!strcmp(curr, "macro")) /* Check if it is 'macro' */
		return TRUE;

	return FALSE;
}



/* This func checks if the given line is the end of macro body */
boolean isEndOfMacroDef(char * line)
{
	
	char curr[LINE_LENGTH];
	
	line = skipSpaces(line); /* Skip spaces */
	
	if(isEndOfLine(line)) /* Empty line */
		return FALSE;
	
	copyNote(curr,line); /* Copy first note in line to curr */
	
	if(!strcmp(curr, "endmacro")) /* Check if it is 'endmacro' */
		return TRUE;

	return FALSE;
}



/* This func inserting macro name into dest */
void getMacroName(char * dest, char * line)
{
	
	line = nextNote(line); /* Skip spaces */
	
	copyNote(dest,line); /* Copy macro name into dest */

	return;
}



/* This func checks if a given macro name is valid */
boolean isMacroNameValid(char * macroName)
{
	
	if(isRegister(macroName))
	{
		errorHolder = MACRO_NAME_CAN_NOT_BE_REGISTER;
		return FALSE;
	}
						
	if(checkIfCommandName(macroName))
	{
		errorHolder = MACRO_NAME_CAN_NOT_BE_COMMAND;
		return FALSE;
	}
						
	if(checkIfDirectiveName(macroName))
	{
		errorHolder = MACRO_NAME_CAN_NOT_BE_DIRECTIVE;
		return FALSE;
	}
						
	return TRUE;
}



/* This func check if a given macro name is exists and copies macro body content into a given file */
void putMacroContent(FILE * fpMacro, macroP h, char * macroName)
{
	while(h)
	{
		if(!strcmp((h->name), macroName)) /* Check if macro name exists */
			fprintf(fpMacro, "%s", h->line); /* Print to file macro content */
		
		h = h->next; /* Move to the next macro */
		
	}
	
	return;
}



/* This func checks if macro line exists in the original file, if exists returns TRUE else returns FALSE */
boolean isMacroExists(FILE * fpOriginal)
{
	char line[LINE_LENGTH];
    
	while(fgets(line, LINE_LENGTH, fpOriginal)) /* Checks until EOF */
	{
		if(isMacroDef(line)) /* If the string 'macro' exists return TRUE */
		{		
			rewind(fpOriginal); /* Move the pointer to the beginning */
			return TRUE;	
		}
	}
	
	rewind(fpOriginal); /* Move the pointer to the beginning */
	
	return FALSE;
}
