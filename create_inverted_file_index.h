#include "common.h"
#define STOPLIST_MAX 600
#define MAX_POSTINGS_SIZE 10000
#define STOPLIST_FILE "stoplist.txt"
#define LINE1MAX 1000000
#define LINE2MAX 1000000
#define LINE3MAX 2000000

typedef struct dict_node * Dict_node; // Dict_node is a class of pointers to dict_node
typedef struct cur_dict_node * Cur_Doc_Dict_node; // Cur_Doc_Dict_node is a class of pointers tp cur_dict_node
typedef struct term_docs_node * Term_Docs_Node; // Term_Docs_Node is a class of pointers to term_docs_node
typedef struct doc_stats_node * Doc_StatsNode; // Doc_StatsNode is a class of pointers to doc_stats_node
typedef struct term_ll * TermLL; // TermLL is a class of pointers to term_ll
typedef struct doc_ll * DocLL; // DocLL is a class of pointers to doc_ll

char * intStrings[21];

// A matrix of stop words - high frequency words in a document which don't help in describing it
char stoplist[STOPLIST_MAX][WORDSIZE_MAX]; 

//typedef struct stopListNode StopList_node;
/* Dictionary node contains 
 - term // A word
 - term_frequency (tf) // Occurence of term in the documents list
 - document_frequency (df) // Number of documents the term occurs in
 - termID //
 - left // Pointer to left subtree
 - right // Pointer to right subtree
*/
struct dict_node {
    char * term;
    Dict_node left, right;
    int tf, df, termID;
};

/* docStats contains
 - doc_id // The document's id
 - tf // The number of times the term occurs in that document
*/
struct docStats{
    int doc_id, tf;
};

/*
 Used to update df in dict_node and 

*/
struct cur_dict_node {
    char * term;
    Cur_Doc_Dict_node left, right;
    int tf, termID;
};

/* term_docs_node contains
 - term // A word
 - tf // The term's frequency
 - df // The number of documents in which the term occurs
 - termID // The word's ID
 - left // Pointer to it's left subtree of 
 - right // Pointer to it's right subtree of
 - doc // Pointer to postings list of documents containing the term
*/
struct term_docs_node {
    char * term;
    Term_Docs_Node left, right;
    int tf, df, termID;
    Doc_StatsNode doc;
};


struct doc_stats_node {
    struct docStats d;
    Doc_StatsNode next;
};
struct doc_term{
    int tid;
    int dtf;
};
struct term_ll{
    struct doc_term term;
    TermLL next;
};
struct document{
    int did;
    int dtf;
    TermLL terms;
};

/*
 - 
*/
struct doc_ll{
    struct document doc;
    DocLL next;
};

Dict_node dictNewNode(char * term, Dict_node l, Dict_node r, int tf, int df, int *id);
void traverseTempNode(Cur_Doc_Dict_node node);
Cur_Doc_Dict_node cur_doc_dictNewNode(char * term, Cur_Doc_Dict_node l, Cur_Doc_Dict_node r, int id, int tf, char *docID);
Term_Docs_Node term_docsNewNode(char * term, Term_Docs_Node l, Term_Docs_Node r, Doc_StatsNode doc, int tid, int did);
Doc_StatsNode newDocNode(Doc_StatsNode *head, int did);
Term_Docs_Node termDocsNodeInit(int tid, int did);
Dict_node dictNodeInit(int * id);
Cur_Doc_Dict_node cur_doc_dictNodeInit(int id, char *docID);
Cur_Doc_Dict_node insertCurDocDictElementAlpha(Cur_Doc_Dict_node node, char * term, int id, char *docID);
Cur_Doc_Dict_node insertCurDocDictElementTID(Cur_Doc_Dict_node node, char * term, int id, char *docID);
Term_Docs_Node insertCurDocStats(Term_Docs_Node node, char * term, int tid, int did);
Dict_node insertDictElement(Dict_node node, char * term, int *id, int *);
Dict_node searchDictTree(Dict_node node, char *key);
Term_Docs_Node searchPostingsDictTree(Term_Docs_Node node, char *key);
void insertDocID(FILE *postingsFilePtr, Cur_Doc_Dict_node node, int *lineNo, char *docID);
Term_Docs_Node storePostings(Term_Docs_Node node, FILE *termsFilePtr);
Cur_Doc_Dict_node storePostingsX(Cur_Doc_Dict_node node, FILE *postingsFilePtr, char *docID);
Dict_node storeTerms(Dict_node node, FILE *termsFilePtr);

int searchStopList(char [STOPLIST_MAX][WORDSIZE_MAX], char * key, int l, int r);

//int searchStopListTree(StopList_node node, char *key);
int getstoplist(char [STOPLIST_MAX][WORDSIZE_MAX]);
int getAbsTerms(FILE *sourceFile, FILE *postingsFilePtr, char *termsWithDocsFileName, Dict_node dnode, Term_Docs_Node termsDocsNode, DocLL *,DocLL *,char [STOPLIST_MAX][WORDSIZE_MAX], int stopListCnt, int *term_id, char *docID, int did);
void updateTermDocFreq(Dict_node dnode, Cur_Doc_Dict_node curDocDicNode);
void updatePostingsTermDocFreqAlpha(Term_Docs_Node tdnode, Cur_Doc_Dict_node curDocDicNode, int);
void updatePostingsTermDocFreqTID(Term_Docs_Node tdnode, Cur_Doc_Dict_node curDocDicNode, int);
void updateDocumentList(DocLL dll, Cur_Doc_Dict_node curDocDicNode);
void initIntString(void);
void getDictTerms(char *sourceDocFileName, FILE **,FILE **, Dict_node dnode, Term_Docs_Node *termsDocsNode, DocLL *,char postingsFileNm[50], char docsFileNm[50],char [STOPLIST_MAX][WORDSIZE_MAX], int stopListCnt, int *term_id);
DocLL doc_NewNode(int did);
DocLL docNodeInit(DocLL,int did);
TermLL term_NewNode(int tid,int dtf);
TermLL termNodeInit(TermLL tll, int tid, int dtf);
void storeDocuments(DocLL node, FILE *docsFilePtr);
