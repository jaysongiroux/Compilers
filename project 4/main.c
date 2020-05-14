/*************************************************************/
/* (C) Copyright Kenneth C. Louden 1999                      */
/*                                                           */
/* Permission is granted for the use of this code by         */
/* instructors in regularly scheduled classes at a college   */
/* or university requiring my text:                          */
/*                                                           */
/* K. Louden, Compiler Construction: Principles and          */
/*  Practice, PWS Publishing Co., 1997.                      */
/*                                                           */
/* Additionally, individual purchasers of the text may use   */
/* this code for their own private, non-commercial purposes. */
/* All other reproduction, distribution, or display outside  */
/* the classroom, including posting or tranmission on the    */
/* internet, are prohibited and constitute a violation of    */
/* this copyright.                                           */
/*************************************************************/

/*************************************************************/
/*   File: main.c                                            */
/*   Main program for C-minus compiler                       */
/*   Version: 4                                              */
/*   Authors: Jason Giroux, Jeffrey Chesanek, Dhaval Patel   */
/*   jgiroux776@g.rwu.edu,jchesanek513@g.rwu.edu,            */
/*   dpatel657@g.rwu.edu                                     */
/*   Roger Williams University		                           */
/*   COMSC 440 Language Translation and Compiler Design      */
/*************************************************************/

#include "global.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE //Set To TRUE

/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE TRUE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = TRUE;	//Flags on
int TraceCode = FALSE;

/* error flag to inhibit subsequent passes */
int Error = FALSE;

main( int argc, char * argv[] )
{ TreePtr syntaxTree;
  char pgm[120]; /* source code file name */
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".cm");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\n>>>>> C-MINUS COMPILATION: %s\n\n",pgm);
#if NO_PARSE
  while (getToken()!=ENDFILE);
#else
  syntaxTree = parse();
#if !NO_ANALYZE
  if (! Error)
  { buildSymtab(syntaxTree);
    typeCheck(syntaxTree);
  }
#if !NO_CODE
  if (! Error)
  { char * codefile;
    int fnlen = strcspn(pgm,".");
    codefile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".tm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("Unable to open %s\n",codefile);
      exit(1);
    }
    codeGen(syntaxTree,codefile);
    fclose(code);
  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}

