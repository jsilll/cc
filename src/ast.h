#ifndef JCC_AST_H
#define JCC_AST_H

#include "base.h"

typedef struct Type Type;
typedef struct ExprNode ExprNode;
typedef struct StmtNode StmtNode;
typedef struct DeclNode DeclNode;

#define GENERATE_TYPE_ENUM(Enum) TY_##Enum,

#define GENERATE_TYPE_EXTERN(Enum) extern Type *TYPE_##Enum;

#define ENUMERATE_TYPES(M)                                                     \
  M(INT)                                                                       \
  M(PTR)

ENUMERATE_TYPES(GENERATE_TYPE_EXTERN)

DECLARE_REPR_ENUM_MACRO(TypeKind, ENUMERATE_TYPES, GENERATE_TYPE_ENUM)

#define ENUMERATE_UNOPS(M)                                                     \
  M(UNOP_ADD)                                                                  \
  M(UNOP_NEG)                                                                  \
  M(UNOP_NOT)                                                                  \
  M(UNOP_ADDR)                                                                 \
  M(UNOP_DEREF)

DECLARE_REPR_ENUM(UnOpKind, ENUMERATE_UNOPS)

#define ENUMERATE_BINOPS(M)                                                    \
  M(BINOP_ADD)                                                                 \
  M(BINOP_SUB)                                                                 \
  M(BINOP_MUL)                                                                 \
  M(BINOP_DIV)                                                                 \
  M(BINOP_EQ)                                                                  \
  M(BINOP_NE)                                                                  \
  M(BINOP_LT)                                                                  \
  M(BINOP_LE)                                                                  \
  M(BINOP_GT)                                                                  \
  M(BINOP_GE)                                                                  \
  M(BINOP_ASGN)

DECLARE_REPR_ENUM(BinOpKind, ENUMERATE_BINOPS)

#define ENUMERATE_EXPRS(M)                                                     \
  M(EXPR_NUM)                                                                  \
  M(EXPR_VAR)                                                                  \
  M(EXPR_UN)                                                                   \
  M(EXPR_BIN)

DECLARE_REPR_ENUM(ExprKind, ENUMERATE_EXPRS)

#define ENUMERATE_STMTS(M)                                                     \
  M(STMT_EXPR)                                                                 \
  M(STMT_RETURN)                                                               \
  M(STMT_DECL)                                                                 \
  M(STMT_BLOCK)                                                                \
  M(STMT_WHILE)                                                                \
  M(STMT_IF)                                                                   \
  M(STMT_FOR)

DECLARE_REPR_ENUM(StmtKind, ENUMERATE_STMTS)

struct Type {
  TypeKind kind;
  Type *base;
};

struct ExprNode {
  ExprKind kind;
  StringView lex;
  Type *type;
  union {
    int32_t num;
    struct {
      StmtNode *decl;
    } var;
    struct {
      UnOpKind op;
      ExprNode *expr;
    } un;
    struct {
      BinOpKind op;
      ExprNode *lhs;
      ExprNode *rhs;
    } bin;
  } u;
};

struct StmtNode {
  StmtKind kind;
  StringView lex;
  StmtNode *next;
  union {
    struct {
      ExprNode *expr;
    } ret;
    struct {
      ExprNode *expr;
    } expr;
    struct {
      StmtNode *body;
    } block;
    struct {
      Type *type;
      StringView name;
      ExprNode *expr;
    } decl;
    struct {
      ExprNode *cond;
      StmtNode *body;
    } whil;
    struct {
      ExprNode *cond;
      StmtNode *then;
      StmtNode *elss;
    } iff;
    struct {
      StmtNode *init;
      ExprNode *cond;
      ExprNode *step;
      StmtNode *body;
    } forr;
  } u;
};

typedef struct {
  StmtNode *body;
} FuncNode;

void ast_debug(FILE *out, FuncNode *ast);

#endif // JCC_AST_H
