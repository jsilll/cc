#ifndef JCC_SEMA_H
#define JCC_SEMA_H

#include "ast.h"
#include "arena.h"

void sema(Arena *arena, FuncNode *ast);

#endif // JCC_SEMA_H
