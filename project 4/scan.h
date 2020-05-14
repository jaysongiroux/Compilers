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
/*   File: scan.h                                            */
/*   The scanner interface for the C-Minus compiler          */
/*   Version: 4                                              */
/*   Authors: Jason Giroux, Jeffrey Chesanek, Dhaval Patel   */
/*   jgiroux776@g.rwu.edu,jchesanek513@g.rwu.edu,            */
/*   dpatel657@g.rwu.edu                                     */
/*   Roger Williams University		                         */
/*   COMSC 440 Language Translation and Compiler Design      */
/*************************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* function getToken returns the 
 * next token in source file
 */
TokenType getToken(void);

#endif
