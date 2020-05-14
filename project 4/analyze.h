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
/*   File: analyze.h                                         */
/*   Semantic analyzer interface for C-Minus compiler        */
/*   Version 4 8/1/99                                        */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/
/*   Version: 4                                              */
/*   Date: 5/14/20  						                 */  
/*   Authors: Jason Giroux, Jeffrey Chesanek, Dhaval Patel   */
/*   jgiroux776@g.rwu.edu,jchesanek513@g.rwu.edu,            */
/*   dpatel657@g.rwu.edu                                     */
/*   Roger Williams University		                         */
/*   COMSC 440 Language Translation and Compiler Design      */
/*************************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_


/* Function buildSymtab constructs the symbol
 * table by a mainly preorder traversal of the syntax tree
 */
void buildSymtab(TreePtr);

/* Procedure typeCheck performs type checking
 * by a mainly postorder syntax tree traversal
 */
void typeCheck(TreePtr);

#endif
