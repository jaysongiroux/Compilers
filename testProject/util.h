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
/*   File: util.h                                            */
/*   Utility function interface for the C-Minus compiler     */
/*   Version 4 8/1/99                                        */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType, const char* );

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction
 */
TreePtr newDeclNode(DeclKind);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreePtr newStmtNode(StmtKind);

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreePtr newExpNode(ExpKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString( char * );

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreePtr );

/* printTypes prints the tree with data types
 */
void printTypes( TreePtr );

#endif
