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
/*   File: symtab.c                                          */
/*   Symbol table implementation for the C-Minus compiler    */
/*   (allows only one symbol table)                          */
/*   Version: 4                                              */
/*   Authors: Jason Giroux, Jeffrey Chesanek, Dhaval Patel   */
/*   jgiroux776@g.rwu.edu,jchesanek513@g.rwu.edu,            */
/*   dpatel657@g.rwu.edu                                     */
/*   Roger Williams University		                           */
/*   COMSC 440 Language Translation and Compiler Design      */
/*************************************************************/

#include "symtab.h"


/* SIZE is the size of each hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }			//add code here to complete the implementation of the hash function

  return temp;
}

/* The record in the bucket lists for
 * each defnode
 */
typedef struct BucketListRec
   { TreePtr defnode;
     struct BucketListRec * next;
   } * BucketList;

/* each hash table */
typedef BucketList Table[SIZE];

/* the linked list of tables */
typedef struct TableListRec
   { Table t;
     struct TableListRec * next;
   } * TableList;

/* The actual symbol table */
static TableList symTab = NULL; /* starts out empty */

/* Procedure st_enterScope starts a new scope
 * returns 0 if memory allocation fails, else 1
 */
int st_enterScope(void)
{ TableList tempTab = (TableList) malloc(sizeof(struct TableListRec));
  int i;
  if (!tempTab) return 0; /* memory allocation error */
  for (i=0;i<SIZE;i++) 
	  tempTab->t[i] = NULL;
	  tempTab->next = symTab;
	  symTab = tempTab;
  return 1;
} /* st_enterScope */

/* Procedure st_exitScope removes all declarations
 * in the current scope
 */
void st_exitScope(void)
{
  symTab = symTab->next;
			//Add code to handle the case when a scope needs to be deleted
}

/* Procedure st_insert inserts def nodes from
 * from the syntax tree into the symbol table
 * returns 0 if memory allocation fails, else 1
 */
int st_insert( TreePtr t)
{
	int h = hash(t->attr.name);	//step 1: calculate the hash value for t
	if (!symTab) {
		st_enterScope();	//step 2: when the table is empty, create a new scope
		return 0;		//step 3: If the obtained symbol table is empty, return 0
	}
	//step 4: allocate memory for a new BucketListRec and cast the return
	//type to BucketList and assigned it a variable l, which should
	//be declared as BucketList
	BucketList l = (BucketList) malloc(sizeof(struct BucketListRec));
	if(!l) {	//step 5: check on memeory allocation error
		return 0;
	}
	
	l->defnode = t;		//step 6: set up the attribute(defnode) of l as t, the accepted TreePtr
	l->next = symTab->t[h];	//step 7: insert the new BucketList l to the specific hash table cell
	symTab->t[h] = l;	//step 8: return the new BucketList l 
	return 1;
} /* st_insert */

/* Function st_lookup returns the defnode
 * of a variable, parameter, or function,
 * or NULL if not found
 */
TreePtr st_lookup ( char * name )
{ int h = hash(name);
  TableList currentTable = symTab;
  while (currentTable)
  { BucketList l =  currentTable->t[h];
    while (l && (strcmp(name,l->defnode->attr.name) != 0))
      l = l->next;
    if (l) return l->defnode;
    else currentTable = currentTable->next;
  }
  return NULL;
}  /* st_lookup */

