# include "util.h"
# include "tree.h"

__TREE_NODE *__llang_node_create(void)
{
	__TREE_NODE *node = (__TREE_NODE *) llang_malloc(sizeof (__TREE_NODE));
	return node;	
};

LLANG_TREE *__llang_tree_create(void)
{
	return (LLANG_TREE *) llang_malloc(sizeof (LLANG_TREE));
};

void __llang_tree_destroy(LLANG_TREE **tree)
{
	__llang_tree_clean(*tree);
	free(*tree);
	*tree = NULL;
};

void __llang_tree_clean(LLANG_TREE *tree)
{
	llang_ptr process_node(llang_ptr node)
	{

	};
	__llang_tree_traverse_preorder(tree->root, (llang_lambda) process_node);
};

void __llang_tree_insert(__TREE_NODE *root, llang_ptr data_item);

llang_i32 __llang_tree_delete_node(__TREE_NODE *root, __TREE_NODE *node);

void __llang_tree_traverse_preorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_traverse_inorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_traverse_postorder(__TREE_NODE *root, llang_lambda node_proc);

void __llang_tree_node_rotate_left(__TREE_NODE *node);

void __llang_tree_node_rotate_right(__TREE_NODE *node);
