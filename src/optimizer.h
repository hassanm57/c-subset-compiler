#ifndef OPTIMIZER_H
#define OPTIMIZER_H

/*
 * optimizer.h - TAC Optimization Passes
 *
 * Implemented passes:
 *   1. Constant Folding     — evaluate constant expressions at compile time
 *   2. Constant Propagation — replace variables assigned constants
 *   3. Dead Code Elimination — remove assignments to values never used
 *   4. Copy Propagation     — replace x = y; ... use(x) → use(y)
 *   5. Algebraic Simplification — x+0=x, x*1=x, x*0=0, etc.
 */

#include "codegen.h"

void optimize(TACList *tl);

#endif /* OPTIMIZER_H */
