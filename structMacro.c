/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Headers */
#include "preAssemblerStructData.h"
#include "programData.h"
#include "programPrototypes.h"



/* This func adds a macro to the macros list */
macroP addMacroToList(macroP * hptr, char * currMacroName, char * line)
{	
	/* Vars */
	macroP ptr = *hptr;
	macroP temp; 

	
	/* Memory allocation */
	temp = (macroP) malloc(sizeof(macro)); 
	
	if(!temp) /* Checking if memory allocation failed */
	{
		printf("\nMemory allocation failed!\n");
		exit(ERROR);
	}

	/* Setting relevant fields */
	strcpy(temp->name, currMacroName);
	strcpy(temp->line, line);

	/* If the list is empty updating the 'head' to point to the first label  */
	if(!(*hptr))
	{	
		*hptr = temp;
		temp -> next = NULL;
		return temp;	
	}

	/* Adding the label to the end of the list */
	while(ptr->next) /* Continue until ptr-> next == NULL to the end of the list*/
		ptr = ptr->next;
	
	temp->next = NULL;
	ptr->next = temp;

	return temp;
}




/* This func checks if a given macro name is exists in macros list and copies the name to dest */
boolean isMacroName(macroP h, char * dest, char * line)
{
	char curr[LINE_LENGTH];
	
	line = skipSpaces(line); /* Skip spaces */
	
	copyNote(curr, line); /* Copy first token in line to curr */
	
	while(h) /* If h != NULL */
	{	
		if(!strcmp(h->name, curr)) /* Check if macro name exists */
		{
			strcpy(dest, curr); /* Copy curr into dest */
			return TRUE;
		}
			
		h = h->next; /* Move to the next macro */
	}
	
	return FALSE;
}




/* This func frees the allocated memory for the macros list */
void freeMacrosList(macroP * hptr)
{
	macroP ptr;
	
	while(*hptr)
	{
		ptr = *hptr;
		*hptr = (*hptr)->next;
		free(ptr);
	}
	
	return;
}
