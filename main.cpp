/******************************************************************************
 * [Title]
 *  My Original IME (main program)
 *
 * [Author]
 *  gofer (@gofer_ex) <gofer.ex.studio@gmail.com>
 *    Department of Creative Informatics,
 *    Graduate School of Information Science and Technology,
 *    The University of Tokyo.
 * 
 * [Description]
 *  This program is made for the class of Creative Informatics Industrial
 *  Collaboration Program VII. 
 *  The lecture in this class is Mr. Kudo (Taku Kudo) of Google Japan Inc.
 *****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include "my_ime.h"

//const char* test_str = "けいえいりすくのさいしょうかをはかる";
//const char* test_str = "わたしはとうきょうだいがくだいがくいんのがくせいです。";
//const char* test_str = "くるまがくるまでまつ";
//const char* test_str = "わたしはがくせいです";

#ifdef __DEBUG
void put_leaves(TrieLeafVector *leaf_vector) {
	if(leaf_vector == NULL) return;
	TrieLeafVector::iterator itr = leaf_vector->begin();
	TrieLeafVector::iterator end = leaf_vector->end();
	for(; itr != end; ++itr)
		printf("%d\t%d\t%d\t%s\n", (*itr)->leftID, (*itr)->rightID, (*itr)->emissionCost, (*itr)->word);
}

void put_substr_list(SubstringList *substring_list) {
	if(substring_list == NULL) return;
	SubstringList::iterator itr = substring_list->begin();
	SubstringList::iterator end = substring_list->end();
	for(; itr != end; ++itr) {
		put_leaves(*itr);
		puts("---------------------------");
	}
}
#endif

std::list<char*>* convert(Trie *dictionary, TransitionCostHashTable *transisionCost, const char* str) {
	// 与えられた文字列から部分文字列を生成
	std::vector<SubstringList*>* substring_list_vector = new std::vector<SubstringList*>();
	substring_list_vector = createSubstring(*dictionary, str);
	//puts("Create Substring List\t\t\t\t\t[OK]");
	
	// 部分文字列をもとにノードを生成
	ViterbiNodeListVector* node_list_vector;
	node_list_vector = createNode(substring_list_vector);
	//puts("Create Viterbi Nodes\t\t\t\t\t[OK]");
	
	// エッジの作成
	ViterbiEdgeList *edge_list = createEdge(node_list_vector, transisionCost);
	//puts("Create Viterbi Edges\t\t\t\t\t[OK]");
	
	// 各ノードに最小コストを持たせる
	if(!setMinCost(edge_list)) return NULL;
	//puts("Set Minimum Cost\t\t\t\t\t[OK]");
	
	// 後ろからたどって文字列を取得する
	return search();
}

int main(int argc, char** argv) {
	FILE *fp;
	int left_id, right_id, cost;
	
	// TRIEによる辞書の作成
	Trie dictionary;
	char *file_name = (char*)malloc(0xFF);
	char *reading = (char*)malloc(0x07FF), *word = (char*)malloc(0x07FF);
	for(int i=0; i<10; ++i) {
		sprintf(file_name, "./resource/dictionary%02d.txt", i);
		//sprintf(file_name, "./resource/small_dictionary.txt");
		fp = fopen(file_name, "r");
		while(fscanf(fp, "%s\t%d\t%d\t%d\t%s", reading, &left_id, &right_id, &cost, word) != -1) {
			dictionary.insert(reading, left_id, right_id, cost, word, strlen(reading) / BYTE_PAR_CHAR);
		}
		fclose(fp);
	}
	free(reading);
	free(word);
	free(file_name);
	
	puts("Create TRIE Dictionary\t\t\t\t\t[OK]");
	
	// Transition CostのHash table作成
	TransitionCostHashTable transisionCost;
	fp = fopen("./resource/connection.txt", "r");
	fscanf(fp, "%d\t%d", &left_id, &right_id);
	while(fscanf(fp, "%d\t%d\t%d", &left_id, &right_id, &cost) != -1)
		transisionCost.insert(std::map<key_type, int>::value_type(key_type(left_id, right_id), cost));
	fclose(fp);
	
	puts("Create Transition Cost Hash Table\t\t\t[OK]");
	
	char* str = (char*)malloc(0xFFFF);
	while(true) {
		printf("ひらがな入力> ");
		fscanf(stdin, "%s", str);
		std::list<char*> *answer = convert(&dictionary, &transisionCost, str);
		
		if(answer == NULL) {
			puts("変換できませんでした。");
			continue;
		}
		
		std::list<char*>::iterator itr = answer->begin();
		const std::list<char*>::iterator end = answer->end();
		printf("｜");
		for(; itr != end; ++itr) {
			//char *buf = *itr
			printf("%s｜", *itr);
		}
		printf("\n\n");
	}
	
	return EXIT_SUCCESS;
}
