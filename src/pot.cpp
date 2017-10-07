#include<stdio.h>
#include <time.h>
#include<algorithm>

#include "bitops.h"
#include "pot.h"


pot_node* buildPoTree(UINT8 *code_db, UINT8 *compCode, UINT32 N, UINT32 dim1codes) {
	UINT64 code_dbBound = N * dim1codes;
	UINT64 compCodeInd8 = 0;
	pot_node *root = new pot_node(-1);
	root->left = new pot_node(0);
	UINT64 code_dbInd8;
	for(code_dbInd8 = dim1codes; code_dbInd8 < code_dbBound; code_dbInd8+=dim1codes) {
		pot_node *pre = NULL;
		pot_node *cur = root;
		bool is_left = true;
		int o_de = 0;
		while(o_de == 0) {
			int o_de_l, o_de_r;
			if (cur->left) {
				o_de_l = order_de(compCode + cur->left->ind8, code_db + code_dbInd8, dim1codes);
				o_de = o_de_l;
				is_left = true;
			} else {
				break;
			}
			if (cur->right) {
				o_de_r = order_de(compCode + cur->right->ind8, code_db + code_dbInd8, dim1codes);
				if (o_de_r < o_de_l) {
					o_de = o_de_r;
					is_left = false;
				}
			}
			pre = cur;
			if (is_left) {
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		
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
	return root->left;
}

void print_potree(pot_node* root) {
	if(root) {
		if(root->left) printf("--%ld\n", root->ind8);
		else printf("%ld\n", root->ind8);
	} else {
		return;
	}
	if(root->left) printf("{\n");
	print_potree(root->left);
	//if(root->left) printf("----------\n");
	print_potree(root->right);
	if(root->left) printf("}\n");
	return;
}

void pot_query(UINT64 *v_c, UINT32 *res, UINT32 *res_ind, UINT8 *codes, UINT8 *compcodes, 
	pot_node *pot, UINT8 *queries, UINT32 d, UINT32 dim1codes) {
	*v_c ++;
	if(! pot->left) {
		int h_d = match(codes + pot->ind8, queries, dim1codes);
		if (h_d <= d) {
			res[*res_ind] = pot->ind8 / dim1codes;
			*res_ind ++;
		}
		return;
	}
	int o_de = order_de(compcodes + pot->ind8, queries, dim1codes);
	if(o_de <= d) {
		pot_query(v_c, res, res_ind, codes, compcodes, pot->left, queries, d, dim1codes);
		pot_query(v_c, res, res_ind, codes, compcodes, pot->right, queries, d, dim1codes);
	}
	return;
}

void pot_query_batch(UINT32 **res, UINT32 *resn, UINT8 *codes, UINT8 *compcodes, pot_node *pot, UINT8 *queries, 
	UINT32 NQ, UINT32 d, UINT32 dim1codes) {
	UINT64 tmp = 0;
	UINT64 *v_c = &tmp;
	for (UINT32 q_ind = 0; q_ind < NQ; q_ind++) {
		pot_query(v_c, res[q_ind], resn + q_ind, codes, compcodes, pot, queries+q_ind*dim1codes, d, dim1codes);
	}
	printf("total visit %ld\n", *v_c);
	return;
}



