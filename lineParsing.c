/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Headers */
#include "programPrototypes.h"
#include "programData.h"



/* This func checks if a given line(string) is an empty line */
boolean isEmptyLine(char * line)
{	
	if(line == NULL) /* Checking if "empty" */
		return TRUE;

	line = skipSpaces(line); /* Skipping whitespaces */
	
	if(*line == '\n' || *line == '\0')
		return TRUE;
	
	return FALSE;
}




/* This func checks if a given line(string) is a comment line */
boolean isCommentLine(char * line)
{	
	if(line == NULL) /* Checking if "empty" */
		return FALSE;

	line = skipSpaces(line); /* Skipping whitespaces */
	
	if(*line == ';')
		return TRUE;
	
	return FALSE;
}




/* This func checks if the first non blank char in a line is valid */
boolean isFirstCharValid(char * line)
{
	line = skipSpaces(line);
	
	if(*line == '.' || isalpha(*line)) /* Checks if the char is a '.' or a letter */
		return TRUE;
	
	return FALSE;
}




/* This func skips whitespaces and copies to dest all the characters until the end of the line or ',' is reached,
If the pointer points to ',' in the current line, we copy it into dest. Then returns a pointer to the next char */
char * copyUntilComma(char * dest, char * line)
{
    char * ptr = dest;

    line = skipSpaces(line); /* Skip whitespaces */
    
    /* Checking if we have reached the end of the line or string return NULL*/
    if(isEndOfLine(line)) 
    {
    	*dest = '\0';
    	return NULL;
    }
	
	/* If the pointer is now pointing to ',' copy the ',' to dest move the pointer to the next char and return the pointer */
    if(*line == ',')
    {
        strcpy(dest, ","); /* Copy comma into dest */
        line++;
        return line; /* Returns a pointer to the next char */
    }
	
	/* Copy to dest until we reach whitespace, end of the line or ',' */
    while(!isEndOfLine(line) && !isspace(*line) && *line != ',')
    {
        *ptr = *line;
        line++;
        ptr++;
    }
    
    *ptr = '\0'; /* End string */

    return line; /* Returns a pointer to the next char */
}




/* This func skips whitespaces and copies to a given note all the characters from 'op' until the end of the line or '.' is reached,
Then returns a pointer to the next char */
char * copyUntilDot(char * dest, char * op)
{
    char * ptr = dest;
	
	op = skipSpaces(op); /* Skip whitespaces */	

	/* Copy to dest until we reach '.' */
    while(!isEndOfLine(op) && *op != '.')
    {
        *ptr = *op;
        op++;
        ptr++;
    }
    
    *ptr = '\0'; /* End string */

    return op; /* Returns a pointer to '.' */
}




/* This func skips whitespaces and returns a pointer to the first non-blank character if it exists, Otherwise returns NULL */
char * skipSpaces(char * ch)
{
    if(ch == NULL) /* Checking if "empty" */
    	return NULL;
    
    while(isspace(*ch)) /* Skipping whitespaces */
        ch++;
    
    return ch; /* Pointer to the next char */
}




/* This func copies chars from source to destination until it reaches a space, end of line, or end of string */
void copyNote(char * destination, char * source)
{
    int i = 0; /* Index counter */
    
    if(destination == NULL || source == NULL) /* Checking if "empty" */
    	return;

    while(i < LINE_LENGTH && !isspace(source[i]) && source[i] != '\0') /* Copies note from a given line/string into dest */
    {
        destination[i] = source[i]; /* Inserting chars */
        i++; /* Move */
    }
    
    destination[i] = '\0'; /* End of String */
}




/* This func skips the current note and returns a pointer to the beginning of the next note in the line */
char * nextNote(char * line)
{
	if(line == NULL) /* Checking if "empty" */
		return NULL;
    
	while(!isEndOfLine(line) && !isspace(*line)) /* Skip current note if exists */
    	line++; 
    
    line = skipSpaces(line); /* Move to next char */
    
    if(isEndOfLine(line)) /* If no more notes in line */
    	return NULL;
    
    return line; /* Pointer to the next note in line */
}




/* This func checks if we have reached the end of the line or the end of a given string */
boolean isEndOfLine(char * line)
{	
	if(line == NULL) /* Checking if "empty" */
		return TRUE;	
	
    if(*line == '\n' || *line == '\0') /* End of line/string */
    	return TRUE;
    
    return FALSE; 
}
