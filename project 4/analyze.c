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
/*   File: analyze.c                                         */
/*   Semantic analyzer implementation for C-Minus compiler   */
/*   Version 4 8/1/99                                        */
/*   Corrected 4/12/00                                       */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/


#include "globals.h"
#include "util.h"
#include "symtab.h"
#include "analyze.h"

/* nesting level counter to check for redeclaration errors */
static int nestLevel = 0;

/* current function pointer for type checking return statements */
static TreePtr currentFunc = NULL;

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse( TreePtr t,
               void (* preProc) (TreePtr),
               void (* postProc) (TreePtr) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

static void declError(TreePtr t, char * message)
{ fprintf(listing,"Declaration error for name '%s' at line %d: %s\n",
                   t->attr.name,t->lineno,message);
  Error = TRUE;
}

/* Procedure insertNode inserts
 * declaration tree nodes t
 * into the symbol table and fills in
 * the declptr field for expression nodes containing
 * identifiers (Ids, Subs, and Calls)
 */
static void insertNode( TreePtr t)
{ switch (t->nodekind)
  { case DeclK:
      { TreePtr p = st_lookup(t->attr.name);
        if ((p != NULL) && (p->nestLevel == nestLevel))
        { declError(t,"redeclaration at same nesting level");
          fprintf(listing,"Previous declaration on line %d\n",p->lineno);
        }
        t->nestLevel = nestLevel;
        st_insert(t);
        if (TraceAnalyze)
          fprintf(listing,
                  "At line %d: Inserting node with name %s, nestLevel = %d\n",
                          t->lineno,t->attr.name,nestLevel);
        if (t->kind.decl == FunK)
        { st_enterScope();
          nestLevel++;
          if (TraceAnalyze)
          { fprintf(listing,
                  "At line %d: Entering scope and incrementing nestLevel\n",
                   t->lineno);
            fprintf(listing,"New nesting level = %d\n",nestLevel);
          }
        }
      }
      break;
    case StmtK:
      if (t->kind.stmt == CmpdK)
      { st_enterScope();
        nestLevel++;
        if (TraceAnalyze)
        { fprintf(listing,"At line %d: Entering scope and incrementing nestLevel\n",
                           t->lineno);
          fprintf(listing,"New nesting level = %d\n",nestLevel);
        }
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
        case CallK:
        case SubsK:
          { TreePtr p = st_lookup(t->attr.name);
            if (TraceAnalyze)
               fprintf(listing,"Lookup of identifier %s at line %d\n",
                                t->attr.name,t->lineno);
            if (p == NULL) declError(t,"undeclared identifier");
            else
            { t->declptr = p;
              if (TraceAnalyze)
                 fprintf(listing,"Found on line %d\n",p->lineno);
            }
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Procedure cleanupST performs the small amount
 * of postorder processing during construction
 * of the symbol table
 */
static void cleanupST( TreePtr t )
{ if ( ((t->nodekind == StmtK) && (t->kind.stmt == CmpdK)) ||
       ((t->nodekind == DeclK) && (t->kind.decl == FunK)))
  { nestLevel--;
    st_exitScope();
    if (TraceAnalyze)
    { fprintf(listing,"At line %d: Exiting scope and decrementing nestLevel\n",
                       t->lineno);
      fprintf(listing,"New nesting level = %d\n",nestLevel);
    }
  }
} /* cleanupST */

/* Procedure initSymtab inserts input and
 * output functions into symbol table
 */
static void initSymtab(void)
{ TreePtr in = newDeclNode(FunK);
  TreePtr out = newDeclNode(FunK);
  TreePtr p = newDeclNode(ParamK);
  in->lineno = 0;
  in->attr.name = "input";
  in->type = INT;
  in->datatype = IntFuncType;
  in->nestLevel = 0;
  st_insert(in);
  if (TraceAnalyze)
      fprintf(listing,
              "At line %d: Inserting node with name %s, nestLevel = %d\n",
              in->lineno,in->attr.name,in->nestLevel);
  out->lineno = 0;
  out->attr.name = "output";
  out->type = VOID;
  out->datatype = VoidFuncType;
  out->nestLevel = 0;
  out->child[0] = p;
  p->datatype = IntType;
  p->nestLevel = 1;
  st_insert(out);
  if (TraceAnalyze)
      fprintf(listing,
              "At line %d: Inserting node with name %s, nestLevel = %d\n",
              out->lineno,out->attr.name,out->nestLevel);
} /* initSymtab */

/* Procedure buildSymtab constructs the symbol
 * table by mostly preorder traversal of the syntax tree
 */
void buildSymtab(TreePtr syntaxTree)
{
  if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
  initSymtab();
  traverse(syntaxTree,insertNode,cleanupST);
}

static void typeError(TreePtr t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure prepTC does the small
 * amount of preorder processing necessary
 * for type checking
 */
static void prepTC( TreePtr t )
{ if ((t->nodekind == DeclK) && (t->kind.decl == FunK))
  { currentFunc = t;
    if (t->type == VOID) t->datatype = VoidFuncType;
    else t->datatype = IntFuncType;
  }
} /* prepTC */

/* Procedure checkNode performs
 * type checking and type inference
 * at a single tree node
 */
static void checkNode(TreePtr t)
{ switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      { case OpK:
          if ((t->child[0]->datatype != IntType) ||
              (t->child[1]->datatype != IntType))
            typeError(t,"op applied to non-integer");
          t->datatype = IntType;
          break;
        case ConstK:
          t->datatype = IntType;
          break;
        case IdK:
          if (t->declptr != NULL)
            t->datatype = t->declptr->datatype;
          break;
        case SubsK:
          if ((t->declptr != NULL) && (t->declptr->datatype != ArrayType))
            typeError(t,"Subscripted variable not array");
          if (t->child[0]->datatype != IntType)
            typeError(t,"Subscript not integer");
          t->datatype = IntType;
          break;
        case CallK:
          { TreePtr p = t->declptr;
            if (p == NULL)
            { typeError(t,"attempt to call non-function");
              fprintf(listing,"(%s)\n",t->attr.name);
            }
            else
            { TreePtr q = t->child[0];
              if ((p->datatype != IntFuncType)
                  && (p->datatype != VoidFuncType))
              { typeError(t,"attempt to call non-function");
                fprintf(listing,"(%s)\n",t->attr.name);
              }
              if (p->datatype == IntFuncType)
                t->datatype = IntType;
              else t->datatype = VoidType;
              p = p->child[0];
              while ((p != NULL) && (q != NULL))
              { if (p->datatype != q->datatype)
                  typeError(q,"type mismatch in call argument");
                p = p->sibling;
                q = q->sibling;
              }
              if (p != NULL) typeError(t,"too few argments in call");
              else if (q != NULL) typeError(t,"too many arguments in call");
            }
          }
          break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { case IfK:
        case WhileK:
          if (t->child[0]->datatype != IntType)
            typeError(t->child[0],"condition is not integer");
          break;
        case ReturnK:
          switch (currentFunc->datatype)
          { case VoidFuncType:
              if (t->child[0] != NULL)
                typeError(t,"void function cannot return a value");
              break;
            case IntFuncType:
              if ((t->child[0] == NULL) || (t->child[0]->datatype != IntType))
                typeError(t,"function must return an integer value");
              break;
            default:
              typeError(t,"unknown function type");
              break;
          }
          break;
        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      {  case VarK:
         case ParamK:
           if (t->type == VOID)
             typeError(t,"void type illegal for parameters and variables");
           if (t->size != 0) t->datatype = ArrayType;
           else t->datatype = IntType;
           if ((t->sibling == NULL) && (t->nestLevel == 0))
             typeError(t,"last declaration must be function");
           break;
         case FunK:
           if (t->sibling == NULL)
           { if (strcmp(t->attr.name,"main"))
               typeError(t,"last function must have name 'main'");
             if (t->datatype != VoidFuncType)
               typeError(t,"main function must be void");
             if (t->child[0] != NULL)
               typeError(t,"main function may not have parameters");
           }
        default: break;
      }
      break;
    default:
      break;
  }
}

/* Procedure typeCheck performs type checking
 * by a mostly postorder syntax tree traversal
 */
void typeCheck(TreePtr syntaxTree)
{ if (TraceAnalyze) fprintf(listing,"\nChecking Types...\n");
  traverse(syntaxTree,prepTC,checkNode);
  if (TraceAnalyze)
  { fprintf(listing,"\nType Checking Finished\n");
    fprintf(listing,"\nSyntax tree with types:\n");
    printTypes(syntaxTree);
  }
}
