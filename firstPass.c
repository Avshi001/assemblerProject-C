/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Headers */
#include "programExternalVars.h"
#include "assemblerPrototypes.h"
#include "assemblerStructData.h"
#include "programData.h"
#include "programPrototypes.h"



/* This func handles the entire first pass */
void firstPass(FILE * fp)
{
    /* Will contain the current line */
    char line[LINE_LENGTH];
    
    /* Lines counter*/
    int lineNumber = 1; 
	
    /* Instructions & Data counters */
    IC = 0;
    DC = 0;
	
	/* Get lines until end of file */
    while(fgets(line, LINE_LENGTH, fp)) 
    {
        errorHolder = NO_ERROR; /* Reset errorHolder before parsing each line */
        
        if(!isEmptyLine(line) && !isCommentLine(line))	 
            firstPassLineParser(line); /* Line parser */

        /* If there was an error during the program, we will print the errors */
        if(errorHolder != NO_ERROR)
        {
            errorExists = TRUE; /* Global var */
            writeError(lineNumber); 
        }
        
        lineNumber++; /* Add 1 to lines counter */
    }
    
    /* Updates the address of each label */
    updateLabelAddress(labelsList,IC);

    return;
}




/* This func parses a given line, checks for optional label, instruction line or directive line */
void firstPassLineParser(char * line)
{	
	/* Vars */
	char curr [LINE_LENGTH];
	int commandType;
	int directiveType;
	labelP ptr;
	boolean colonFlag = FALSE;
	
	line = skipSpaces(line); /* Skips spaces */
	if(isEndOfLine(line)) /* Empty line */
		return;
	
	if(!isFirstCharValid(line)) 
	{
		errorHolder = LINE_INVALID_FIRST_CHAR;
		return;
	}
	
	copyNote(curr, line); /* Copy note into curr */
	
	/* Checking if the first note in line is command or directive */
	commandType = checkIfCommand(curr);
	directiveType = checkIfDirective(curr);
	
	/* In case that it is not command or directive it is must be label declaration */
	if(commandType == NOT_FOUND && directiveType == NOT_FOUND)
	{	
		/* Checking if the first note in line is a label declaration */
		if(isLabelDeclaration(curr, &colonFlag))
		{				
			/* Adding the label to the labels list */
			ptr = addLabelToList(&labelsList, curr);
		
			if(!ptr) /* If ptr==NULL label already exists */
			{
				ptr = getLabel(labelsList, curr); /* Get label */
		
				if(!(ptr->external)) /* Checking if the label is External */
					errorHolder = LABEL_NAME_ALREADY_EXISTS;

				else
					errorHolder = LABEL_CAN_NOT_BE_ENTRY_AND_EXTERN;

				return;
			}
			
			line = nextNote(line); /* Move to next note */
		
			/* We must have another note after label declaration */
			if(isEndOfLine(line))
			{
				errorHolder = LABEL_DECLARATION_LINE_ONLY_LABEL;
				return;
			}
		
			copyNote(curr, line); /* Copy note into curr */

			/* Checking if curr is command */
			if((commandType = checkIfCommand(curr)) != NOT_FOUND)
			{
				/* Updating relevant info */
				ptr->address = IC;
				ptr->usedInCommandLine = TRUE;
		
				line = nextNote(line); /* Move to next note */
				commandsProcessor(commandType, line); /* Process command */
			}

			/* Checking if curr is directive */
			else if((directiveType = checkIfDirective(curr)) != NOT_FOUND)
			{
				if(directiveType == ENTRY || directiveType == EXTERN)	
					deleteLabelFromList(&labelsList, ptr->name); /* Delete label */
			
				else /* Updating relevant info */
					ptr->address = DC;
		
				line = nextNote(line); /* Move to next note */
				directivesProcessor(directiveType, line); /* Process directive */
			}
			
			else /* Note after label declaration is not command or directive */
			{	
				errorHolder = LABEL_DECLARATION_LINE_INVALID;
				return;
			}
		}
		
		/* A line must start with command, directive or label declaration */
		else
		{	
			if(!colonFlag)/* If colonFlag == FALSE this line is without command, directive or label declaration */
				errorHolder = LINE_INVALID;
			return;
		}
		
	}
	
	/* If first note in line is command or directive  */
	else
	{
		if(commandType != NOT_FOUND)
		{
			line = nextNote(line); /* Move to next note */
			commandsProcessor(commandType, line); /* Process command */
		}

		else if(directiveType != NOT_FOUND)
		{
			line = nextNote(line); /* Move to next note */
			directivesProcessor(directiveType, line); /* Process directive */
		}
	}
	
	return;
}




/* This func parses a command line by command type and encode first word into instructions array */
void commandsProcessor(int commandType, char * line)
{	
	/* Vars */
	char firstOp[LINE_LENGTH]; /* Will contain the first operand if exists */
	char secondOp[LINE_LENGTH]; /* Will contain the second operand if exists */
	
	int firstOpAddressingMode = NOT_FOUND;
	int secondOpAddressingMode = NOT_FOUND;
	
	boolean firstOpFlag = FALSE;
	boolean secondOpFlag = FALSE;
	boolean commaFlag = FALSE;
	
	unsigned int word = 0 ;
	
	/* Copy first opreand to firstOp */
	line = copyUntilComma(firstOp,line);
	
	if(*firstOp != '\0') /* If *firstOp == '\0' we have reached the end of line */
	{
		if(*firstOp == ',') /* Checking for the first char in line */
		{
			errorHolder = COMMAND_FIRST_CHAR_COMMA;
			return;
		}
		
		else
			firstOpFlag = TRUE;

		line = copyUntilComma(secondOp,line); /* Copy into secondOp next note */
		
		if(*secondOp != '\0') /* If *secondOp == '\0' we have reached the end of line. only 1 operand */
		{	
			/* If we have reached this point there is at least 2 "operands" in line */
			if(!CommandNumOfOperandsValid(commandType, TRUE, TRUE) && *secondOp != ',') 
			{														/* Number of operands first check */
				errorHolder = COMMAND_INVALID_NUM_OF_OPERANDS;
       			return;
			}
			
			/* In case that we did not reach the end of line next char may be ',' */
			if(*secondOp == ',')
				commaFlag = TRUE;
			
			else /* Comma must exists between 2 operands */
			{
				errorHolder = COMMAND_NO_COMMA_BETWEEN_OPERANDS;
				return;
			}
		}
		
		/* If we found comma after first operand we must have a second operand */
		if(commaFlag)
		{
			line = copyUntilComma(secondOp,line); /* Copy into secondOp next note */
			
			if(*secondOp == '\0') /* If *secondOp == '\0' we have reached the end of line. only 1 operand */
			{
				errorHolder = COMMAND_NO_OPERAND_AFTER_COMMA;
				return;
			}
			
			if(*secondOp == ',') /* Checking for commas in a row */
			{
				errorHolder = COMMAND_COMMAS_IN_A_ROW;
				return;
			}
			
			line = skipSpaces(line); /* Skipping spaces */
			
			/* If line points to a non-blank char we have too many operands */
			if(!isEndOfLine(line)) 
			{
				errorHolder = COMMAND_TOO_MANY_OPERANDS;
				return;
			}
			
			secondOpFlag = TRUE;	
		}
	}
	
	/* Checking for addressing modes  */
	if(firstOpFlag)
		firstOpAddressingMode = findAddressingMode(firstOp);
	
	if(secondOpFlag)
		secondOpAddressingMode = findAddressingMode(secondOp);	

	/* Checking if the number and the addressing mode of a given operands is valid */
	if(!CommandNumOfOperandsValid(commandType, firstOpFlag, secondOpFlag))
	{													/* Number of operands second check */
		errorHolder = COMMAND_INVALID_NUM_OF_OPERANDS;
        return;
	}
		
	if(!CommandAddressingModeValid(commandType, firstOpAddressingMode, secondOpAddressingMode))
	{
		errorHolder = COMMAND_INVALID_OPERANDS_ADDRESSING_MODE;
        return;
	}
		
	/* encode first word of the command to memory and increase IC by the number of additional words */
	word = encodeFirstWord(commandType, firstOpFlag, firstOpAddressingMode, secondOpFlag, secondOpAddressingMode); /* Encode first word */
			
	insertWordToInstructions(word); /* Insert first word to instructions array */
			
	IC += commandAdditionalWords(firstOpFlag, firstOpAddressingMode, secondOpFlag, secondOpAddressingMode); /* Increase IC by command additional words */
			
	return;
}




/* This func calculates the number of additional words for command line*/
int commandAdditionalWords(boolean firstOp, int addresingModeFirstOp, boolean secondOp, int addresingModeSecondOp)
{
    int totalWords = 0;
    
    /* For any addressing mode except Struct we need to add 1 word*/
    if(firstOp)
    	totalWords += ((addresingModeFirstOp != STRUCT_M) ? 1:2); 
    if(secondOp)
    	totalWords += ((addresingModeSecondOp != STRUCT_M) ? 1:2);

    /* In Case that both addressing modes are register we need to add only 1 word*/
    if( (firstOp && addresingModeFirstOp == REGISTER_M) && (secondOp && addresingModeSecondOp == REGISTER_M) )
    	totalWords--;

    return totalWords;
}




/* This func checks if the number of opeands is valid according to the command type */
boolean CommandNumOfOperandsValid(int commandType, boolean firstOp , boolean secondOp)
{
	switch(commandType)
	{
        /* accept 2 operands */
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
        
            if(firstOp && secondOp)
            	return TRUE;
            
            return FALSE;
		
		/* accept 1 operand */
        case NOT:
        case CLR:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case GET:
        case PRN:
        case JSR:
        
            if(firstOp && !secondOp)
            	return TRUE;
            
            return FALSE;
		
		/* accept 0 operands */
        case RTS:
        case HLT:
        
            if(!firstOp && !secondOp)
            	return TRUE;
            
            return FALSE;
    }
    
    return FALSE;
}




/* This func checks if the given addressing modes are valid according to the command type */
boolean CommandAddressingModeValid(int commandType, int firstOpAddressMode, int secondOpAddressMode)
{	

	/* In case that command type accept 2 operands 'firsoOpAddressMode' will represent source addreesing mode and 'secondOpAddressMode' the destination
	 addreesing mode, if command type accept only 1 operand 'firsoOpAddressMode' will represent destination addreesing mode */
		
	switch(commandType)
	{
		case CMP:
			if( (firstOpAddressMode == IMMEDIATE_M || firstOpAddressMode == DIRECT_M || firstOpAddressMode == STRUCT_M || firstOpAddressMode == REGISTER_M)
																		&&
			    (secondOpAddressMode == IMMEDIATE_M || secondOpAddressMode == DIRECT_M || secondOpAddressMode == STRUCT_M || secondOpAddressMode == REGISTER_M) )
				return TRUE;
			
			return FALSE;
		
		case MOV:
		case ADD:
		case SUB:
			if( (firstOpAddressMode == IMMEDIATE_M || firstOpAddressMode == DIRECT_M || firstOpAddressMode == STRUCT_M || firstOpAddressMode == REGISTER_M)
																		&&
			    			(secondOpAddressMode == DIRECT_M || secondOpAddressMode == STRUCT_M || secondOpAddressMode == REGISTER_M) )
				return TRUE;
			
			return FALSE;
		
		case LEA:
			if( 				(firstOpAddressMode == DIRECT_M || firstOpAddressMode == STRUCT_M)
															&&							
				(secondOpAddressMode == DIRECT_M || secondOpAddressMode == STRUCT_M || secondOpAddressMode == REGISTER_M) )
				return TRUE;
			
			return FALSE;
		
		case PRN:/* Only 1 operand. this is why we are checking firstOpAddressMode */
			if(firstOpAddressMode == IMMEDIATE_M || firstOpAddressMode == DIRECT_M || firstOpAddressMode == STRUCT_M || firstOpAddressMode == REGISTER_M)
				return TRUE;
			
			return FALSE;
		
		case NOT:
		case CLR:
		case INC:
		case DEC:
		case JMP:
		case BNE:
		case GET:
		case JSR:/* Only 1 operand. this is why we are checking firstOpAddressMode */
			if(firstOpAddressMode == DIRECT_M || firstOpAddressMode == STRUCT_M || firstOpAddressMode == REGISTER_M)
				return TRUE;
			
			return FALSE;
		
		case RTS:/* Irrelevant */
		case HLT:
			return TRUE; 
	}
	
	return FALSE;
}





/* This func parses a dircetive line by directive type, and running the relevant func */
void directivesProcessor(int directiveType, char * line)
{
	if(line ==NULL || isEndOfLine(line)) /* Must be a parameter after directive declaration */
	{	
		errorHolder = DIRECTIVE_WITHOUT_PARAMETERS;
		return;
	}
	
	/* Running the relevant func by directive type */
	switch(directiveType)
	{
		case DATA:
			dataDirectiveProcessor(line);
			break;
		
		case STRING:
			stringDirectiveProcessor(line);
			break;
		
		case STRUCT:
			structDirectiveProcessor(line);
			break;			

		case ENTRY:
			entryDirectiveProcessor(line);
			break;
		
		case EXTERN:
			externDirectiveProcessor(line);
			break;
	}
	
	return;
}




/* This func parses a data directive line and encode the integers into data array */
void dataDirectiveProcessor(char * line)
{	
	/* Vars */
	char curr[LINE_LENGTH];
	int currNum;
	
	boolean integerFlag = FALSE;
	boolean commaFlag = FALSE;
	
	/* Checking if the first non-blank in line after data is comma */
	if(*line == ',')
	{
		errorHolder = DATA_FIRST_CHAR_COMMA;
		return;
	}
	
	/* As long as we haven't reached the end of line continue */
	while(!isEndOfLine(line))
	{
		line = copyUntilComma(curr,line); /* Copy parameter into curr */
		
		/* If integerFlag = FALSE we are expecting for an integer */
		if(!integerFlag) 
		{
			
			if(!isInteger(curr)) /* Checking if curr is an integer */
			{
				errorHolder = DATA_INVALID_PARAMETER;
				return;
			}
			
			/* Updating flags and inserting the integer to the data array */	
			else
			{	
				integerFlag = TRUE;
				commaFlag = FALSE;
				currNum = atoi(curr); /* convert string to integer */
				insertNumToData(currNum); /* Inserting the num to data array */
			}
		}
			
		else if(*curr == ',') /* Checking if curr is ',' */
		{
			if(!commaFlag) /* Updating flags */
			{
				integerFlag = FALSE;
				commaFlag = TRUE;
			}
			
			/* In case that commaFlag = TRUE & curr is ','  */	
			else 
			{
				errorHolder = DATA_COMMAS_IN_A_ROW;
				return;
			}
		}
		
		/* We expected a comma between the integers in we could not find */	
		else
		{	if(isInteger(curr))
			{
				errorHolder = DATA_NO_COMMA_BETWEEN_NUMBERS;
				return;
			}
		}
		
	}
	
	/* Checking for the last non-blank char in the line */
	if(commaFlag)
	{
		errorHolder = DATA_LAST_PARAMETER_COMMA;
		return;
	}
	
	return;
}




/* This func parses a string directive line and encode the relevant chars + an extra '\0' into data array */
void stringDirectiveProcessor(char * line)
{	
	/* Vars */
	char curr[LINE_LENGTH];
	int currLength;
	
	copyNote(curr,line); /* Copy note to curr */
	
	/* Checking that it is a valid string */
	if(!isString(curr))
	{
		errorHolder = STRING_INVALID;
		return;
	}
	
	line = nextNote(line); /* Move to the next note in line */
	
	/* If we haven't reached the end of the line there is an invalid number of parameters  */
	if(!isEndOfLine(line))
	{
		errorHolder = STRING_TOO_MANY_PARAMETERS;
		return;
	}
	
	/* Delete the quotation marks from the original string */
	currLength = strlen(curr);
	curr[currLength-1] = '\0';
	insertStringToData(curr+1);
	
	return;
}




/* This func parses a struct directive line and encode the relevant fileds into data array */
void structDirectiveProcessor(char * line)
{	
	/* Vars */
	char curr[LINE_LENGTH];
	char next[LINE_LENGTH];
	int nextLength;
	int currNum;
	
	/* Checking if the first non-blank in line after data is comma */
	if(*line == ',')
	{
		errorHolder = STRUCT_FIRST_CHAR_COMMA;
		return;
	}
	
	line = copyUntilComma(curr, line); /* Copy parameter into curr */
		
	if(!isInteger(curr)) /* Checking if curr is an integer */
	{
		errorHolder = STRUCT_FIRST_PARAMETER_MUST_BE_NUMBER;
		return;
	}
	
	currNum = atoi(curr); /* Convert string to integer and put it into currNum */
	line = copyUntilComma(curr,line); /* Copy parameter into curr */
	line = skipSpaces(line); /* Skipping spaces */
	
	/* If we had a comma before and we have reached the end of line */
	if(isEndOfLine(line) && *curr == '\0')
	{
		errorHolder = STRUCT_MISSING_SECOND_PARAMETER;
		return;
	}

	if(*line == ',' && *curr == ',')
	{
		errorHolder = STRUCT_COMMAS_IN_A_ROW;
		return;
	}

	line = copyUntilComma(next,line); /* Copy next parameter into next */

	/* Checking if the second parameter is a valid string */
	if(!isString(next))
	{
		errorHolder = STRUCT_SECOND_PARAMETER_MUST_BE_STRING;
		return;
	}
	
	if(*curr != ',') /* Checking if curr is ',' */
	{	
		/* We expecting a comma between the two parameters */	
		errorHolder = STRUCT_MISSING_COMMA_BETWEEN_PARAMETERS;
		return;
	}
	
	/*line = copyUntilComma(curr,line);  Copy parameter into curr */
	line = skipSpaces(line); /* Skipping spaces */
	
	/* If we haven't reached the end of the line there is an invalid number of parameters  */
	if(!isEndOfLine(line))
	{
		errorHolder = STRUCT_TOO_MANY_PARAMETERS;
		return;
	}
	
	
	
	/* Delete the quotation marks from the original string and put it into curr */
	nextLength = strlen(next);
	next[nextLength-1] = '\0';
	
	/* Inserting struct fileds to data array */
	insertStructToData(currNum, next+1);
	
	return;
}




/* This func parses an entry directive line and checks for errors */
void entryDirectiveProcessor(char * line)
{
	char curr[LINE_LENGTH];
	
	copyNote(curr,line); /* Copy note into curr */
	
	if(!isValidLabel(curr, TRUE, FALSE)) /* Checking if it is a valid label name */
		return;
	
	line = nextNote(line); /* Moving to the next note in line */
	
	/* If we have not reached the end of line we have too many parameters */
	if(!isEndOfLine(line))
	{
		errorHolder = ENTRY_TOO_MANY_PARAMETERS;
		return;
	}
	
	return;
}




/* This func parses an extern directive line, checks for errors and adding the labels to labels list */
void externDirectiveProcessor(char * line)
{	
	/* Vars */
	char curr[LINE_LENGTH];
	labelP ptr;
	
	copyNote(curr,line); /* Copy rest of line after extern declaration into curr */
	
	/* Checking if label name is valid */
	if(!isValidLabel(curr, TRUE, FALSE))
		return;
	
	line = nextNote(line); /* Move forward in line */
	
	/* If we have not reached the end of line we have too many parameters */
	if(!isEndOfLine(line))
	{
		errorHolder = EXTERN_TOO_MANY_PARAMETERS;
		return;
	}
	
	/* Add label to labels list */
	ptr = addLabelToList(&labelsList, curr);
	
	if(!ptr) /* If ptr==NULL label already exists */
	{
		ptr = getLabel(labelsList, curr);
		
		if(ptr->external) /* Checking if the label is External */
			errorHolder = LABEL_NAME_ALREADY_EXISTS;

		else
			errorHolder = LABEL_CAN_NOT_BE_ENTRY_AND_EXTERN;

		return; 
	}
	
	ptr->external = TRUE; /* Update external field in label to TRUE */
	
	return;
}




/* This func checks if the given string is a valid label declaration name */
boolean isLabelDeclaration(char * string, boolean * colonFlag)
{
	/* Vars */
	int stringLength = strlen(string);
	
	if(string == NULL) /* "Empty" */
		return FALSE;
	
	if(string[stringLength-1] != ':') /* Label declaration must end with ':' */
	{	
		errorHolder = LABEL_DECLARATION_MUST_END_WITH_A_COLON;
		return FALSE;
	}

	*colonFlag = TRUE; /* If the last char in the string is ':' colonFlag == TRUE */
		
	/* Label declaration length must be between 2-31 (including colon) */
	if((stringLength < MIN_LABEL_DECLARATION_LENGTH) || (stringLength > MAX_LABEL_DECLARATION_LENGTH))
	{	
		errorHolder = LABEL_DECLARATION_INVALID_LENGTH;
		return FALSE;
	}
	
	/* Send string without ':' to the next func */
	string[stringLength-1] = '\0'; 
	
	/* This func checks if the label name without ':' is valid */
	if(!isValidLabel(string, TRUE, TRUE)) 
		return FALSE;
	
	return TRUE;
}




/* This function encodes the first word of the command */
unsigned int encodeFirstWord(int commandType, boolean firstOp, int firstOpMode, boolean secondOp, int secondOpMode)
{
    unsigned int word = 0;
	
	word = commandType; /* Command bits */
	word <<= TOTAL_ADDRESSING_MODE_BITS; /* Inserting to 00 from right to left */
	
	/* When only 2 operands exists the first is source and the second is destination */
	if(firstOp && secondOp)
	{
		word |= firstOpMode; /* Updating source */
		word <<= TOTAL_ADDRESSING_MODE_BITS; /* Inserting to 00 from right to left */
		word |= secondOpMode; /* Updating destination */
	}
	
	/* When only 1 operand exists that is destination */
	else if(firstOp)
	{
		word <<= TOTAL_ADDRESSING_MODE_BITS; /* Inserting to 00 from right to left */
		word |= firstOpMode; /* Updating destination */
	}
	
	else
		word <<= TOTAL_ADDRESSING_MODE_BITS; /* Inserting to 00 from right to left */
	
	word <<= TOTAL_ARE_BITS; /* Inserting '00' first word ARE is ABSOLUTE(00) */
	
	return word;
}




/* This func checks the addressing mode of a given operand and returns is addressing mode in decimal value */
int findAddressingMode(char * op)
{	
	/* Vars */
	char curr[LINE_LENGTH];
	int opLength = strlen(op);
	
	if(isEndOfLine(op)) /* 'Op' only blank spaces */
		return NOT_FOUND;
	
	if(*op == '#') /* IMMEDIATE - 'op' must start with '#' and then an integer */
	{
		op++;
		if(isInteger(op))
			return IMMEDIATE_M;
	}
	
	if(isRegister(op)) /* REGISTER - 'op' is a register */
		return REGISTER_M;
	
	if(isValidLabel(op, FALSE, FALSE)) /* DIRECT - 'op' is an existing label */
		return DIRECT_M;
	
	/* Copying 'op' to 'curr' without the last 2 chars */
	strcpy(curr, op);
	curr[opLength-2] = '\0'; 
	
	if(isValidLabel(curr, FALSE, FALSE)) /* Checking if 'curr' is a valid label name */
	{	
		/* Checking if the last char in 'op' is 1 or 2 and if the char before the last one is '.'  */
		if((op[opLength-1] == '1' || op[opLength-1] == '2') && op[opLength-2] == '.')
			return STRUCT_M;
	}
	
	return NOT_FOUND;
}
