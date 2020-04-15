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
/*   File: util.c                                            */
/*   Utility function implementation for                     */
/*   the C-Minus compiler                                    */
/*   Version 4 8/1/99                                        */
/*   K. Louden, Dept. of Math & Computer Science             */
/*   San Jose State University, San Jose, CA 95192-0103      */
/*   (408) 924-5134   louden@cs.sjsu.edu                     */
/*   http://www.mathcs.sjsu.edu/faculty/louden/              */
/*************************************************************/

#include "global.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case ELSE:
    case IF:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case LE: fprintf(listing,"<=\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GE: fprintf(listing,">=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case NE: fprintf(listing,"!=\n"); break;
    case ASSIGN: fprintf(listing,"=\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
    case COMMA: fprintf(listing,",\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LBRACKET: fprintf(listing,"[\n"); break;
    case RBRACKET: fprintf(listing,"]\n"); break;
    case LCURLY: fprintf(listing,"{\n"); break;
    case RCURLY: fprintf(listing,"}\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val = %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name = %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
} /* printToken */

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction
 */
TreePtr newDeclNode(DeclKind kind)
{ TreePtr t = (TreePtr) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->size = 0;
    t->lineno = lineno;
    t->nestLevel = 0;  /* additional initializations */
    t->declptr = NULL;
    t->datatype = VoidType;
  }
  return t;
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreePtr newStmtNode(StmtKind kind)
{ TreePtr t = (TreePtr) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->nestLevel = 0;  /* additional initializations */
    t->declptr = NULL;
    t->datatype = VoidType;
  }
  return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreePtr newExpNode(ExpKind kind)
{ TreePtr t = (TreePtr) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->nestLevel = 0;  /* additional initializations */
    t->declptr = NULL;
    t->datatype = VoidType;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreePtr tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==DeclK)
    { switch (tree->kind.decl) {
        case VarK:
          fprintf(listing,"var: %s",tree->attr.name);
          if (tree->size>0)
            fprintf(listing,"[%d]",tree->size);
          fprintf(listing,": ");
          switch (tree->type) {
            case VOID: fprintf(listing,"void\n"); break;
            case INT: fprintf(listing,"int\n"); break;
            default: fprintf(listing,"unknown type\n");break;
          }
        break;
        case FunK:
          fprintf(listing,"fun: %s",tree->attr.name);
          fprintf(listing,": ");
          switch (tree->type) {
            case VOID: fprintf(listing,"void\n"); break;
            case INT: fprintf(listing,"int\n"); break;
            default: fprintf(listing,"unknown type\n");break;
          }
          break;
        case ParamK:
          fprintf(listing,"param: %s",tree->attr.name);
          if (tree->size!=0)
            fprintf(listing,"[]");
          fprintf(listing,": ");
          switch (tree->type) {
            case VOID: fprintf(listing,"void\n"); break;
            case INT: fprintf(listing,"int\n"); break;
            default: fprintf(listing,"unknown type\n");break;
          }
          break;
        default:
          fprintf(listing,"Unknown DeclNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case ReturnK:
          fprintf(listing,"Return\n");
          break;
        case WhileK:
          fprintf(listing,"While\n");
          break;
        case CmpdK:
          fprintf(listing,"Cmpd\n");
          break;
        default:
          fprintf(listing,"Unknown StmtNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\n");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        case SubsK:
          fprintf(listing,"Subs: %s\n",tree->attr.name);
          break;
        case CallK:
          fprintf(listing,"Call: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
} /* printTree */

/* printType used in printTypes procedure */
static void printType(CMType t)
{ switch (t)
  { case VoidType: fprintf(listing,"Void type");break;
    case IntType: fprintf(listing,"Int type");break;
    case VoidFuncType: fprintf(listing,"Void func type");break;
    case IntFuncType: fprintf(listing,"Int func type");break;
    case ArrayType: fprintf(listing,"Array type");break;
    default: fprintf(listing,"Unknown type");break;
  }
} /* printType */

void printOp( TokenType token)
{ switch (token)
  { case PLUS: fprintf(listing,"+"); break;
    case MINUS: fprintf(listing,"-"); break;
    case TIMES: fprintf(listing,"*"); break;
    case OVER: fprintf(listing,"/"); break;
    case LT: fprintf(listing,"<"); break;
    case LE: fprintf(listing,"<="); break;
    case GT: fprintf(listing,">"); break;
    case GE: fprintf(listing,">="); break;
    case EQ: fprintf(listing,"=="); break;
    case NE: fprintf(listing,"!="); break;
    case ASSIGN: fprintf(listing,"="); break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d",token);
  }
} /* printOp */



/* printTypes prints the tree with data types
 */
void printTypes( TreePtr tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==DeclK)
    { switch (tree->kind.decl) {
        case VarK:
          fprintf(listing,"var: %s",tree->attr.name);
          if (tree->size>0)
            fprintf(listing,"[%d]",tree->size);
          fprintf(listing,": ");
          printType(tree->datatype);
          fprintf(listing,"\n");
          break;
        case FunK:
          fprintf(listing,"fun: %s",tree->attr.name);
          fprintf(listing,": ");
          printType(tree->datatype);
          fprintf(listing,"\n");
          break;
        case ParamK:
          fprintf(listing,"param: %s",tree->attr.name);
          if (tree->size!=0)
            fprintf(listing,"[]");
          fprintf(listing,": ");
          printType(tree->datatype);
          fprintf(listing,"\n");
          break;
        default:
          fprintf(listing,"Unknown DeclNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case ReturnK:
          fprintf(listing,"Return\n");
          break;
        case WhileK:
          fprintf(listing,"While\n");
          break;
        case CmpdK:
          fprintf(listing,"Cmpd\n");
          break;
        default:
          fprintf(listing,"Unknown StmtNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printOp(tree->attr.op);
          break;
        case ConstK:
          fprintf(listing,"const: %d",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s",tree->attr.name);
          break;
        case SubsK:
          fprintf(listing,"Subs: %s",tree->attr.name);
          break;
        case CallK:
          fprintf(listing,"Call: %s",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
      fprintf(listing,": ");
      printType(tree->datatype);
      fprintf(listing,"\n");
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTypes(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}


