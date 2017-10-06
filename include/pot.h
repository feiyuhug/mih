#include "types.h"

#ifndef POT_NODE_ 
#define POT_NODE_

struct pot_node {
	UINT64 ind8;
	struct pot_node *left;
	struct pot_node *right;
	pot_node(UINT64 ind8) :
	ind8(ind8), left(NULL), right(NULL) {}
	~pot_node() {
		delete left;
		delete right;
	}
};

#endif

pot_node* buildPoTree(UINT8 *code_db, UINT32 N, UINT32 dim1codes);

