/******************************************************************************
 * [Title]
 *  Viterbi Node and Edges
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

ViterbiNode *BOS, *EOS;
unsigned int stringLength;

unsigned int unusedID = MAX_INT-1;

SubstringList* _createSubstring(Trie dictionary, char* str) {
	SubstringList *substring_list = new SubstringList();
	TrieLeafVector* leaves;
	
	int length = strlen(str) + 1;
	char temp;
	
	for(int i=BYTE_PAR_CHAR; i<=length; i+=BYTE_PAR_CHAR) {
		temp = str[i];
		str[i] = '\0';
		
		leaves = dictionary.find(str);
		// もし辞書にない文字が入力されたら
		if((strlen(str)==BYTE_PAR_CHAR) && (leaves == NULL)) {
			leaves = new TrieLeafVector();
			TrieLeaf* leaf = new TrieLeaf;
			leaf->leftID = leaf->rightID = --unusedID;
			leaf->emissionCost = MAX_INT / stringLength;
			leaf->reading_length = strlen(str) / 3;
			leaf->word = (char*)malloc(strlen(str)+1);
			memmove(leaf->word, str, strlen(str)+1);
			leaves->push_back(leaf);
		}
		substring_list->push_back(leaves);
		
		str[i] = temp;
	}
	
	return substring_list;
}

std::vector<SubstringList*>* createSubstring(Trie dictionary, const char* str) {
	int length = strlen(str) + 1;
	stringLength = (length - 1) / BYTE_PAR_CHAR;
	
	char *buf = (char*)malloc(length * sizeof(char));
	memmove(buf, str, length);
	
	std::vector<SubstringList*>* substring_list_vector = new std::vector<SubstringList*>();
	for(int i=0; i<length-1; i+=BYTE_PAR_CHAR) {
		substring_list_vector->push_back(_createSubstring(dictionary, buf+i));
	}
	
	free(buf);
	
	return substring_list_vector;
}

bool viterbiNodeCompare(ViterbiNode *a, ViterbiNode *b) {
	if((a == NULL) || (b == NULL)) return false;
	
	if(a->leaf->emissionCost == b->leaf->emissionCost) {
		if(a->leaf->reading_length == b->leaf->reading_length) {
			if(a->leaf->leftID == b->leaf->leftID) {
				return a->leaf->rightID < b->leaf->rightID;
			}
			else return a->leaf->leftID < b->leaf->leftID;
		}
		else return a->leaf->reading_length < b->leaf->reading_length;
	}
	else return a->leaf->emissionCost < b->leaf->emissionCost;
}

ViterbiNodeList* _createNode(TrieLeafVector *leaf_vector, unsigned int i) {
	if(leaf_vector == NULL) return NULL;
	ViterbiNodeList* tempNodeList = new ViterbiNodeList();
	
	TrieLeafVector::iterator itr = leaf_vector->begin();
	TrieLeafVector::iterator end = leaf_vector->end();
	for(; itr != end; ++itr) {
		ViterbiNode *node = new ViterbiNode;
		node->leaf = *itr;
		node->leftPos = i + 1;
		node->minTotalCost = MAX_INT;
		node->prevEdge = NULL;
		tempNodeList->push_back(node);
	}
	
	return tempNodeList;
}

ViterbiNodeList* _createNode(SubstringList* substr_list, unsigned int i) {
	ViterbiNodeList* node_list = new ViterbiNodeList();
	SubstringList::iterator itr = substr_list->begin();
	const SubstringList::iterator end = substr_list->end();
	
	for(; itr != end; ++itr) {
		ViterbiNodeList* temp_node_list;
		temp_node_list = _createNode(*itr, i);
		if(temp_node_list != NULL)
			node_list->merge(*temp_node_list, viterbiNodeCompare);
	}
	
	return node_list;
}

std::vector<ViterbiNodeList*>* createNode(std::vector<SubstringList*>* substring_list_vector) {
	std::vector<ViterbiNodeList*> *node_list_vector = new std::vector<ViterbiNodeList*>();
	
	unsigned int i = 0;
	std::vector<SubstringList*>::iterator itr = substring_list_vector->begin();
	const std::vector<SubstringList*>::iterator end = substring_list_vector->end();
	for(; itr != end; ++itr)
		node_list_vector->push_back(_createNode(*itr, i++));
	
	return node_list_vector;
}

void createBOSandEOS(void) {
	TrieLeaf *leaf = new TrieLeaf;
	leaf->leftID = 0;
	leaf->rightID = 0;
	leaf->emissionCost = 0;
	leaf->word = NULL;
	
	BOS = new ViterbiNode;
	BOS->leaf = leaf;
	BOS->leftPos = 0;
	BOS->minTotalCost = 0;
	BOS->prevEdge = NULL;
	
	EOS = new ViterbiNode;
	EOS->leaf = leaf;
	EOS->leftPos = stringLength;
	EOS->minTotalCost = MAX_INT;
	EOS->prevEdge = NULL;
}

bool ViterbiEdgeCompare(ViterbiEdge *a, ViterbiEdge *b) {
	if((a == NULL) || (b == NULL)) return false;
	
	const int aCost = a->transition_cost + a->next->leaf->emissionCost + a->prev->leaf->emissionCost;
	const int bCost = b->transition_cost + b->next->leaf->emissionCost + b->prev->leaf->emissionCost;
	
	if(aCost == bCost) {
		if(a->prev->leaf->reading_length == b->prev->leaf->reading_length) {
			return a->next->leftPos < b->next->leftPos;
		}
		else return a->prev->leaf->reading_length < b->prev->leaf->reading_length;
	}
	else return aCost < bCost;
}

void displayEdgeList(ViterbiEdgeList *list) {
	ViterbiEdgeList::iterator itr = list->begin();
	const ViterbiEdgeList::iterator end = list->end();
	for(; itr != end; ++itr) {
		printf("%d\t%s -> %s [%d ---(%d)--> %d :: %d] (%d)\n", 
			(*itr)->prev->leftPos,
			((*itr)->prev != BOS) ? (*itr)->prev->leaf->word : "BOS",
			((*itr)->next != EOS) ? (*itr)->next->leaf->word : "EOS",
			(*itr)->prev->leaf->emissionCost,
			(*itr)->transition_cost,  
			(*itr)->next->leaf->emissionCost,
			(*itr)->transition_cost + (*itr)->next->leaf->emissionCost/* + (*itr)->prev->leaf->emissionCost*/,
			(*itr)->next->minTotalCost
		);
	}
}

#ifdef __DEBUG
ViterbiEdgeList* _createEdge(ViterbiNode *parent, int str_pos, ViterbiNodeListVector* node_list_vector, TransitionCostHashTable* transisionCost) {
	if(str_pos > stringLength) return NULL;
	ViterbiEdgeList *edge_list = new ViterbiEdgeList();
	
	ViterbiNodeList* test = (*node_list_vector)[str_pos];
	ViterbiNodeList::iterator itr = test->begin();
	const ViterbiNodeList::iterator end = test->end();
	for(; itr != end; ++itr) {
		ViterbiEdge *edge = new ViterbiEdge;
		edge->prev = parent;
		edge->next = *itr;
		edge->transition_cost = (*transisionCost)[key_type(parent->leaf->rightID, (*itr)->leaf->leftID)];
		edge_list->push_back(edge);
		
		// 文末のノードならEOSを持たせる
		if(str_pos + (*itr)->leaf->reading_length == stringLength) {
			ViterbiEdge *eos_edge = new ViterbiEdge;
			eos_edge->prev = *itr;
			eos_edge->next = EOS;
			eos_edge->transition_cost = (*transisionCost)[key_type((*itr)->leaf->leftID, 0)];
			edge_list->push_back(eos_edge);
		}
	}
	
	return edge_list;
}
#endif

ViterbiEdgeList* createEdge(ViterbiNodeListVector* node_list_vector, TransitionCostHashTable* transisionCost) {
	ViterbiEdgeList *edge_list = new ViterbiEdgeList(), *temp_list;
	createBOSandEOS();
	
	temp_list = _createEdge(BOS, 0, node_list_vector, transisionCost);
	if(temp_list != NULL)
		edge_list->merge(*temp_list);
	
	const int length = stringLength - 1;
	for(int i=0; i<length+1; ++i) {
		ViterbiNodeList* parent_list = (*node_list_vector)[i];
		ViterbiNodeList::iterator itr = parent_list->begin();
		const ViterbiNodeList::iterator end = parent_list->end();
		for(; itr != end; ++itr) {
			if(i+((*itr)->leaf->reading_length) <= length) {
				temp_list = _createEdge(*itr, i+((*itr)->leaf->reading_length), node_list_vector, transisionCost);
				if(temp_list != NULL)
					edge_list->merge(*temp_list);
			}
		}
	}
	
	edge_list->sort(ViterbiEdgeCompare);
	
	return edge_list;
}

ViterbiEdgeList* selectEdgeWithTrieNode(ViterbiEdgeList *edge_list, ViterbiNode* node) {
	if((edge_list == NULL) || (node == NULL)) return NULL;
	
	ViterbiEdgeList *return_list = new ViterbiEdgeList();
	ViterbiEdgeList::iterator itr = edge_list->begin();
	const ViterbiEdgeList::iterator end = edge_list->end();
	for(; itr != end; ++itr) {
		if((*itr)->prev == node)
			return_list->push_back(*itr);
	}
	
	return_list->sort(ViterbiEdgeCompare);
	
	return return_list;
}

bool _setMinCost(ViterbiEdgeList *edge_list, ViterbiNode* node, int prev_cost) {
	ViterbiEdgeList* list = selectEdgeWithTrieNode(edge_list, node);
	if((list == NULL) || (list->size() == 0)) return false;
	
	ViterbiEdgeList::iterator itr = list->begin();
	const ViterbiEdgeList::iterator end = list->end();
	for(; itr != end; ++itr) {
		if((*itr)->next == NULL) return false;
		
		int minCostCondidate = (*itr)->next->leaf->emissionCost + (*itr)->transition_cost + prev_cost;
		if(minCostCondidate < (*itr)->next->minTotalCost) {
			(*itr)->next->minTotalCost = minCostCondidate;
			(*itr)->next->prevEdge = *itr;
			_setMinCost(edge_list, (*itr)->next, (*itr)->next->minTotalCost);
		}
	}
	
	return true;
}

bool setMinCost(ViterbiEdgeList *edge_list) {
	return _setMinCost(edge_list, BOS, BOS->leaf->emissionCost);
}

void _search(ViterbiNode* node, std::list<char*> *answer) {
	if((node == NULL) || (answer == NULL)) return;
	
	ViterbiNode* prevNode = node->prevEdge->prev;
	
	if((prevNode != BOS) && (prevNode != NULL)) {
		_search(prevNode, answer);
		
		if(prevNode->leaf->word != NULL) {
			//puts(prevNode->leaf->word);
			answer->push_back(prevNode->leaf->word);
		}
	}
}

std::list<char*>* search() {
	std::list<char*> *answer = new std::list<char*>();
	
	//printf("BOS: %08X, EOS: %08X\n", BOS, EOS);
	//printf("[BOS] leftPos: %d, minTotalCost: %d, leftID: %d, rightID: %d, emissionCost: %d\n", BOS->leftPos, BOS->minTotalCost, BOS->leaf->leftID, BOS->leaf->rightID, BOS->leaf->emissionCost);
	//printf("[EOS] leftPos: %d, minTotalCost: %d, leftID: %d, rightID: %d, emissionCost: %d\n", EOS->leftPos, EOS->minTotalCost, EOS->leaf->leftID, EOS->leaf->rightID, EOS->leaf->emissionCost);
	
	//printf("stringLength: %d\n", stringLength);
	
	_search(EOS, answer);
	return answer;
}
