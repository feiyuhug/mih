#include<stdio.h>
#include <time.h>

#include "bitops.h"
#include "pot.h"


pot_node* buildPoTree(UINT8 *code_db, UINT32 N, UINT32 dim1codes) {
	UINT64 code_dbBound = N * dim1codes;
	UINT8 *compCode = (UINT8*)malloc((size_t)N * dim1codes * sizeof(UINT8));
	UINT64 compCodeInd8 = 0;
	pot_node *root = new pot_node(-1);
	root->left = new pot_node(0);
	UINT64 code_dbInd8;
	for(code_dbInd8 = dim1codes; code_dbInd8 < code_dbBound; code_dbInd8+=dim1codes) {
		pot_node *pre = root;
		pot_node *cur = root->left;
		bool is_left = true;
		int o_de;
		do {
			UINT8 *db;
			if(cur->left) {
				db = compCode;
			} else {
				break
			}
			o_de = o_c(db + cur->ind8, dim1codes) - match_po(code_db + code_dbInd8, db + cur->ind8, dim1codes);
		} while(o_de == 0)
		pot_node *new_node = new pot_node(compCodeInd8);
		compCodeInd8 += dim1codes;
		new_node->left = cur;
		new_node->right = new pot_node(code_dbInd8);
		if (is_left) {
			pre->left = new_node;
		} else {
			pre->right = new_node;
		}
	}
}