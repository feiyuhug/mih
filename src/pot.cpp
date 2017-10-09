#include<stdio.h>
#include <time.h>
#include<algorithm>
#include<cstring>

#include "bitops.h"
#include "pot.h"


pot_node* buildPoTree(UINT8 *code_db, UINT8 *compcodes, UINT32 N, UINT32 dim1codes) {
	UINT64 code_dbBound = N * dim1codes;
	UINT64 compCodeInd8 = 0;
	pot_node *root = new pot_node(0);
	memset(compcodes, 0, sizeof(UINT8)*dim1codes);
	compCodeInd8 += dim1codes;
	root->left = new pot_node(0);
	UINT64 code_dbInd8;
	for(code_dbInd8 = dim1codes; code_dbInd8 < code_dbBound; code_dbInd8+=dim1codes) {
		pot_node *cur = root;
		int cur_od = 0;
		while(true) {
			pot_node *p = cur->left;
			int max_ic_od = -1;
			pot_node *max_ic_od_p = NULL;
			while(p) {
				int p_ic_od;
				if (p->left) {
					p_ic_od = match_po(compcodes + p->ind8, code_db + code_dbInd8, dim1codes);
				} else {
					p_ic_od = match_po(code_db + p->ind8, code_db + code_dbInd8, dim1codes);
				}
				if (max_ic_od < p_ic_od) {
					max_ic_od = p_ic_od;
					max_ic_od_p = p;
				}
				p = p->right;
			}
			if (max_ic_od <= cur_od) {
				pot_node *new_node = new pot_node(code_dbInd8);
				new_node->right = max_ic_od_p->right;
				max_ic_od_p->right = new_node;
				break;
			} else {
				if (max_ic_od_p->left) {
					cur_od = o_c(compcodes + max_ic_od_p->ind8, dim1codes);
				} else {
					cur_od = o_c(code_db + max_ic_od_p->ind8, dim1codes);
				}
				if (cur_od > max_ic_od || !(max_ic_od_p->left)) {
					pot_node *new_node = new pot_node(compCodeInd8);
					if (max_ic_od_p->left) {
						op_and(compcodes + max_ic_od_p->ind8, code_db + code_dbInd8, compcodes + compCodeInd8, dim1codes);
					} else {
						op_and(code_db + max_ic_od_p->ind8, code_db + code_dbInd8, compcodes + compCodeInd8, dim1codes);
					}
					compCodeInd8 += dim1codes;
					new_node->right = max_ic_od_p->right;
					new_node->left = max_ic_od_p;
					max_ic_od_p->right = new pot_node(code_dbInd8);
					if(cur->left == max_ic_od_p) {
						cur->left = new_node;
					} else {
						p = cur->left;
						while(p->right != max_ic_od_p) {
							p = p->right;
						}
						p->right = new_node;
					}
					break;
				} else {
					cur = max_ic_od_p;
				}
			}
		}
	}
	return root;
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
	pot_node *pot, UINT8 *queries, UINT32 d, UINT32 dim1codes, UINT32 K) {
	/**
	if (pot->left) {
		printf("--%08X%08X\n", ((UINT32*)(compcodes + pot->ind8))[0], ((UINT32*)(compcodes + pot->ind8))[1]);
	} else {
		printf("%08X%08X\n", ((UINT32*)(codes + pot->ind8))[0], ((UINT32*)(codes + pot->ind8))[1]);
	}
	**/
	if (*res_ind >= K) {
		//printf("res full...\n");
		return;
	}
	(*v_c)++;
	if(! pot->left) {
		int h_d = match(codes + pot->ind8, queries, dim1codes);
		if (h_d <= d) {
			res[*res_ind] = pot->ind8 / dim1codes;
			(*res_ind) ++;
		}
		return;
	}
	int o_de = order_de(compcodes + pot->ind8, queries, dim1codes);
	if(o_de <= d) {
		pot_node *child = pot->left;
		while(child) {
			pot_query(v_c, res, res_ind, codes, compcodes, child, queries, d, dim1codes, K);
			child = child->right;
		}
	}
	return;
}

void pot_query_batch(UINT32 **res, UINT32 *resn, UINT8 *codes, UINT8 *compcodes, pot_node *pot, UINT8 *queries, 
	UINT32 NQ, UINT32 d, UINT32 dim1codes, UINT32 K) {
	UINT64 VC = 0;
	UINT32 resn_ = 0;
	for (UINT32 q_ind = 0; q_ind < NQ; q_ind++) {
		UINT64 vc = 0;
		resn[q_ind] = 0;
		//printf("--------------\n");
		pot_query(&vc, res[q_ind], resn + q_ind, codes, compcodes, pot, queries+q_ind*dim1codes, d, dim1codes, K);
		//printf("result: %d\n", resn[q_ind]);
		VC += vc;
		resn_ += resn[q_ind];
	}
	printf("total visit %ld / %d\n", VC, NQ);
	printf("total return %d / %d\n", resn_, NQ);
	return;
}



