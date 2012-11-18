/******************************************************************************
 * [Title]
 *  TRIE Data Structure
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

/*** TrieNodeクラス ***/
TrieNode::TrieNode() {
	leaves = new std::vector<TrieLeaf*>();
	children = new std::list<TrieNode*>();
	for(int i=0; i<BYTE_PAR_CHAR+1; ++i)
		ch[i] = '\0';
}

TrieNode::TrieNode(char* character) {
	leaves = new std::vector<TrieLeaf*>();
	children = new std::list<TrieNode*>();
	memmove(ch, character, BYTE_PAR_CHAR*sizeof(char));
	character[BYTE_PAR_CHAR] = '\0';
}

char* TrieNode::getChar() {
	return ch;
}

void TrieNode::insertLeaf(int l_id, int r_id, int e_cost, const char* wd, int rd_len) {
	TrieLeaf* new_leaf = (TrieLeaf*)malloc(sizeof(TrieLeaf));
	new_leaf->leftID = l_id;
	new_leaf->rightID = r_id;
	new_leaf->emissionCost = e_cost;
	new_leaf->reading_length = rd_len;
	int length = strlen(wd) + 1;
	new_leaf->word = (char*)malloc(length * sizeof(char));
	memmove(new_leaf->word, wd, length);	
	leaves->push_back(new_leaf);
}

TrieNode* TrieNode::insertChild(char* character) {
	TrieNode *insertNode = new TrieNode(character);
	children->push_back(insertNode);
	return insertNode;
}

std::vector<TrieLeaf*>* TrieNode::getLeaves() {
	return leaves;
}

std::list<TrieNode*>* TrieNode::getChildren() {
	return children;
}

/*** Trieクラス ***/
Trie::Trie() {
	root = new TrieNode();
}

void Trie::_insert(TrieNode* node, char* reading, int l_id, int r_id, int e_cost, char* wd, int rd_len) {
	std::list<TrieNode*> *children = node->getChildren();
	std::list<TrieNode*>::iterator itr = children->begin();
	std::list<TrieNode*>::iterator end = children->end();
	for(; itr != end; ++itr) {
		if(!strncmp((*itr)->getChar(), reading, BYTE_PAR_CHAR))
			break;
	}
	
	if(itr == end) {
		char ch[BYTE_PAR_CHAR + 1];
		memmove(ch, reading, BYTE_PAR_CHAR);
		ch[BYTE_PAR_CHAR] = '\0';
		TrieNode *newNode = node->insertChild(ch);
		if(reading[BYTE_PAR_CHAR] == '\0') {
			newNode->insertLeaf(l_id, r_id, e_cost, wd, rd_len);
			return;
		} else {
			return _insert(newNode, reading+BYTE_PAR_CHAR, l_id, r_id, e_cost, wd, rd_len);
		}
	} else {
		if(reading[BYTE_PAR_CHAR] == '\0') {
			(*itr)->insertLeaf(l_id, r_id, e_cost, wd, rd_len);
			return;
		} else {
			return _insert((*itr), reading+BYTE_PAR_CHAR, l_id, r_id, e_cost, wd, rd_len);
		}
	}
}

void Trie::insert(char *reading, int l_id, int r_id, int e_cost, char* wd, int rd_len) {
	return _insert(root, reading, l_id, r_id, e_cost, wd, rd_len);
}

std::vector<TrieLeaf*>* Trie::_find(TrieNode* node, const char* reading) {
	std::list<TrieNode*> *children = node->getChildren();
	std::list<TrieNode*>::iterator itr = children->begin();
	std::list<TrieNode*>::iterator end = children->end();
	for(; itr != end; ++itr) {
		if(!strncmp((*itr)->getChar(), reading, BYTE_PAR_CHAR))
			break;
	}
	
	if(itr != end) {
		if(reading[BYTE_PAR_CHAR] == '\0') {
			std::vector<TrieLeaf*>* leaves = (*itr)->getLeaves();
			if(leaves->size() != 0)
				return leaves;
			else
				return NULL;
		} else {
			return _find((*itr), reading+BYTE_PAR_CHAR);
		}
	}
	
	return NULL;
}

std::vector<TrieLeaf*>* Trie::find(const char* reading) {
	return _find(root, reading);
}

#ifdef __DEBUG
void Trie::_display(TrieNode* node, int tabs) {
	for(int i=0; i<tabs; ++i) putchar('\t');
	printf("%s\n", node->getChar());
	
	std::list<TrieNode*> *children = node->getChildren();
	std::list<TrieNode*>::iterator itr = children->begin();
	std::list<TrieNode*>::iterator end = children->end();
	for(; itr != end; ++itr)
		_display((*itr), tabs+1);
}

void Trie::display() {
	return _display(root, 0);
}
#endif
