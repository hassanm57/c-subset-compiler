#ifndef SEMANTIC_H
#define SEMANTIC_H

/*
 * semantic.h - Semantic Analysis Pass
 * Performs: type checking, scope resolution, undeclared variable detection,
 *           type coercion warnings, return-type checking, array bounds.
 */

#include "ast.h"

void semantic_analyze(ASTNode *root);

#endif /* SEMANTIC_H */
