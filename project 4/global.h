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
/*   File: globals.h                                         */
/*   Global types and vars for C-Minus compiler              */
/*   must come before other include files                    */
/*   Version 4 8/1/99                                        */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/
/*   Version: 4                                              */
/*   Date: 5/14/20  						                             */  
/*   Authors: Jason Giroux, Jeffrey Chesanek, Dhaval Patel   */
/*   jgiroux776@g.rwu.edu,jchesanek513@g.rwu.edu,            */
/*   dpatel657@g.rwu.edu                                     */
/*   Roger Williams University		                           */
/*   COMSC 440 Language Translation and Compiler Design      */
/*************************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 6

typedef enum 
    /* book-keeping tokens */
   {ENDFILE,ERROR,
    /* reserved words */
    ELSE,IF,INT,RETURN,VOID,WHILE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    PLUS,MINUS,TIMES,OVER,LT,LE,GT,GE,EQ,NE,ASSIGN,
    SEMI,COMMA,LPAREN,RPAREN,LBRACKET,RBRACKET,LCURLY,RCURLY
   } TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/********   C- Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK,ExpK,DeclK} NodeKind;
typedef enum {IfK,ReturnK,WhileK,CmpdK} StmtKind;
typedef enum {OpK,ConstK,IdK,CallK,SubsK} ExpKind;
typedef enum {VarK,FunK,ParamK} DeclKind;

/* types */
typedef enum { IntType,ArrayType,IntFuncType,VoidFuncType,
               VoidType } CMType;

#define MAXCHILDREN 3

typedef struct treeNode * TreePtr;

typedef struct treeNode
{ TreePtr child[MAXCHILDREN];
  TreePtr sibling;
  int lineno;
  NodeKind nodekind;
  union { StmtKind stmt;
          ExpKind exp;
          DeclKind decl; } kind;
  union { TokenType op;
          int val;
          char * name; } attr;
/* additional C- attributes for declarations */
  TokenType type; /* int or void for decls */
  int size; /* for arrays */
  int nestLevel; /* for semantic analysis */
  int funaddr; /* for code generation of function calls -
                  TM location of function entry */
  int offset; /* for code generation of variable access */
/* additional C- attributes for expresssions */
  TreePtr declptr; /* for id's, points to declaration */
  CMType datatype; /* for type checking */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table and
 * type checking information to be printed to
 * the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
