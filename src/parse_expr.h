#ifndef LL_PARSE_EXPR
# define LL_PARSE_EXPR

# include "util.h"
# include "node.h"
# include "parser.h"

LL_EXPR * parse_expr(LL_PARSE_TREE *tree, LL_PARSE_SCOPE *scope, __LLANG_NODE **node);

# endif
