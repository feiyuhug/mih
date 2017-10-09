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
		if (left) delete left;
		if (right) delete right;
	}
};

#endif

pot_node* buildPoTree(UINT8 *code_db, UINT8 *compCode, UINT32 N, UINT32 dim1codes);

void pot_query(UINT64 *v_c, UINT32 *res, UINT32 *res_ind, UINT8 *codes, UINT8 *compcodes, 
	pot_node *pot, UINT8 *queries, UINT32 d, UINT32 dim1codes, UINT32 K);

void pot_query_batch(UINT32 **res, UINT32 *resn, UINT8 *codes, UINT8 *compcodes, pot_node *pot, UINT8 *queries, 
	UINT32 NQ, UINT32 d, UINT32 dim1codes, UINT32 K);

void print_potree(pot_node* root);

