/* Avshalom Madmoni - 203245097 */


/* Libraries */
#include <stdlib.h>
#include <stdio.h>

/* Headers */
#include "programPrototypes.h"
#include "programExternalVars.h"



/* This func inserts ARE bits into a given word */
unsigned int insertAreBitsToWord(unsigned int word, int encodingType)
{
	unsigned int result;
    
    result = (word << TOTAL_ARE_BITS); /* Inserting 00 from the 'right side' */
    result |= encodingType; /* Updating 00 with the relevent encodingType */
    
    return result;
}




/* This func gets the 5 necessary bits from a given word (number)
If leftBits=TRUE we will take the 5 bits from the left side, Otherwise from the right side.
And returns the relevant bits ready for conversion to base32  */
unsigned int getFiveBits(unsigned int word, boolean leftBits)
{
    unsigned int bits;
    unsigned int mask = (unsigned int)MASK_5_BITS; /*0000011111*/
	
	if(leftBits)
    {	
		mask <<= MOVE_5_BITS; /*1111100000*/
    	bits = mask & word; /* "Turnning off" irrelevant bits */
	 	bits >>= MOVE_5_BITS; /* Moving the relevent bits to the "beginning" */
    }
	
	else
		bits = mask & word; /* "Turnning off" irrelevant bits */
    
	return bits;
}




/* This func gets the 2 necessary addressing mode bits from a given word (number),
source or destination bits */
unsigned int getAddressingModeBits(unsigned int word, boolean sourceBits)
{
    unsigned int bits;
    unsigned int mask = (unsigned int)MASK_2_BITS; /*0000000011*/
	
	if(sourceBits)
    {	
		mask <<= MOVE_4_BITS; /*0000110000*/
    	bits = mask & word; /* "Turnning off" irrelevant bits */
	 	bits >>= MOVE_4_BITS; /* Moving the relevent bits to the "beginning" */
    }
	
	else
	{
		mask <<= MOVE_2_BITS; /*0000001100*/
    	bits = mask & word; /* "Turnning off" irrelevant bits */
	 	bits >>= MOVE_2_BITS; /* Moving the relevent bits to the "beginning" */
	}	
    
	return bits;
}




/* This func converts a given word to a string in base 32 and returns a pointer to the string */
char * convertWordToBase32(unsigned int word)
{	
    char * base32Word;
	
	/* Memory allocation */
	base32Word = (char *) malloc(BASE32_WORD_LENGTH);
	
	/* Checking memory allocation */
	if(!base32Word)
	{
		printf("\nMemory allocation failed!\n");
		return NULL;
	}
	
    /* Converting 5 bits in binary to 1 symbol in base32 */
    base32Word[0] = base32[getFiveBits(word, TRUE)]; /* Left symbol in base32 */
    base32Word[1] = base32[getFiveBits(word, FALSE)]; /* Right symbol in base32 */
    base32Word[2] = '\0'; /* End of String */

    return base32Word;
}
