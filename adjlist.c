/******************PROGRAM INTRODUCTION*******************\
***********************            ***********************\
| Filename: adjlist.c                                      |
| Author: Joyceline Echondong                              |
| Algorithms:                                              |
| Tools: Ubuntu 14, gedit, gcc,                            |
| References:Algorithms and data structures notes          |
**********************************************************/
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

typedef struct Doc_node *link;/* This is a user-defined type for a pointer to a doc_node*/

/* We now define a Doc_node structure */
typedef struct Doc_node
{
  int docID;    /* Document Identity */
  int termID; /* term Identity */
  int tdf; /* term document frequency */
  int dtf; /* document term frequency */
  float tw; /* term weight */
  link next; /* pointer to the next node */
} doc_node;/* Instead of writing struct Doc_node all the time, we write doc_node */

#define MAXNODES 6890 /* Maximum number of documents in Adjacency list based on docsWithTerms.txt */
#define N 350000 /*  */

/********************************************************\
|******   FUNCTION AND GLOBAL VARIABLE DECLARATIONS   ****|
\********************************************************/

void populate_adjlist();
doc_node adjlist[MAXNODES]; /* This is an array of doc_nodes*/
FILE *docsWithTermsFilePtr, *outputFilePtr; /* Both of these files are global variables */
unsigned int num_node = 0; //indexes the nodes in the graph

/*************************************\
|*****  MAIN PROGRAM STARTS HERE  ****|
\*************************************/

int main (void)
{
    int id, s, k, temp_holder;
 
    /* Open the input and output files for processing  */
    docsWithTermsFilePtr = fopen("/home/Izakey/Documents/MmeJoyce/One/docsWithTerms8.txt", "r"); 
    outputFilePtr = fopen("/home/Izakey/Documents/MmeJoyce/One/OutputFile8.txt", "w+");   
    
    /* Call this function to read data from the input file docsWithTerms7.txt into the adjlist[] */
    populate_adjlist();
    
    /* For each document, this code prints the document term frequency
    and for each term, it prints the term document frequency and term_weight to the output file */
    
    for (k=0; k < MAXNODES;k++){
        doc_node *node;
        fprintf(outputFilePtr, "%d %d:", adjlist[k].docID, adjlist[k].dtf);
        temp_holder = adjlist[k].dtf;
        node = adjlist[k].next; 
        while(node != NULL){
	    node->tw = node->tdf * log2(N) / temp_holder; /* This is the term weight */
            fprintf(outputFilePtr,"%d %d %.3f;", node->termID, node->tdf, node->tw);
            node = node->next;
        }
	/* After each document and its terms, go to the next line */
        fprintf(outputFilePtr, "\n");
    }
}

/* We now define the populate_adjlist() function*/
void populate_adjlist()
{
    char ch;
    int val, count = 0;
    char tempch[10];
    doc_node *tempnode;
    int docID = 0;    /* Document Identity */
    int termID = 0; /* term Identity */
    int tdf = 0; /* term document frequency */
    int dtf = 0; /* document term frequency */
 
    
    while ((ch = getc(docsWithTermsFilePtr)) != EOF) {
        if(ch == '\0' || ch == '\t' || ch == '\r' || ch == ' ' || ch == ';' || ch == ':'){
            tempch[count] = '\0';
            if(val = strtol(tempch, NULL, 10)){
                if(docID == 0){
                    docID = val;
                    adjlist[num_node].docID = docID;
                    docID == 1;
                }else if(dtf == 0){
                    dtf = val;
                    adjlist[num_node].dtf = dtf;
                    adjlist[num_node].next = NULL;
                    tempnode = &adjlist[num_node];
                }else if(termID == 0){
                    if(tempnode->next == NULL){
                        tempnode->next =  (void*)malloc(sizeof(doc_node));
                        tempnode = tempnode->next;
                    }
                    tempnode->termID = val;
                    termID = 1;
                    tdf = 0;
                }else{
                    tempnode->tdf= val;
                    tdf = 1;
                    termID = 0;
                    tempnode->next = NULL;
                }
            }
            count = 0;
            continue;
        }
        
        if(ch == '\n'){
            num_node++;
            docID = 0;    /* Document Identity */
            termID = 0; /* term Identity */
            tdf = 0; /* term document frequency */
            dtf = 0; /* document term frequency */
            count = 0;
        }
        tempch[count] = ch;
        count++;      
    }  
}
