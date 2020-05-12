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
/*   File: symtab.h                                          */
/*   Symbol table interface for the C-Minus compiler         */
/*   (allows only one symbol table)                          */
/*   Version 4 8/1/99                                        */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/

#include "global.h"
#ifndef _SYMTAB_H_
#define _SYMTAB_H_


/* Procedure st_enterScope starts a new scope
 * returns 0 if memory allocation fails, else 1
 */
int st_enterScope(void);

/* Procedure st_exitScope removes all declarations
 * in the current scope
 */
void st_exitScope(void);

/* Procedure st_insert inserts def nodes from
 * from the syntax tree into the symbol table
 * returns 0 if memory allocation fails, else 1
 */
int st_insert( TreePtr );

/* Function st_lookup returns the defnode
 * of a variable, parameter, or function,
 * or NULL if not found
 */
TreePtr st_lookup ( char * name );

#endif
