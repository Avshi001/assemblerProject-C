/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* Headers */
#include "programExternalVars.h"
#include "assemblerStructData.h"
#include "programData.h"
#include "programPrototypes.h"



/* This func checks if a given label name is valid */
boolean isValidLabel(char * labelName, boolean error, boolean colonFlag)
{	
	/* error==TRUE if we would like to update errorHolder(global var) */
	/* colonFlag==TRUE if we would like to print the errors of label declaration */	

	/* Vars */
	int labelNameLength = strlen(labelName);
	
	if(labelName == NULL) /* "Empty" */
		return FALSE;

	/* Checking label length */
	if(labelNameLength < MIN_LABEL_LENGTH || labelNameLength > MAX_LABEL_LENGTH)
	{	
		if(error)
			errorHolder = LABEL_NAME_INVALID_LENGTH;
		
		return FALSE;	
	}
	
	if(!isalpha(*labelName)) /* Label must start with a letter */
	{
		if(error)
		{	
			if(colonFlag)
				errorHolder = LABEL_DECLARTION_NAME_MUST_START_WITH_A_LETTER;
			else
				errorHolder = LABEL_NAME_MUST_START_WITH_A_LETTER;
		}
		return FALSE;
	}
	
	if(isRegister(labelName)) /* Label cannot have the same name as a register */
	{	
		if(error)
		{	
			if(colonFlag)
				errorHolder = LABEL_DECLARATION_NAME_CAN_NOT_BE_REGISTER;
			else
				errorHolder = LABEL_NAME_CAN_NOT_BE_REGISTER;
		}	
		return FALSE;
	}
	
	if(checkIfCommandName(labelName)) /* Label cannot have the same name as a command */
	{
		if(error)
		{	
			if(colonFlag)
				errorHolder = LABEL_DECLARATION_NAME_CAN_NOT_BE_COMMAND;
			else
				errorHolder = LABEL_NAME_CAN_NOT_BE_COMMAND;
		}		
		return FALSE;
	}
	
	
	if(checkIfDirectiveName(labelName)) /* Label cannot have the same name as a directive */
	{
		if(error)
		{	
			if(colonFlag)
				errorHolder = LABEL_DECLARATION_NAME_CAN_NOT_BE_DIRECTIVE;
			else
				errorHolder = LABEL_NAME_CAN_NOT_BE_DIRECTIVE;
		}
		return FALSE;
	}
	
	
	/* Label can only contain letters and numbers */
	while(*labelName != '\0')
	{
		if(!isalnum(*labelName))
		{	
			if(error)
			{	
				if(colonFlag)
					errorHolder = LABEL_DECLARATION_NAME_SHOULD_CONTAIN_ALPHANUMERIC_ONLY;
				else
					errorHolder = LABEL_NAME_SHOULD_CONTAIN_ALPHANUMERIC_ONLY;
			}
			return FALSE;
		}

		labelName++;
	}
	
	return TRUE;
}




/* This func checks if a given label exists in the list of labels */
boolean isLabelExists(labelP head, char * labelName)
{
	while(head)
	{
		if(!strcmp((head->name), labelName)) /* Comparing */
			return TRUE;
		
		head = head->next; /* Move to next label */
	}
	
	return FALSE;
}




/* This func search for a label by a given label name and returns the label address, if could not find returns 0 */
unsigned int getLabelAddress(labelP h, char * labelName)
{
	labelP p;
	
	p = getLabel(h,labelName); /* Searching for the label */
	
	if(p) /* if p != NULL */	
		return p->address;

	return 0;
}




/* This func search for a label by a given label name and returns a pointer to the label, otherwise returns NULL */
labelP getLabel(labelP h, char * labelName)
{
	while(h)
	{
		if(!strcmp((h->name), labelName)) /* Comparing */
			return h;
		
		h = h->next; /* Move to next label */
	}
	
	return NULL;
}




/* This func adds a label to the labels list */
labelP addLabelToList(labelP * hptr, char * labelName)
{	
	/* Vars */
	labelP ptr = *hptr;
	labelP temp; 
	
	/* Checking if the label already exists */
	if(isLabelExists(*hptr, labelName))
		return NULL;
	
	/* Memory allocation */
	temp = (labelP) malloc(sizeof(label)); 
	
	if(!temp) /* Checking if memory allocation failed */
	{
		printf("\nMemory allocation failed!\n");
		exit(ERROR);
	}

	/* Setting */
	strcpy(temp->name, labelName);
	temp->usedInCommandLine = FALSE;
	temp->entry = FALSE;
	temp->external = FALSE;
	temp->address = 0;

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




/* This func frees the allocated memory for the labels list */
void freeLabelsList(labelP * hptr)
{
	labelP ptr;
	
	while(*hptr)
	{
		ptr = *hptr;
		*hptr = (*hptr)->next;
		free(ptr);
	}
	
	return;
}




/* This func searches for a label name in the list.
If it finds the name of the label, it deletes it from the list and returns TRUE, otherwise it returns FALSE*/
boolean deleteLabelFromList(labelP * hptr, char * labelName)
{	
	/* Vars */
	labelP ptr = *hptr;
    labelP temp;
	
	/* Checks if the names are equal */
	if(!strcmp((*hptr)->name, labelName))
	{
		*hptr = (*hptr)->next; /* In case that the label is the head of the list */
		free(ptr);
		return TRUE;
	}
	
	temp = ptr; /* 'Temp' is always "one step behind" 'ptr' */
	ptr = ptr->next; /* Move to next label */
	
    while(ptr) /* As long that we did not reach the end of the list continue */
    {	
    	/* Checks if the names are equal */
        if(!strcmp((ptr->name), labelName))
        {	
        	temp->next = ptr->next;

            free(ptr);

            return TRUE;

        }
        
        temp = ptr; /* 'Temp' is always "one step behind" 'ptr' */
        ptr = ptr->next; /* Move to next label */

    }
    
    return FALSE;
}




/* This func prints the labels list */
void printLabelsList(labelP h)
{	
	printf("\n");

    while (h)
    {
        printf("\nname: %s\t address: %d\t usedInCommandLine: %d\t entry: %d\t external: %d \n", h->name, h->address, h->usedInCommandLine, h->entry, h->external);
        
        h = h->next;
    }
    
	printf("End of labels list\n");
}




/* This func checks in the labels list if a label is marked as external by a given label name */
boolean isLabelExternal(labelP h, char * labelName)
{
	while(h)
	{
		if(!strcmp((h->name), labelName)) /* In case that we have a match by name */
		{
			if(h->external) /* Check if the label marked as external */
				return TRUE;
		}
		
		h = h->next; /* Move to the next label */
	}
	
	return FALSE;
}




/* This func checks in the labels list if at least one of the labels marked as entry */
boolean isEntryLabelExists(labelP h)
{
	while(h)
	{
		if(h->entry) /* Check if the label marked as entry */
			return TRUE;
		
		h = h->next; /* Move to the next label */
	}
	
	return FALSE;
}




/* This func search for a label in labels list by a given label name and updates the entry field to TRUE, 
If could not find the label return FALSE otherwise return TRUE */
boolean updateLabelToEntry(labelP h, char * labelName)
{
	labelP ptr;
	
	ptr = getLabel(h, labelName); /* Searching for the label */
	
	if(!ptr) /* If we could not find */
	{
		errorHolder = LABEL_NAME_DOES_NOT_EXIST;
		return FALSE;
	}
	
	if(ptr->external) /* If the label set as external */
	{
		errorHolder = LABEL_CAN_NOT_BE_ENTRY_AND_EXTERN;
		return FALSE;
	}
	
	ptr->entry = TRUE; /* updating */
	
	return TRUE;
}




/* This func updates labels address */
void updateLabelAddress(labelP h, int delta)
{
	while(h) /* While h =! NULL */
	{
		if(!(h->external)) /* If non-external address */
		{
			/* If the label was set before a command line */
			if(h->usedInCommandLine)
				h->address += FIRST_MEMORY_ADDRESS;
			
			/* If the label was set before a directive line */
			else
				h->address += (FIRST_MEMORY_ADDRESS + delta);
		}
		
		h = h->next; /* Move to next label */
	}
	
	return;
}
