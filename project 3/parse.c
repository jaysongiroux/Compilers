/*************************************************************/
/*************************************************************/
/*   File: parse.c                                           */
/*   The parser implementation for the C-Minus compiler      */
/*************************************************************/


//Please fill up the blank for functions additive_expression, term and factor.

#include "global.h"
#include "util.h"
#include "scan.h"
#include "parse.h"


static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreePtr declaration_list(void);
static TreePtr declaration(void);
static TreePtr var_declaration(void);
static TreePtr local_declarations(void);
static TreePtr param_list(void);
static TreePtr param(int);
static TreePtr compound_stmt(void);
static TreePtr statement_list(void);
static TreePtr statement(void);
static TreePtr selection_stmt(void);
static TreePtr iteration_stmt(void);
static TreePtr return_stmt(void);
static TreePtr expression(void);
static TreePtr simple_expression(void);
static TreePtr additive_expression(void);
static TreePtr term(void);
static TreePtr factor(void);
static TreePtr arg_list(void);

static void syntaxError(char * message)
{ fprintf(listing,">>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
  }
}

static int isType( TokenType token)
{ return (token==VOID)||(token==INT);
}

TreePtr declaration_list()
{ TreePtr t,p;
  p = t = declaration();
  while (token != ENDFILE)
    if (p != NULL)
    { p->sibling = declaration();
      if (p->sibling != NULL) p = p->sibling;
    }
    else
    { p = declaration();
      if (t == NULL) t = p;
    }
  return t;
} /* declaration_list */

TreePtr declaration(void)
{ TreePtr t = NULL,p;
  TokenType savedType;
  char* savedName;
  if (isType(token))
  { savedType = token;
    match(token);
  }
  else
    syntaxError("type expected\n");
  if (token == ID)
  { savedName = copyString(tokenString);
    match(ID);
  }
  else
    syntaxError("Identifier expected\n");
  switch (token)
  { case SEMI: /* simple variable declaration */
      t = newDeclNode(VarK);
      if (t!=NULL)
      { t->attr.name = savedName;
        t->type = savedType;
      }
      match(SEMI);
      break;
    case LBRACKET: /* array variable declaration */
      t = newDeclNode(VarK);
      if (t!=NULL)
      { t->attr.name = savedName;
        t->type = savedType;
      }
      match(LBRACKET);
      if (token == NUM)
      { int size = atoi(tokenString);
        if (size <= 0) syntaxError("array size must be > 0\n");
        /* use nonzero size later to test for arrays */
        else if (t!=NULL) t->size = size;
        match(NUM);
      }
      else syntaxError("Number expected\n");
      match(RBRACKET);
      match(SEMI);
      break;
    case LPAREN: /* function declaration */
      t = newDeclNode(FunK);
      if (t!=NULL)
      { t->attr.name = savedName;
        t->type = savedType;
      }
      match(LPAREN);
      p = param_list(); /* VOID lookahead problem means param_list
                           must handle both cases */
      if (t!=NULL) t->child[0] = p;
      match(RPAREN);
      p = compound_stmt();
      if (t!=NULL) t->child[1] = p;
      break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
  }
  return t;
} /* declaration */

TreePtr var_declaration(void)
{ TreePtr t = NULL;
  TokenType savedType;
  char* savedName;
  if (isType(token))
  { savedType = token;
    match(token);
  }
  else
  { syntaxError("type expected\n");
    return NULL;
  }
  if (token == ID)
  { savedName = copyString(tokenString);
    match(ID);
  }
  else
  { syntaxError("Identifier expected\n");
    return NULL;
  }
  t = newDeclNode(VarK);
  if (t!=NULL)
  { t->attr.name = savedName;
    t->type = savedType;
  }
  if (token==LBRACKET)
  {  match(LBRACKET);
     if (token == NUM)
     { int size = atoi(tokenString);
       if (size <= 0) syntaxError("array size must be > 0\n");
       /* use nonzero size later to test for arrays */
       else if (t!=NULL) t->size = size;
       match(NUM);
     }
     else syntaxError("Number expected\n");
     match(RBRACKET);
  }
  match(SEMI);
  return t;
} /* var_declaration */

TreePtr local_declarations(void)
{ TreePtr t=NULL,p=t;
  if (isType(token))
  { p = t = var_declaration();
    while (isType(token))
      if (p != NULL)
      { p->sibling = var_declaration();
        if (p->sibling != NULL) p = p->sibling;
      }
      else
      { p = var_declaration();
        if (t == NULL) t = p;
      }
  }
  return t;
} /* local_declarations */

TreePtr param_list(void)
{ TreePtr t,p,dp;
  p = t = param(TRUE); /* first time */
  while (token == COMMA)
  { match(COMMA);
    dp = param(FALSE); /* not first time */
    if (p != NULL && dp != NULL) 
    { p->sibling = dp;
      p = dp;
    }
  }
  return t;
} /* param_list */

TreePtr param(int first_time)
{ TreePtr t = NULL;
  TokenType savedType;
  char* savedName;
  if (isType(token))
  { savedType = token;
    match(token);
  }
  else
  { syntaxError("type expected\n");
    return NULL;
  }
  if (token == ID)
  { savedName = copyString(tokenString);
    match(ID);
  }
  else if ((token == RPAREN) && (savedType == VOID) && first_time)
  /* empty paramlist case */
  { return NULL;
  }
  else
  { syntaxError("Identifier expected\n");
    return NULL;
  }
  t = newDeclNode(ParamK);
  if (t!=NULL)
  { t->attr.name = savedName;
    t->type = savedType;
  }
  if (token == LBRACKET)
  { match(LBRACKET);
    /* flag for array parameter */
    if (t!=NULL) t->size = -1;
    match(RBRACKET);
  }
  return t;
} /* param */

TreePtr compound_stmt(void)
{ TreePtr t,p,q;
  match(LCURLY);
  t = newStmtNode(CmpdK);
  p = local_declarations();
  q = statement_list();
  if (t != NULL)
  { t->child[0] = p;
    t->child[1] = q;
  }
  match(RCURLY);
  return t;
} /* compound_stmt */

TreePtr statement_list(void)
{ TreePtr t = NULL,p;
  if (token != RCURLY)
  { p = t = statement();
    while ((token != RCURLY) && (token != ENDFILE))
    { if (p != NULL)
      { p->sibling = statement();
        if (p->sibling != NULL) p = p->sibling;
      }
      else
      { p = statement();
        if (t == NULL) t = p;
      }
    }
  }
  return t;
} /* statement_list */

TreePtr statement(void)
{ TreePtr t = NULL;
  switch (token)
  { case SEMI:
    /* empty expression statement */
      match(SEMI);
      break;
    case ID:
    /* expression_stmt with possible assignment */
      t = expression();
      match(SEMI);
      break;
    case LPAREN:
    case NUM:
    /* expression_stmt without assignment */
      t = simple_expression();
      match(SEMI);
      break;
    case LCURLY:
      t = compound_stmt();
      break;
    case IF:
      t = selection_stmt();
      break;
    case WHILE:
      t = iteration_stmt();
      break;
    case RETURN:
      t = return_stmt();
      break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
  }
  return t;
} /* statememt */

TreePtr selection_stmt(void)
{ TreePtr t,p;
  t = newStmtNode(IfK);
  match(IF);
  match(LPAREN);
  p = expression();
  if (t != NULL) t->child[0] = p;
  match(RPAREN);
  p = statement();
  if (t != NULL) t->child[1] = p;
  if (token == ELSE)
  { match(ELSE);
    p = statement();
    if (t != NULL) t->child[2] = p;
  }
  return t;
} /* selection_stmt */

TreePtr iteration_stmt(void)
{ TreePtr t,p;
  t = newStmtNode(WhileK);
  match(WHILE);
  match(LPAREN);
  p = expression();
  if (t != NULL) t->child[0] = p;
  match(RPAREN);
  p = statement();
  if (t != NULL) t->child[1] = p;
  return t;
} /* iteration_stmt */

TreePtr return_stmt(void)
{ TreePtr t,p;
  t = newStmtNode(ReturnK);
  match(RETURN);
  if (token != SEMI)
  { p = expression();
    if (t != NULL) t->child[0] = p;
  }
  match(SEMI);
  return t;
} /* return_stmt */

TreePtr expression(void)
{ TreePtr t,p,q;
  /* should be var if assignment is coming */
  t = simple_expression();
  if (token == ASSIGN)
  { match(ASSIGN);
    p = expression();
    /* make sure lhs of assignment really is a var */
    if ((t != NULL) && (t->kind.exp != IdK) && (t->kind.exp != SubsK))
      syntaxError("illegal left hand side of assignment\n");
    q = newExpNode(OpK);
    if (q != NULL)
    { q->attr.op = ASSIGN;
      q->child[0] = t;
      q->child[1] = p;
      t = q;
    }
  }
  return t;
} /* expression */

TreePtr simple_expression(void)
{ TreePtr t,p,q;
  t = additive_expression();
  if ((token == LT) || (token == LE) || (token == GT)
       || (token == GE) || (token == EQ) || (token == NE))
  {  p = newExpNode(OpK);
     p->attr.op = token;
     match(token);
     q = additive_expression();
     if (p != NULL)
     { p->child[0] = t;
       p->child[1] = q;
       t = p;
     }
  }
  return t;
} /* simple_expression */

TreePtr additive_expression(void)
{ TreePtr t,p,q;
  t = term();
  if ((token == PLUS) || (token == MINUS))
  {  p = newExpNode(OpK);
     p->attr.op = token;
     match(token);
     q = additive_expression();
     if (p != NULL)
     { p->child[0] = t;
       p->child[1] = q;
       t = p;
     }
  }         //*************Add code here 
   return t;
} /* additive_expression */

TreePtr term(void)
{ TreePtr t,p,q;
  t = factor();
  if ((token == TIMES || token == OVER))
  {  p = newExpNode(OpK);
     p->attr.op = token;
     match(token);
     q = term();
     if (p != NULL)
     { p->child[0] = t;
       p->child[1] = q;
       t = p;
     }
  }         //*************Add code here
  return t;
} /* term */

TreePtr factor(void)
{ TreePtr t = NULL,p;
  char* savedName;
  switch (token)
  { case LPAREN:           
     
     match(LPAREN);
     t = expression();
     match(RPAREN);
     break;
     
      case ID:
      savedName = copyString(tokenString);
      match(ID);
      if (token == LPAREN)
      { match(LPAREN);
        t = newExpNode(CallK);
        if (t != NULL) t->attr.name = savedName;
        if (token != RPAREN)
        { p = arg_list();
          if (t != NULL) t->child[0] = p;
        }
        match(RPAREN);
      }
      else if (token == LBRACKET)
      { match(LBRACKET);
        t = newExpNode(SubsK);
        if (t != NULL) t->attr.name = savedName;
        p = expression();
        if (t != NULL) t->child[0] = p;
        match(RBRACKET);
      }
      else
      { t = newExpNode(IdK);
        if (t != NULL) t->attr.name = savedName;
      }
      break;
    case NUM :
      t = newExpNode(ConstK);
        if ((t!=NULL) && (token==NUM))
          t->attr.val = atoi(tokenString);     //************** Add code here 
        match(NUM);
     break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
  }
  return t;
} /* factor */

TreePtr arg_list(void)
{ TreePtr t,p;
  p = t = expression();
  while (token == COMMA)
  { match(COMMA);
    if (p != NULL)
    { p->sibling = expression();
      if (p->sibling != NULL) p = p->sibling;
    }
    else
    { p = expression();
      if (t == NULL) t = p;
    }
  }
  return t;
} /* arg_list */


TreePtr parse(void)
{ TreePtr syntaxTree;
  token = getToken();
  syntaxTree = declaration_list();
  if (token != ENDFILE) syntaxError("end of file expected\n");
  if (TraceParse)
  { fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);
  }
  return syntaxTree;
}
