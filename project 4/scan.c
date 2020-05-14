
/*************************************************************/
/*   File: scan.c                                            */
/*   The scanner implementation for the C-Minus compiler     */
/*   The Scanner goes through the programs and collects all  */
/*   the tokens for the parser.                              */
/*   Date: 5/14/20                                           */
/*   Dhaval Patel , Jason Giroux and Jeffrey Chesanek        */
/*   dpatel657@g.rwu.edu                                     */
/*   jgiroux776@g.rwu.edu                                    */
/*   jchesanek513@g.rwu.edu                                  */
/*   Last Edited 03/06/2020                                  */
/*   COMSC 440 Compiler Design                               */
/*                                                           */
/*************************************************************/

#include "global.h"
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
 //****************************
 //add code here
 //load your binary search algorithm here 
  int low = 0;
  int high = 5;

  while(high >= low) {
    int middle = (low + high) >> 1;
    if(strcmp(s, reservedWords[middle].str) == 0) {
      return reservedWords[middle].tok;
    }
    if(strcmp(s,reservedWords[middle].str) < 0) {
      high = middle - 1;
    }
    if(strcmp(s,reservedWords[middle].str) > 0) {
      low = middle + 1;
    }
  }
  return ID;
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
         /*skip*/;
         else if (isdigit(c))
         { save = TRUE;
           state = INNUM;
         }
         else if (isalpha(c))
         {
           save = TRUE;
           state = INID;  //Add code here
      
         }
         else switch (c)
         {  case '<':
             state = INLESS;
             break;
            case '=':
              state = INEQ;
              break;
            case '>':
              state = INGREATER;           //add code here
              break;
            case '!':
              state = INNEQ;
              break;
            case '/':
              state = ENTERCOMMENT;
              break;   
      
            default:
              state = DONE;
  
            switch (c)
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
                 currentToken = OVER;                  //Add code here
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
               case ';':
                 currentToken = SEMI;
                 break;
         case ',':
          currentToken = COMMA;
          break;
         case '!':
          currentToken = NE;
         break;

        default:
                 save = TRUE;
                 currentToken = ERROR;
                 state = START;
                 
                 break;
             }break;
         }break;
       case ENTERCOMMENT:
         if (c == '*'){
          state = INCOMMENT;
        }
         else
         { state = DONE;
           /* backup in the input */
           ungetNextChar();  //Add code here
           currentToken = OVER;
         }
         break;
       case INCOMMENT:
         if (c == '*')
          state = EXITCOMMENT;   //Add code here 
         else if (c == EOF)
         { state = ERROR;
           currentToken = ENDFILE;
         }
         break;
       case EXITCOMMENT:
         if (c == '/') {
          state = START;
          }             //Add code here
       else if (c == EOF)
         { state = DONE;
           currentToken = ENDFILE;
         }
         else{
          state = INCOMMENT;
          ungetNextChar();
         }
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
         if (c == '=')
           currentToken = EQ;    //Add code here
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
           currentToken = GT;   //Add code here       
   }
         break;
       case INNEQ:
         state = DONE;
         if (c == '=')
          // Add code here
          currentToken = NE;
         else
         { /* backup in the input */
           ungetNextChar();
         save = TRUE;
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
         { /* backup in the input */      //Add code here
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
