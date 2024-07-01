#include "ast.h"

#define GENERATE_TYPE_VARIABLE(Enum)                                           \
  Type *TYPE_##Enum = &(Type){TY_##Enum, NULL};

ENUMERATE_TYPES(GENERATE_TYPE_VARIABLE)

DEFINE_REPR_ENUM(TypeKind, ENUMERATE_TYPES)

DEFINE_REPR_ENUM(UnopKind, ENUMERATE_UNOPS)

DEFINE_REPR_ENUM(BinopKind, ENUMERATE_BINOPS)

DEFINE_REPR_ENUM(ExprKind, ENUMERATE_EXPRS)

DEFINE_REPR_ENUM(StmtKind, ENUMERATE_STMTS)

static void ast_expr_debug(FILE *out, ExprNode *expr, uint8_t indent) {
  for (uint8_t i = 0; i < indent; ++i) {
    fprintf(out, "  ");
  }
  switch (expr->kind) {
  case EXPR_VAR:
    if (expr->type != NULL) {
      fprintf(out, "%s<%s>: '%.*s' %p\n", ExprKind_Repr[expr->kind],
              TypeKind_Repr[expr->type->kind], (int)expr->lex.size,
              expr->lex.data, (void *)expr->u.var.decl);
    } else {
      fprintf(out, "%s: '%.*s' %p\n", ExprKind_Repr[expr->kind],
              (int)expr->lex.size, expr->lex.data, (void *)expr->u.var.decl);
    }
    break;
  case EXPR_NUM:
    if (expr->type != NULL) {
      fprintf(out, "%s<%s>: %d\n", ExprKind_Repr[expr->kind],
              TypeKind_Repr[expr->type->kind], expr->u.num);
    } else {
      fprintf(out, "%s: %d\n", ExprKind_Repr[expr->kind], expr->u.num);
    }
    break;
  case EXPR_UN:
    if (expr->type != NULL) {
      fprintf(out, "%s<%s>:\n", UnopKind_Repr[expr->u.un.op],
              TypeKind_Repr[expr->type->kind]);
    } else {
      fprintf(out, "%s:\n", UnopKind_Repr[expr->u.un.op]);
    }
    ast_expr_debug(out, expr->u.un.expr, indent + 1);
    break;
  case EXPR_BIN:
    if (expr->type != NULL) {
      fprintf(out, "%s<%s>:\n", BinopKind_Repr[expr->u.bin.op],
              TypeKind_Repr[expr->type->kind]);
    } else {
      fprintf(out, "%s:\n", BinopKind_Repr[expr->u.bin.op]);
    }
    ast_expr_debug(out, expr->u.bin.lhs, indent + 1);
    ast_expr_debug(out, expr->u.bin.rhs, indent + 1);
    break;
  }
}

static void ast_stmt_debug(FILE *out, StmtNode *stmt, uint8_t indent) {
  for (uint8_t i = 0; i < indent; ++i) {
    fprintf(out, "  ");
  }
  switch (stmt->kind) {
  case STMT_EXPR:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    ast_expr_debug(out, stmt->u.expr.expr, indent + 1);
    break;
  case STMT_RETURN:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    if (stmt->u.ret.expr != NULL) {
      ast_expr_debug(out, stmt->u.ret.expr, indent + 1);
    }
    break;
  case STMT_DECL:
    if (stmt->u.decl.type != NULL) {
      fprintf(out, "%s<%s>: '%.*s' %p\n", StmtKind_Repr[stmt->kind],
              TypeKind_Repr[stmt->u.decl.type->kind],
              (int)stmt->u.decl.name.size, stmt->u.decl.name.data,
              (void *)stmt);
    } else {
      fprintf(out, "%s: '%.*s' %p\n", StmtKind_Repr[stmt->kind],
              (int)stmt->u.decl.name.size, stmt->u.decl.name.data,
              (void *)stmt);
    }
    if (stmt->u.decl.expr != NULL) {
      ast_expr_debug(out, stmt->u.decl.expr, indent + 1);
    }
    break;
  case STMT_BLOCK:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    for (StmtNode *s = stmt->u.block.body; s != NULL; s = s->next) {
      ast_stmt_debug(out, s, indent + 1);
    }
    break;
  case STMT_WHILE:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    ast_expr_debug(out, stmt->u.whil.cond, indent + 1);
    ast_stmt_debug(out, stmt->u.whil.body, indent + 1);
    break;
  case STMT_IF:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    ast_expr_debug(out, stmt->u.iff.cond, indent + 1);
    ast_stmt_debug(out, stmt->u.iff.then, indent + 1);
    if (stmt->u.iff.elss != NULL) {
      ast_stmt_debug(out, stmt->u.iff.elss, indent + 1);
    }
    break;
  case STMT_FOR:
    fprintf(out, "%s:\n", StmtKind_Repr[stmt->kind]);
    ast_stmt_debug(out, stmt->u.forr.init, indent + 1);
    if (stmt->u.forr.cond != NULL) {
      ast_expr_debug(out, stmt->u.forr.cond, indent + 1);
    }
    if (stmt->u.forr.step != NULL) {
      ast_expr_debug(out, stmt->u.forr.step, indent + 1);
    }
    ast_stmt_debug(out, stmt->u.forr.body, indent + 1);
    break;
  }
}

void ast_debug(FILE *out, FuncNode *func) {
  if (func == NULL) {
    return;
  }
  ast_stmt_debug(out, func->body, 1);
}
