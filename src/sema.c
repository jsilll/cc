#include "sema.h"

#include "arena.h"

typedef struct TypeCtx {
  Arena *arena;
} TypeCtx;

static void type_ctx_init(TypeCtx *ctx, Arena *arena) { ctx->arena = arena; }

static void type_ctx_free(TypeCtx *ctx) { ctx->arena = NULL; }

static void type_check_expr(TypeCtx *ctx, ExprNode *expr) {
  switch (expr->kind) {
  case EXPR_VAR:
    expr->type = expr->u.var.decl->u.decl.type;
    break;
  case EXPR_UN:
    type_check_expr(ctx, expr->u.un.expr);
    expr->type = expr->u.un.expr->type;
    break;
  case EXPR_BIN:
    type_check_expr(ctx, expr->u.bin.lhs);
    type_check_expr(ctx, expr->u.bin.rhs);
    expr->type = expr->u.bin.lhs->type;
  case EXPR_NUM:
    break;
  }
}

static void type_check_stmt(TypeCtx *ctx, StmtNode *stmt) {
  switch (stmt->kind) {
  case STMT_EXPR:
    type_check_expr(ctx, stmt->u.expr.expr);
    break;
  case STMT_RETURN:
    type_check_expr(ctx, stmt->u.ret.expr);
    break;
  case STMT_DECL:
    if (stmt->u.decl.expr != NULL) {
      type_check_expr(ctx, stmt->u.decl.expr);
    }
    break;
  case STMT_BLOCK:
    for (StmtNode *n = stmt->u.block.body; n != NULL; n = n->next) {
      type_check_stmt(ctx, n);
    }
    break;
  case STMT_WHILE:
    type_check_expr(ctx, stmt->u.whil.cond);
    type_check_stmt(ctx, stmt->u.whil.body);
    break;
  case STMT_IF:
    type_check_expr(ctx, stmt->u.iff.cond);
    type_check_stmt(ctx, stmt->u.iff.then);
    if (stmt->u.iff.elss != NULL) {
      type_check_stmt(ctx, stmt->u.iff.elss);
    }
    break;
  case STMT_FOR:
    if (stmt->u.forr.init != NULL) {
      type_check_stmt(ctx, stmt->u.forr.init);
    }
    if (stmt->u.forr.cond != NULL) {
      type_check_expr(ctx, stmt->u.forr.cond);
    }
    if (stmt->u.forr.step != NULL) {
      type_check_expr(ctx, stmt->u.forr.step);
    }
    type_check_stmt(ctx, stmt->u.forr.body);
    break;
  }
}

void sema(Arena *arena, FuncNode *func) {
  TypeCtx ctx;
  type_ctx_init(&ctx, arena);
  type_check_stmt(&ctx, func->body);
  type_ctx_free(&ctx);
}
