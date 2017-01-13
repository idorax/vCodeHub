#ifndef _LIBBST_H
#define _LIBBST_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum boolean_s {
	B_FALSE=0,
	B_TRUE
} boolean_t;

typedef int key_t;

typedef struct bst_node_s {
	key_t key;
	struct bst_node_s *left;
	struct bst_node_s *right;
} bst_node_t;

typedef enum walk_order_s {
	WALK_ORDER_PRE = 0x1,	/* root -> left -> right */
	WALK_ORDER_IN,		/* left -> root -> right */
	WALK_ORDER_POST		/* left -> right -> root */
} walk_order_t;

void	bst_walk(bst_node_t *root, walk_order_t order);
int	bst_init(bst_node_t **root, key_t a[], size_t n);
void	bst_fini(bst_node_t *root);

void	bst_add_node(bst_node_t **root, key_t key);
int	bst_add_node2(bst_node_t **root, key_t key);

int	bst_del_node2(bst_node_t **root, key_t key);

bst_node_t *bst_search(bst_node_t *root, key_t key);
bst_node_t *bst_search2(bst_node_t *root, key_t key);
bst_node_t *bst_search3(bst_node_t *root, key_t key, bst_node_t **parent);

int	bst_find(bst_node_t *root, key_t *key, int k);

void	get_debug();

#ifdef	__cplusplus
}
#endif

#endif
