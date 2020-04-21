#include "globals.h"
#include "util.h"
#include "scan.h"

/* states in scanner DFA */
typedef enum
   { START,INLESS,INEQ,INGREATER,INNEQ,ENTERCOMMENT,
     INCOMMENT,EXITCOMMENT,INNUM,INID,DONE }
   StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_Flag = FALSE; /* kludge for EOF without newline */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void)
{ if (!(linepos < bufsize))
  { if (fgets(lineBuf,BUFLEN-1,source))
    { lineno++;
      bufsize = strlen(lineBuf);
      if (EchoSource)
      { fprintf(listing,"%4d: %s",lineno,lineBuf);
        /* The following line makes sure a newline gets printed
           even if it is not in the original line -- this occurs
           only on EOF without newline */
        if (lineBuf[bufsize-1] != '\n') fprintf(listing,"\n");
      }
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { EOF_Flag = TRUE;
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{ if (!EOF_Flag) linepos-- ;}

/* lookup table of reserved words */
/* Alphabetic order for binary search */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"else",ELSE},{"if",IF},{"int",INT},
      {"return",RETURN},{"void",VOID},{"while",WHILE}
     };

/* lookup an identifier to see if it is a reserved word */
/* uses binary search */
static TokenType reservedLookup (char * s)
{ 
 /*{ int min = 0;
  int max = 5;

 while (max >= min)
  {
	int mid = (min + max) >> 1;
	if(strcmp(s,reservedWords[mid].str) == 0)
	{
		return reservedWords[mid].tok;
	}
	if(strcmp(s,reservedWords[mid].str) < 0)
	{
		max = mid - 1;
	}
	if(strcmp(s,reservedWords[mid].str) > 0)
	{
		min = mid + 1;
	}
  }
  return ID;
}
 */ 
   int i;
   int found = FALSE;
   int lo = 0 , hi = MAXRESERVED-1;
   while( (hi >= lo) && !found )
   { i = (hi + lo)/2;
     if(strcmp(s, reservedWords[i].str)<0)
       hi = i-1;
     else if (strcmp(s,reservedWords[i].str)>0)
       lo = i+1;
     else found = TRUE;
   }
   if (found) return reservedWords[i].tok;
   else return ID;


}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   /* holds current token to be returned */
   TokenType currentToken;
   /* current state - always begins at START */
   StateType state = START;
   /* flag to indicate save to tokenString */
   /* FALSE case is most common -- set to TRUE
      below if ID or NUM or ERROR */
   int save = FALSE;
   while (state != DONE)
   { int c = getNextChar();
     switch (state)
     { case START:
         /* order of tests reorganized a bit for better
            efficiency */
         if ((c == ' ') || (c == '\t') || (c == '\n'))
           /* skip */;
         else if (isdigit(c))
         { save = TRUE;
           state = INNUM;
         }
         else if (isalpha(c))
         {
 	     save = TRUE;
	     state = INID;
			
         }
         else switch (c)  /* do the cases for different "functions"*/
         { case '<':
             state = INLESS;
             break;
	   case '=':
	     state = INEQ;
	     break;
	   case '>':
	     state = INGREATER;
	     break;
	   case '!':
	     state = INNEQ;
	     break;
	   case '/':
	     state = ENTERCOMMENT;
	     break;
	   default:
             state = DONE;
             switch (c)   /* Token switch case*/
             { case EOF:
                 currentToken = ENDFILE;
                 break;
               case '+':
		 currentToken = PLUS;
		 break;
               case '-':
		 currentToken = MINUS;
		 break;
               case '*':
		 currentToken = TIMES;
		 break;
               case '/':
		 currentToken = OVER;
		 break;
               case ';':
		 currentToken = SEMI;
		 break;
               case ',':
		 currentToken = COMMA;
		 break;
               case '(':
		 currentToken = LPAREN;
		 break;
               case ')':
		 currentToken = RPAREN;
		 break;
               case '[':
		 currentToken = LBRACKET;
		 break;
               case ']':
		 currentToken = RBRACKET;
		 break;
               case '{':
		 currentToken = LCURLY;
		 break;
               case '}':
		 currentToken = RCURLY;
		 break;
					 
             default:
                 currentToken = ERROR;
                 save = TRUE;
                 break;
             }
             break;
         }
         break;
       case ENTERCOMMENT:
         if (c == '*') state = INCOMMENT;
         else
         { state = DONE;
           /* backup in the input */
           ungetNextChar();
	   currentToken = OVER;
	}
         break;
       case INCOMMENT:
         if (c == '*')
	 state = EXITCOMMENT;
         else if (c == EOF)
         { state = DONE;
           currentToken = ENDFILE;
         }
         break;
       case EXITCOMMENT:
         if (c == '/') state = START;
         else if(c != '*') state = INCOMMENT;		
         break;
       case INLESS:
         state = DONE;
         if (c == '=')
           currentToken = LE;
         else
         { /* backup in the input */
           ungetNextChar();
           currentToken = LT;
         }
         break;
       case INEQ:
         state = DONE;
	 if(c == '=')
	    currentToken = EQ;
         else
	 { /* backup in the input */
           ungetNextChar();
           currentToken = ASSIGN;
         }
	 
	 break;
       case INGREATER:
         state = DONE;
         if (c == '=')
           currentToken = GE;
         else
         { /* backup in the input */
           ungetNextChar();
           currentToken = GT;
         }		     
			
         break;
       case INNEQ:
         state = DONE;
         if (c == '=')
           currentToken = NE;
			else
         { /* backup in the input */
           ungetNextChar();
           currentToken = ERROR;
         }
         break;
       case INNUM:
         if (!isdigit(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
       case INID:
         if (!isalpha(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID; 
         }
         break;
		 
       case DONE:
       default: /* should never happen */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

