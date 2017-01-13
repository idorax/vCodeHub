#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "libbst.h"

static boolean_t debug = B_FALSE;
#define DEBUG(a) { if (debug) a; }

void
get_debug()
{
	char *s = getenv("DEBUG");
	if (s != NULL && strcasecmp(s, "yes") == 0)
		debug = B_TRUE;
}

//
// OPs: 1. Init a BST and Fini a BST
//      2. Walk the whole BST (pre-order, in-order, post-order)
//      3. Add, delete and search a node
//

// -1- 1.1 Init a BST ----------------------------------------------------------
int
bst_init(bst_node_t **root, key_t a[], size_t n)
{
	size_t i;

	*root = NULL;
	for (i = 0; i < n; i++) {
		if (i % 2) {
			int rc;
			if ((rc = bst_add_node2(root, a[i])) != 0)
				return -1;
		} else {
			bst_add_node(root, a[i]);
		}
	}

	return 0;
}

// -2- 1.2 Fini a BST ----------------------------------------------------------
static inline void
BST_DESTROY_NODE(bst_node_t *p)
{
	DEBUG(printf("%s1: %p %8d\t%10p %10p\n", __func__,
	    p, *(key_t *)p, p->left, p->right));

	p->left = NULL;
	p->right = NULL;
	*(unsigned long long *)p = 0xdeadbeef;

	DEBUG(printf("%s2: %p %llx\t%10p %10p\n\n", __func__,
	    p, *(unsigned long long *)p, p->left, p->right));
}

void
bst_fini(bst_node_t *root)
{
	if (root == NULL)
		return;

	bst_fini(root->left);
	bst_fini(root->right);

	BST_DESTROY_NODE(root);

	free(root);
}

// -2- Walk a BST --------------------------------------------------------------
void
bst_walk(bst_node_t *root, walk_order_t order)
{
	if (root == NULL)
		return;

	/*
	 *  preorder: root, left, right
	 *   inorder: left, root, right
	 * postorder: left, right,root
	 *
	 * NOTE: inorder is sorted.
	 *
	 * If the array is 1, 3, 2, 5, 4, 6, 0
	 *
	 *           1
	 *          /.\
	 *         / . \
	 *        /  .  \
	 *       /   .   \
	 *      /    .    \
	 *     /     .     \
	 *    0      .      3
	 *    .      .     /.\
	 *    .      .    / . \
	 *    .      .   /  .  \
	 *    .      .  2   .   5
	 *    .      .  .   .  /.\
	 *    .      .  .   . 4 . 6
	 *    .      .  .   . . . .
	 *    +------+--+---+-+-+-+
	 *    0      1  2   3 4 5 6 (Walk By InOrder, left-root-right)
	 */
	switch (order) {
	case WALK_ORDER_PRE:
		printf("[%s] %p: %3d\n", " PRE", root, root->key);
		bst_walk(root->left,  order);
		bst_walk(root->right, order);
		break;

	case WALK_ORDER_IN:
		bst_walk(root->left,  order);
		printf("[%s] %p: %3d\n", "  IN", root, root->key);
		bst_walk(root->right, order);
		break;

	case WALK_ORDER_POST:
		bst_walk(root->left,  order);
		bst_walk(root->right, order);
		printf("[%s] %p: %3d\n", "POST", root, root->key);
		break;

	default:
		return;
	}
}

// -3- 3.1 Add a BST node  -----------------------------------------------------

/*
 * Add node to BST recursively
 */
void
bst_add_node(bst_node_t **root, key_t key)
{
	DEBUG(printf("DEBUG1: %s\tis called (key=%3d)\n", __func__, key));

	if (*root == NULL) {
		bst_node_t *leaf = NULL;
		leaf = (bst_node_t *)malloc(sizeof (bst_node_t));
		if (leaf == NULL) {
			fprintf(stderr, "failed to malloc\n");
			return;
		}

		/* init leaf node */
		leaf->key   = key;
		leaf->left  = NULL;
		leaf->right = NULL;

		*root = leaf;
	} else {
		bst_node_t **pp = NULL;
		if (key < (*root)->key)
			pp = &(*root)->left;
		else
			pp = &(*root)->right;
		bst_add_node(pp, key);
	}
}

/*
 * Add a node to BST non-recursively
 */
int
bst_add_node2(bst_node_t **root, key_t key)
{
	DEBUG(printf("DEBUG2: %s\tis called (key=%3d)\n", __func__, key));

	bst_node_t *leaf = NULL;
	leaf = (bst_node_t *)malloc(sizeof (bst_node_t));
	if (leaf == NULL) {
		fprintf(stderr, "failed to malloc\n");
		return -1;
	}

	/* init leaf node */
	leaf->key   = key;
	leaf->left  = NULL;
	leaf->right = NULL;

	/* add leaf node to root */
	if (*root == NULL) {	/* root node does not exit */
		*root = leaf;
	} else {
		bst_node_t **pp = NULL;
		while (1) {
			if (leaf->key < (*root)->key)
				pp = &((*root)->left);
			else
				pp = &((*root)->right);

			if (*pp == NULL) {
				*pp = leaf;
				break;
			}

			root = pp;
		}
	}

	return 0;
}

// -3- 3.2 Delete a BST node  --------------------------------------------------

static inline bst_node_t *
bst_get_leftmost_leaf(bst_node_t *root)
{
	if (root == NULL)
		return NULL;

	bst_node_t *p = root;
	while (p->left != NULL && p->right != NULL)
		p = p->left;

	return p;
}

/*
 * Delete a node from BST
 */
int
bst_del_node2(bst_node_t **root, key_t key)
{
	bst_node_t *parent = NULL;
	bst_node_t *this = bst_search3(*root, key, &parent);
	if (this == NULL) /* key is not found */
		return -1;

#define PROOT(new) do { \
	printf("ROOT %p ==> NewROOT %p\n", this, new); \
	} while (0)

#define PNODE(tag) do { \
	printf("DEL key=%3d, tag=%s, parent=%p:%d:%p:%p\n\n", \
	       this->key, tag, parent, \
	       parent->key, parent->left, parent->right); \
	} while (0)

#define ISROOT  (this == parent)
#define ISLEFT  (this == parent->left)
#define ISRIGHT (this == parent->right)
	if (this->left == NULL && this->right == NULL) {
		PNODE("LEAF");
		/*
		 * 1. If this is a leaf node
		 *
		 *    1.1 If this is a root node,
		 *
		 *                (D)
		 *
		 *        New root should be NULL
		 *           *root <== NULL
		 *
		 *    1.2 else:
		 *
		 *              P  |  P
		 *             /   |   \
		 *        this(D)  |   (D)this
		 *                 |
		 *              (a)|(b)
		 *
		 *        (a) P->left  <== NULL ; this is P's left  child
		 *        (b) P->right <== NULL ; this is P's right child
		 */

		if (ISROOT) {
			PROOT(NULL);
			*root = NULL;
		} else {
			if (ISLEFT)
				parent->left  = NULL;
			if (ISRIGHT)
				parent->right = NULL;
		}

	} else if (this->left != NULL && this->right == NULL) {
		PNODE("NON-LEAF:LFonly");
		/*
		 * 2. If this node is a non-leaf node having left child only,
		 *
		 *    2.1 If this node is a root node,
		 *
		 *                (D)this
		 *                /
		 *               DL
		 *              / \
		 *                 DLR
		 *
		 *        New root should be DL
		 *           *root <== this->left (i.e. DL)
		 *
		 *    2.2 else
		 *               P   |  P
		 *              / \  |   \
		 *        this(D)    |   (D)this
		 *            /      |   /
		 *           DL      |  DL
		 *                   |
		 *                (a)|(b)
		 *
		 *        (a) P->left  <== this->left (i.e. DL)
		 *        (b) P->right <== this->left (i.e. DL)
		 *
		 */

		if (ISROOT) {
			PROOT(this->left);
			*root = this->left;
		} else {
			if (ISLEFT)
				parent->left  = this->left;
			if (ISRIGHT)
				parent->right = this->left;
		}

	} else if (this->left == NULL && this->right != NULL) {
		PNODE("NON-LEAF:RTonly");
		/*
		 * 3. If this is a non-leaf node having right child only
		 *
		 *    3.1 If this is a root node,
		 *
		 *           (D)this
		 *             \
		 *              DR
		 *              /\
		 *                DRR
		 *
		 *        New root should be DR
		 *           *root <== this->right (i.e. DR)
		 *
		 *    3.2 else
		 *
		 *              P   |   P
		 *             / \  |  / \
		 *        this(D)   |    (D)this
		 *             \    |      \
		 *              DR  |       DR
		 *                  |
		 *               (a)|(b)
		 *
		 *        (a) P->left  <== this->right (i.e. DR)
		 *        (b) P->right <== this->right (i.e. DR)
		 *
		 */

		if (ISROOT) {
			PROOT(this->right);
			*root = this->right;
		} else {
			if (ISLEFT)
				parent->left  = this->right;
			if (ISRIGHT)
				parent->right = this->right;
		}

	} else { /* if (this->left != NULL && this->right != NULL) */
		PNODE("NON-LEAF:LFRT");
		/*
		 * 4. If this node is a non-leaf node having
		 *    both left and right children,
		 *
		 *    4.1 If this is a root node,
		 *                     (D)this
		 *                     /.\
		 *                    / . \
		 *                   /  .  \
		 *                  /   .   \
		 *                 DL   .    DR
		 *                / \   .   / \
		 *               /   \  .  /   \
		 *              DLL  DLR. DRL  DRR
		 *                      . ===
		 *                      .
		 *        MIN <---------+----------> MAX
		 *
		 *            o (D) < DR
		 *            o DL  < DRL
		 *
		 *        New root should be DR
		 *           *root      <== this->right (i.e. DR)
		 *            DRL->left <== this->left  (i.e. DL)
		 *
		 *    4.2 else
		 *
		 *        (a) if this is its parent's left child
		 *                         P
		 *                        / \
		 *                       /   \
		 *                  this(D)
		 *                     /.\
		 *                    / . \
		 *                   /  .  \
		 *                  /   .   \
		 *                 DL   .   DR
		 *                / \   .   / \
		 *               /   \  .  /   \
		 *              DLL DLR . DRL  DRR
		 *                      . ===
		 *                      .
		 *        MIN <---------+----------> MAX
		 *
		 *            o DL < (D) < P
		 *            o DL < DRL
		 *
		 *              P->left <== this->right (i.e. DR)
		 *            DRL->left <== this->left  (i.e. DL)
		 *
		 *        (b) if this is its parent's right child
		 *                   P
		 *                  / \
		 *                 /   \
		 *                     (D)this
		 *                     /.\
		 *                    / . \
		 *                   /  .  \
		 *                  /   .   \
		 *                 DL   .   DR
		 *                / \   .   / \
		 *               /   \  .  /   \
		 *              DLL DLR . DRL  DRR
		 *                      . ===
		 *                      .
		 *        MIN <---------+----------> MAX
		 *
		 *            o DR > (D) > P
		 *            o DL < DRL
		 *
		 *              P->right <== this->right (i.e. DR)
		 *            DRL->left  <== this->left  (i.e. DL)
		 *
		 */

		if (ISROOT) {
			PROOT(this->right);
			*root = this->right;
		} else {
			if (ISLEFT)
				parent->left  = this->right;
			if (ISRIGHT)
				parent->right = this->right;
		}

		/*
		 * Link (this->left) to left child of the Leftmost Leaf (LL)
		 * node of (this->right)
		 *
		 * NOTE: Don't have to check LL before using it because
		 *       it is impossible that LL is NULL as this->right
		 *       is not NULL obviously.
		 */
		bst_node_t *llp = bst_get_leftmost_leaf(this->right);
		llp->left = this->left;
	}

#define DELETE(p) do { BST_DESTROY_NODE(p); free(p); } while (0)
	DELETE(this);

	return 0;
}


// -3- 3.3 Search a BST node  --------------------------------------------------

/*
 * Search the node having key from BST recursively
 */
bst_node_t *
bst_search(bst_node_t *root, key_t key)
{
	DEBUG(printf("DEBUG1: %s\tis called (key=%3d)\n", __func__, key));

	if (root == NULL)
		return NULL;

	if (key == root->key) {
		DEBUG(printf("DEBUG2: key=%d %p %d %p %p\n",
		    key, root, root->key, root->left, root->right));
		return root;
	}

	if (key < root->key)
		return bst_search(root->left, key);
	else
		return bst_search(root->right, key);
}

/*
 * Search the node having key from BST non-recursively
 */
bst_node_t *
bst_search2(bst_node_t *root, key_t key)
{
	DEBUG(printf("DEBUG1: %s\tis called (key=%3d)\n", __func__, key));

	while (root != NULL) {
		if (key == root->key) {
			DEBUG(printf("DEBUG2: key=%d %p %d %p %p\n",
			    key, root, root->key, root->left, root->right));
			return root;
		}

		if (key < root->key)
			root = root->left;
		else
			root = root->right;
	}

	return NULL;
}

bst_node_t *
bst_search3(bst_node_t *root, key_t key, bst_node_t **parent)
{
	DEBUG(printf("DEBUG1: %s\tis called (key=%3d)\n", __func__, key));

	while (root != NULL) {
		if (key == root->key) {
			DEBUG(printf("DEBUG2: key=%d %p %d %p %p\n",
			    key, root, root->key, root->left, root->right));

			/* set the top root node */
			if (*parent == NULL)
				*parent = root;

			return root;
		}

		*parent = root;
		if (key < root->key)
			root = root->left;
		else
			root = root->right;
	}

	return NULL;
}

/*
 * Find the Kth node from BST, k = 1, 2, ...
 */
int
bst_find(bst_node_t *root, key_t *key, int k)
{
	if (root == NULL)
		return -1;

	if (root->left != NULL && k > 0)
		k = bst_find(root->left, key, k);

	DEBUG(printf("DEBUG: %p key=%d k=%d\n", root, root->key, k));
	if (--k == 0) {
		printf("\t\tFound %p key=%d <-- GOOD\n", root, root->key);
		*key = root->key;
		return 0;
	}

	if (root->right != NULL && k > 0)
		k = bst_find(root->right, key, k);

	return k;
}
