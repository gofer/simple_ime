/******************************************************************************
 * [Title]
 *  My Original IME (global header)
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

#include <vector>
#include <list>
#include <map>
#include <cstring>

#define MAX_INT 0x7FFFFFFF
#define __DEBUG

const unsigned int BYTE_PAR_CHAR = 3;

typedef struct TrieLeaf{
	int leftID, rightID;
	int emissionCost;
	int reading_length;
	char* word;	
} TrieLeaf;

class TrieNode{
private:
	char ch[BYTE_PAR_CHAR+1];
	std::vector<TrieLeaf*> *leaves;
	std::list<TrieNode*> *children;

public:
	TrieNode();
	TrieNode(char*);
	char* getChar();
	void insertLeaf(int, int, int, const char*, int);
	TrieNode* insertChild(char*);
	std::vector<TrieLeaf*>* getLeaves();
	std::list<TrieNode*>* getChildren();
};

class Trie{
private:
	TrieNode *root;

	void _insert(TrieNode*, char*, int, int, int, char*, int);
	std::vector<TrieLeaf*>* _find(TrieNode*, const char*);
#ifdef __DEBUG
	void _display(TrieNode*, int);
#endif

public:
	Trie();
	void insert(char *, int, int, int, char*, int);
	std::vector<TrieLeaf*>* find(const char*);
#ifdef __DEBUG
	void display();
#endif
};

typedef struct _ViterbiEdge {
	int transition_cost;
	struct _ViterbiNode* prev;
	struct _ViterbiNode* next;
} ViterbiEdge;

typedef std::list<ViterbiEdge*> ViterbiEdgeList;

typedef struct _ViterbiNode {
	TrieLeaf *leaf;
	int leftPos;
	int minTotalCost;
	struct _ViterbiEdge* prevEdge;
} ViterbiNode;

typedef std::list<ViterbiNode*> ViterbiNodeList;
typedef std::vector<ViterbiNodeList*> ViterbiNodeListVector;

typedef std::vector<TrieLeaf*> TrieLeafVector; // Trieの葉にある変換候補のベクタ
typedef std::list<TrieLeafVector*> SubstringList; //n文字目から始まる部分文字列のリスト

typedef std::pair<int, int> key_type; 
typedef std::map<key_type, int> TransitionCostHashTable;

typedef std::vector<ViterbiNodeList*> ViterbiNodeListVector;
typedef std::list<ViterbiEdge*> ViterbiEdgeList;

std::vector<SubstringList*>* createSubstring(Trie, const char*);
std::vector<ViterbiNodeList*>* createNode(std::vector<SubstringList*>*);
ViterbiEdgeList* createEdge(ViterbiNodeListVector*, TransitionCostHashTable*);
bool setMinCost(ViterbiEdgeList*);
std::list<char*>* search();

#ifdef __DEBUG
void displayEdgeList(ViterbiEdgeList*);
#endif