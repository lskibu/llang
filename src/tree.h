#ifndef LLANG_TREE
#define LLANG_TREE

# include "util.h"

# define llang_tree_create()				__llang_tree_create()
# define llang_tree_destroy(tree)			__llang_tree_destroy(&(tree))
# define llang_tree_clean(tree)				__llang_tree_clean(tree)
# define llang_tree_insert(tree, ptr)			__llang_tree_insert((tree)->root, ptr)
# define llang_tree_delete(tree, node)			__llang_tree_delete_node((tree)->root, node)
# define llang_tree_traverse_preorder(tree, proc)	__llang_tree_traverse_preorder((tree)->root, proc)
# define llang_tree_traverse_inorder(tree, proc)	__llang_tree_traverse_inorder((tree)->root, proc)
# define llang_tree_traverse_postorder(tree, proc)	__llang_tree_traverse_postorder((tree)->root, proc)


typedef struct __TREE_NODE {
    llang_ptr data_ptr;
    __TREE_NODE *left_child;
    __TREE_NODE *right_child;
    __TREE_NODE *parent_node;
} __TREE_NODE;

typedef struct __TREE  {
	struct __TREE_NODE *root;
	llang_i32 n_node;
	llang_i32 height;
	llang_lambda cmp_proc;
} LLANG_TREE;

__TREE_NODE *__llang_node_create(void);

LLANG_TREE *__llang_tree_create();

void __llang_tree_destroy(LLANG_TREE **tree);

void __llang_tree_clean(LLANG_TREE *tree);

void __llang_tree_insert(__TREE_NODE *root, llang_ptr data_item);

llang_i32 __llang_tree_delete_node(__TREE_NODE *root, __TREE_NODE *node);

void __llang_tree_traverse_preorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_traverse_inorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_traverse_postorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_node_rotate_left(__TREE_NODE *node);

void __llang_tree_node_rotate_right(__TREE_NODE *node);

#endif

