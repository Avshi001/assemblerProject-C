/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Headers */
#include "programData.h"
#include "assemblerStructData.h"



/* This func adds an external label to the externals list */
externalP addLabelToExternalsList(externalP *hptr, unsigned int addressOfUse, char * labelName)
{	
	/* Vars */
	externalP ptr = *hptr;
	externalP temp; 

	/* Memory allocation */
	temp = (externalP) malloc(sizeof(external)); 
	
	if(!temp) 
	{
		printf("\nMemory allocation failed!\n");
		exit(ERROR);
	}

	/* Updating relevant fields */
	strcpy(temp->name, labelName);
	temp->address = addressOfUse;
	
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




/* This func prints the external labels list */
void printExternalsList(externalP h)
{	
	printf("\n");

    while (h)
    {
        printf("\nname: %s\t addressOfUse: %d \n", h->name, h->address);
        
        h = h->next;
    }

    printf("End of externals list\n");
}




/* This func frees the allocated memory for the external labels list */
void freeExternalsList(externalP * hptr)
{
	externalP ptr;
	
	while(*hptr)
	{
		ptr = *hptr;
		*hptr = (*hptr)->next;
		free(ptr);
	}
	
	return;
}




/* This func check if the externals list is empty or not, if empty returns FALSE otherwise returns TRUE */
boolean isExternalLabelExists(externalP h)
{
	if(h) /* In case that h != NULL at least one external label exists */
		return TRUE;
		
	return FALSE;
}
