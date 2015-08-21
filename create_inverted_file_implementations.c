#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "create_inverted_file_index.h"
#include "stemmer.h"

Dict_node dictNewNode(char * term, Dict_node l, Dict_node r, int tf, int df, int *id) {
    Dict_node x = malloc(sizeof *x);
    x -> term = malloc(sizeof (char) *(strlen(term) + 1));
    strcpy(x -> term, term);
    x -> left = l;
    x -> right = r;
    x -> tf = tf;
    x -> df = df;
    x -> termID = *id; //printf("%d %s\n",*id,term);getchar();
    (*id)++;
    return x;
}

void traverseTempNode(Cur_Doc_Dict_node node) {
    if (node == NULL) {
        return;
    }
    traverseTempNode(node->left);
    //printf("\n@@@@@@@@%s %d %d**********\n",node->term, node->termID,node->tf);//getchar();
    traverseTempNode(node->right);
}

void traverseTempNode2(DocLL node) {
    TermLL tll = node->doc.terms;
    while (tll != NULL) {
        //printf("@@@***@@@%d %d\n",node->doc.did,node->doc.terms->term.tid);
        tll = tll->next;
    }
}

Cur_Doc_Dict_node cur_doc_dictNewNode(char * term, Cur_Doc_Dict_node l, Cur_Doc_Dict_node r, int id, int tf, char *docID) {
    Cur_Doc_Dict_node x = malloc(sizeof *x);
    x -> term = malloc(sizeof (char) *(strlen(term) + 1));
    strcpy(x -> term, term);
    x -> left = l;
    x -> right = r;
    x -> tf = tf;
    x -> termID = id; //printf("%d %s\n",*id,term);getchar();
    return x;
}

Term_Docs_Node term_docsNewNode(char * term, Term_Docs_Node l, Term_Docs_Node r, Doc_StatsNode doc, int tid, int did) {
    Term_Docs_Node x = malloc(sizeof *x);
    x -> term = malloc(sizeof (char) *(strlen(term) + 1));
    strcpy(x -> term, term);
    x -> left = l;
    x -> right = r;
    x->tf = 1; //new term  ==> freq = 1
    x->df = 0; //doc freq will be updated after parsing all the terms of current document
    x -> termID = tid;
    //    if (doc == NULL)
    x -> doc = malloc(sizeof (*doc));
    (x->doc->d).doc_id = did;
    (x->doc->d).tf = 1;
    x->doc->next = NULL;
    //    else
    //        x -> doc = newDocNode(&doc, did);
    //x->doc = malloc(sizeof(* Doc_Stats));
    //(x ->doc)->doc_id = did;
    //(x->doc)->tf = tf;
    return x;
}

Doc_StatsNode newDocNode(Doc_StatsNode *head, int did) {
    Doc_StatsNode nnode;
    Doc_StatsNode tmp = *head;
    if (*head == NULL) {
        nnode = malloc(sizeof (*nnode));
        (nnode -> d).doc_id = did;
        (nnode -> d).tf = 1;
        nnode ->next = NULL;
        *head = nnode;
    }
        //at least one doc entry present for this term - traverse the linked list
    else {
        while (tmp -> next != NULL) {
            if ((tmp->d).doc_id == did) {
                (tmp->d).tf++;
                return *head;
            }
            tmp = tmp -> next;
        }
        //tmp->next is null - check tmp
        if ((tmp->d).doc_id == did) {
            (tmp->d).tf++;
            return *head;
        }

        //new document for this term
        Doc_StatsNode nnode = malloc(sizeof (*nnode));
        (nnode -> d).doc_id = did;
        (nnode -> d).tf = 1;
        nnode ->next = NULL;
        tmp->next = nnode;
    }
    return *head;
}

Term_Docs_Node termDocsNodeInit(int tid, int did) {
    return term_docsNewNode("", NULL, NULL, NULL, tid, did);
}

TermLL term_NewNode(int tid, int dtf) {
    TermLL x = malloc(sizeof (*x));
    x -> term.tid = tid;
    x -> term.dtf = dtf;
    x -> next = NULL;
    return x;
}

TermLL termNodeInit(TermLL tll, int tid, int dtf) {
    TermLL tmp = tll;
    TermLL tmp2 = term_NewNode(tid, dtf);
    if (tll == NULL)
        return tmp2;
    else {
        while (tmp->next != NULL)
            tmp = tmp -> next;
        tmp -> next = tmp2;
    }
    return tll;
}

DocLL doc_NewNode(int did) {
    DocLL x = malloc(sizeof (*x));
    x -> doc.did = did;
    x -> doc.dtf = 0;
    x ->doc.terms = NULL;
    x->next = NULL;
    return x;
}

DocLL docNodeInit(DocLL dll, int did) {
    DocLL tmp = dll;
    DocLL tmp2 = doc_NewNode(did);
    if (dll == NULL)
        return tmp2;
    else {
        while (tmp->next != NULL)
            tmp = tmp -> next;
        tmp -> next = tmp2;
        //return tmp->next;
        return tmp2;
    }

}

Dict_node dictNodeInit(int * id) {
    //head = (z = newNode(NULL, NULL, NULL, 0));
    return dictNewNode("", NULL, NULL, 0, 0, id);
}

Cur_Doc_Dict_node cur_doc_dictNodeInit(int id, char *docID) {
    return cur_doc_dictNewNode("", NULL, NULL, id, 0, docID);
}

Cur_Doc_Dict_node insertCurDocDictElementTID(Cur_Doc_Dict_node node, char * term, int id, char *docID) {
    //insert element to Tree in TID order

    if (node == NULL) {
        return cur_doc_dictNewNode(term, NULL, NULL, id, 1, docID);
    } else {
        //printf("\n%s %s %d %d\n",term,node->term,id, node->termID);getchar();
        if (id < node->termID) {
            node->left = insertCurDocDictElementTID(node->left, term, id, docID);
            return node;
        } else if (id > node->termID) {
            node->right = insertCurDocDictElementTID(node->right, term, id, docID);
            return node;
        } else if (id == node->termID) {
            node -> tf += 1;
            return node;
        }
    }
}

Cur_Doc_Dict_node insertCurDocDictElementAlpha(Cur_Doc_Dict_node node, char * term, int id, char *docID) {
    //insert element to Tree in alphabetical order
    if (node == NULL) {
        return cur_doc_dictNewNode(term, NULL, NULL, id, 1, docID);
    } else {
        if (strcmp(term, node->term) < 0) {
            //if (id < node->termID) {
            node->left = insertCurDocDictElementAlpha(node->left, term, id, docID);
            return node;
        } else if (strcmp(term, node->term) > 0) {
            node->right = insertCurDocDictElementAlpha(node->right, term, id, docID);
            return node;
        } else if (strcmp(term, node->term) == 0) {
            node -> tf += 1;
            return node;
        }
    }
}

Term_Docs_Node insertCurDocStats(Term_Docs_Node node, char * term, int tid, int did) {
    if (node == NULL) {
        return term_docsNewNode(term, NULL, NULL, NULL, tid, did);
    } else {
        if (strcmp(term, node->term) < 0) {
            //if (id < node->termID) {
            node->left = insertCurDocStats(node->left, term, tid, did);
            return node;
        } else if (strcmp(term, node->term) > 0) {
            node->right = insertCurDocStats(node->right, term, tid, did);
            return node;
        } else if (strcmp(term, node->term) == 0) {
            node -> tf += 1;
            node -> doc = newDocNode(&(node->doc), did);
            return node;
        }
    }
}

Dict_node insertDictElement(Dict_node node, char * term, int *id, int *idUsed) {
    //insert element to Tree in alphabetic order
    if (node == NULL) {

        return dictNewNode(term, NULL, NULL, 1, 0, id);
    } else {
        if (strcmp(term, node->term) < 0) {
            node->left = insertDictElement(node->left, term, id, idUsed);
            return node;
        } else if (strcmp(term, node->term) > 0) {
            node->right = insertDictElement(node->right, term, id, idUsed);
            return node;
        } else if (strcmp(term, node->term) == 0) {
            node -> tf += 1;
            *idUsed = node->termID;
            //printf("OOOOPS%s %d",node->term, node->termID);
            //if (strcmp(node->term,"accur")==0) getchar();
            //printf("Oops! the element is already present in the tree.");
            return node;
        }
    }
}

Dict_node searchDictTree(Dict_node node, char *key) {
    if (node == NULL)
        return NULL;
    else {
        if (strcmp(node->term, key) == 0) {
            return node;
        } else if (strcmp(node->term, key) > 0) {
            node = searchDictTree(node->left, key);
            return node;
        } else {
            node = searchDictTree(node->right, key);
            return node;
        }
    }
}

Term_Docs_Node searchPostingsDictTree(Term_Docs_Node node, char *key) {
    if (node == NULL)
        return NULL;
    else {
        if (strcmp(node->term, key) == 0) {
            return node;
        } else if (strcmp(node->term, key) > 0) {
            node = searchPostingsDictTree(node->left, key);
            return node;
        } else {
            node = searchPostingsDictTree(node->right, key);
            return node;
        }
    }
}


//record doc id against term in file

void insertDocID(FILE *postingsFilePtr, Cur_Doc_Dict_node node, int *lineNo, char *docID) {
    char line[20000];
    int c;
    int i; //, count = 1;

    //printf("%d\n",*lineNo);getchar();
    //printf("%s%d %s", "*****", node->termID, docID);getchar();
    if (node->termID == 0) //root of bst
        return;

    //if (*term_id < *lineNoOld) {
    /*    while (c = getc(postingsFilePtr) != EOF){
            while (c != '\n'){
                c = getc(postingsFilePtr);
            }
        
            //\n read
        }
        //this is a new term - insert it
        //printf("****\n");
        printf("%d%c%s%c\n", node->termID, ';', docID, ' ');
        fprintf(postingsFilePtr, "\n%d%c%s%c", node->termID, ';', docID, ' ');
     */
    //printf("%s%s%s %d %d","&&&&&",line,"@@@@@@@@@@@", count, node->termID);
    while ((fgets(line, 200, postingsFilePtr)) != NULL) {
        /*if (count > node->termID){ //we have not yet reached the line with term id
            printf("%d   %d\n",count,node->termID);
            continue;
        }*/
        //printf("%s%s%s %d %d\n","&&&&&",line,"@@@@@@@@@@@", *lineNo, node->termID);getchar();



        if (*lineNo == node->termID) {
            //we have now reached a line with term id
            //reposition file pointer just before the last CRLF
            //printf("%s%s%s %d %d", "&&&&&", line, "&&&****&&&", *lineNo, node->termID);
            //getchar();
            fseek(postingsFilePtr, -2, SEEK_CUR);
            fprintf(postingsFilePtr, "%s%c", docID, ' '); //getchar();//fflush(postingsFilePtr);getchar();
            return;
        }
        (*lineNo)++;
        //      printf(postingsFilePtr,"%d%c\n",*docID,' ');
    }
    //this is a new term - insert it
    //printf("****\n");
    //fgets(line, 20000, postingsFilePtr);
    //printf("%d%c%s%c\n", node->termID, ';', docID, ' ');
    fprintf(postingsFilePtr, "%d%c%s%c\n", node->termID, ';', docID, ' '); //fflush(postingsFilePtr);//getchar();
    //}
}

Term_Docs_Node storePostings(Term_Docs_Node node, FILE *termsFilePtr) {
    Doc_StatsNode tmp;
    if (node == NULL) {
        return;
    }
    storePostings(node->left, termsFilePtr);
    //printf("%s %d %d %d\n", node->term, node->termID, node->tf, node->df);getchar();

    if (strcmp(node->term, "") != 0) {
        fprintf(termsFilePtr, "%s:%d %d %d;", node->term, node->termID, node->tf, node->df); //fflush(termsFilePtr);//getchar();
        tmp = node -> doc;
        while (tmp != NULL) {
            fprintf(termsFilePtr, "%d %d;", tmp->d.doc_id, tmp->d.tf);
            tmp = tmp->next;
        }
        fprintf(termsFilePtr, "\n");
    }
    storePostings(node->right, termsFilePtr);
}

void storeDocuments(DocLL node, FILE *docsFilePtr) {
    DocLL dtmp = node;
    TermLL ttmp;

    while (dtmp != NULL) {
        //printf("%d %d:",dtmp->doc.did,dtmp->doc.dtf);
        fprintf(docsFilePtr, "%d %d:", dtmp->doc.did, dtmp->doc.dtf);
        ttmp = dtmp->doc.terms;
        while (ttmp != NULL) {
            if (ttmp->term.tid != 0)
                fprintf(docsFilePtr, "%d %d;", ttmp->term.tid, ttmp->term.dtf);
            ttmp = ttmp->next;
        }
        fprintf(docsFilePtr, "\n");
        dtmp = dtmp->next;
    }
}

Dict_node storeTerms(Dict_node node, FILE *termsFilePtr) {
    if (node == NULL) {
        return;
    }
    storeTerms(node->left, termsFilePtr);
    fprintf(termsFilePtr, "%s %d %d %d\n", node->term, node->termID, node->tf, node->df);
    storeTerms(node->right, termsFilePtr);
}


/**********************************************/ //int z;

int searchStopList(char stopList[STOPLIST_MAX][WORDSIZE_MAX], char * key, int l, int r) {
    int m;
    //char cmp[WORDSIZE_MAX];
    while (r >= l) {
        m = (l + r) / 2; //printf("%s %d %s %s\n",key, m,stopList[m], "###%%%%");getchar();
        if (strcmp(key, stopList[m]) == 0) return 1;
        if (strcmp(key, stopList[m]) < 0) r = m - 1;
        else l = m + 1;
    }
    return 0;
}

int getstoplist(char stoplist[STOPLIST_MAX][WORDSIZE_MAX]) {
    FILE *list;
    char word[WORDSIZE_MAX];
    char new_word[WORDSIZE_MAX];
    char c;
    int i, j, k;

    for (i = 0; i < STOPLIST_MAX; i++)
        stoplist[i][0] = 0;

    list = fopen(STOPLIST_FILE, "rb");
    i = 0;
    j = 0;
    while ((c = getc(list)) != EOF) {
        if ((c != '\n') && (j < WORDSIZE_MAX - 1)) { /*not end of line - continue reading*/
            if (c != '\r') //carriage return
                stoplist[i][j++] = c; //putchar(c);
            else j++;
            /*c = getc(list);*/
            //printf("%c %d %d\n",c,j,WORDSIZE_MAX);
            continue;

        } else {/*end of line - process current line and skip to next line*/
            stoplist[i][j] = 0;
            //printf("%d %s %s\n",strlen(stoplist[i]),stoplist[i],"00");getchar();
        }

        j = 0;
        i++;
    }
    fclose(list);
    //for (j=0;j<i;j++){
    //    printf("%d\n%s\n%d\n",i,stoplist[j],234);
    //}

    return i;
}

//get abstract terms

int getAbsTerms(FILE *sourceFile, FILE *postingsFilePtr, char *termsWithDocsFileName, Dict_node dnode, Term_Docs_Node termsDocsNode, DocLL * dll, DocLL *curDocLL, char stoplist[STOPLIST_MAX][WORDSIZE_MAX], int stopListCnt, int *term_id, char *docID, int did) {
    //getAbsTerms(file, postingsFilePtr,postingsFileNm, dnode, termsDocsNode,stoplist, stopListCnt, term_id, docID);
    //int getAbsTerms(FILE *sourceFile, FILE *postingsFilePtr,char *termsWithDocsFileName, Dict_node dnode, Term_Docs_Node termsDocsNode,char stoplist[STOPLIST_MAX][WORDSIZE_MAX], int stopListCnt, int *term_id, int did) {    
    char word[WORDSIZE_MAX];
    char c;
    int i, j;
    struct stemmer * z = create_stemmer();
    Cur_Doc_Dict_node curDocDicNodeAlpha = NULL;
    Cur_Doc_Dict_node curDocDicNodeTID = NULL;
    int termIDUsed, termIDNext;
    curDocDicNodeAlpha = cur_doc_dictNodeInit(0, "");
    curDocDicNodeTID = cur_doc_dictNodeInit(0, "");
    i = 0;
    j = 0;
    c = getc(sourceFile); //skip LF

    while ((c = getc(sourceFile)) != '\n') {
        j++;
        if ((c != ' ') && (j < WORDSIZE_MAX - 1)) { /*not end of word - continue reading*/
            /*if ((c>='0') && (c<='9') && (i>0) && (word[i-1]<'0') && (word[i-1]>'9')){ //digit preceded by word
                word[i] = 0;
                insertDictElement(dnode, word, term_id);
                i=0;
                word[i++] = c; //store digit that was read
            }
            if ((c == '.') && (word[i-1]>='0') && (word[i-1]<='9')){
                word[i++] = tolower(c);
                continue;
            }
            else */
            //is this a breaking character
            if (((c >= '0') && (c <= '9')) || (c == '$') || (c == '%') || (c == '*') || (c == '+') || (c == '-') || (c == '.') || (c == ':') || (c == '_') || (c == '-') || (c == '"') || (c == '/') || (c == ';') || (c == '\'') || (c == ',') || (c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == '=') || (c == '/') || (c == ',') || (c == '?') || (c == '&') || (c == '#') || (c == '!') || (c == '<') || (c == '>')) {
                word[i] = 0;
                word[stem(z, word, strlen(word) - 1) + 1] = 0;
                //new

                i = 0;
                j = 0;
                //printf("%s %s%s%s\n", "Word: ", "&&&***", word, "&&&***");
                if (!(searchStopList(stoplist, word, 0, stopListCnt - 1))) {
                    //printf("&&&&%s&&&&\n",word);getchar();
                    termIDUsed = *term_id;
                    insertDictElement(dnode, word, term_id, &termIDUsed); //printf("%d %s\n",*term_id,word);getchar();
                    //printf("\n%d %s %s\n", *term_id, word, "inserted");
                    //printf("\n%d %s %s\n", termIDUsed, word, "inserted");//getchar();
                    //termIDUsed
                    insertCurDocDictElementAlpha(curDocDicNodeAlpha, word, termIDUsed, docID);

                    insertCurDocDictElementTID(curDocDicNodeTID, word, termIDUsed, docID);
                    //traverseTempNode(curDocDicNodeTID);//getchar();
                    insertCurDocStats(termsDocsNode, word, termIDUsed, did);
                    //traverseTempNode(curDocDicNodeAlpha);
                    //printf("%s %s %d", "dictnode", dnode->term, dnode->tf);
                    //printf("%s %s %d", "curnode", curDocDicNodeAlpha->term, curDocDicNodeAlpha->tf);
                    //getchar();
                    //i = 0;   statement changed to comment because of changes above
                    strcpy(word, "");
                    continue;
                }
                strcpy(word, "");
            }/*else if ((c == '[')){
                    //while ((c = getc(file)) != ']')
                        //word[i++]=tolower(c);
                
                }
                else if (c == '('){ 
                    //process the word before ( if there is no space before (
                    //*******
                
                }*/

                //not a breaking character - read next character



            else {
                word[i++] = tolower(c);
                continue;
            }
        } else /*end of word encountered*/

            word[i] = 0;
        word[stem(z, word, strlen(word) - 1) + 1] = 0;
        //printf("Word: %s\n", word);
        if (!(searchStopList(stoplist, word, 0, stopListCnt - 1))) { //accept only terms not in the stop list
            //printf("&&&&%s&&&&\n",word);getchar();
            termIDUsed = *term_id;
            insertDictElement(dnode, word, term_id, &termIDUsed);
            //printf("\n%d %s %s\n", termIDUsed, word, "inserted");//getchar();
            //printf("\n%d %s %s\n", *term_id-1, word, "inserted");//getchar();
            insertCurDocDictElementAlpha(curDocDicNodeAlpha, word, termIDUsed, docID);
            insertCurDocDictElementTID(curDocDicNodeTID, word, termIDUsed, docID);
            //traverseTempNode(curDocDicNodeTID);//getchar();
            insertCurDocStats(termsDocsNode, word, termIDUsed, did);
            //traverseTempNode(curDocDicNodeAlpha);
            /*printf("%s%s%d", "dictnode", dnode->term, dnode->tf);
            printf("%s%s%d", "curnode", curDocDicNodeAlpha->term, curDocDicNodeAlpha->tf);
            getchar();*/
        }

        //printf("\n");
        //storeTerms(dnode);
        i = 0;
        j = 0;
        strcpy(word, "");
    }
    free(z);
    //record on disk this document id for all the terms that are in the document
    //FILE * postingsFilePtr = fopen(termsWithDocsFileName, "r+");

    //update dictionary with current document stats
    updateTermDocFreq(dnode, curDocDicNodeAlpha);
    updatePostingsTermDocFreqAlpha(termsDocsNode, curDocDicNodeAlpha, did);

    //printf("%d %d\n",curDocDicNodeTID->termID,curDocDicNodeTID->tf);getchar();
    *curDocLL = docNodeInit(*dll, did);
    if (*dll == NULL) {
        *dll = *curDocLL;
    }

    /*    *curDocLL = docNodeInit(*dll,did, curDocDicNodeTID->tf);
            if (*dll == NULL) {
     *dll = *curDocLL;
            
            }*/

    //printf("%s %s %d %d\n","***&&&^^^%%%",curDocDicNodeTID->term,curDocDicNodeTID->tf,curDocDicNodeAlpha->tf);


    //traverseTempNode(curDocDicNodeTID);//getchar();
    //printf("###$$$$%d***&&&\n",curDocLL);getchar();
    updateDocumentList(*curDocLL, curDocDicNodeTID); //getchar();
    //traverseTempNode2(*curDocLL);//getchar();
}

void updateTermDocFreq(Dict_node dnode, Cur_Doc_Dict_node curDocDicNodeAlpha) {
    Dict_node d;
    if (curDocDicNodeAlpha == NULL) {
        //printf("End\n");
        return;
    }
    updateTermDocFreq(dnode, curDocDicNodeAlpha -> left);
    d = searchDictTree(dnode, curDocDicNodeAlpha -> term);
    if (d != NULL) {
        ///printf("%s %s %d\n", "####@@@@", d->term, d->df);getchar();
        (d -> df)++;
    }
    //traverseTempNode(curDocDicNodeAlpha);getchar;
    updateTermDocFreq(dnode, curDocDicNodeAlpha -> right);
}

void updatePostingsTermDocFreqAlpha(Term_Docs_Node tdnode, Cur_Doc_Dict_node curDocDicNodeAlpha, int did) {
    Term_Docs_Node d;
    Doc_StatsNode ds;
    if (curDocDicNodeAlpha == NULL) {
        return;
    }
    updatePostingsTermDocFreqAlpha(tdnode, curDocDicNodeAlpha -> left, did);
    d = searchPostingsDictTree(tdnode, curDocDicNodeAlpha -> term);
    if (d != NULL) {
        (d -> df)++;
        ds = d->doc; //looks like this line is redundant
    }
    updatePostingsTermDocFreqAlpha(tdnode, curDocDicNodeAlpha -> right, did);
}

void updatePostingsTermDocFreqTID(Term_Docs_Node tdnode, Cur_Doc_Dict_node curDocDicNodeAlpha, int did) {
    Term_Docs_Node d;
    Doc_StatsNode ds;
    if (curDocDicNodeAlpha == NULL) {
        return;
    }
    updatePostingsTermDocFreqTID(tdnode, curDocDicNodeAlpha -> left, did);
    d = searchPostingsDictTree(tdnode, curDocDicNodeAlpha -> term);
    if (d != NULL) {
        (d -> df)++;
        ds = d->doc;
    }
    updatePostingsTermDocFreqTID(tdnode, curDocDicNodeAlpha -> right, did);
}

void updateDocumentList(DocLL curDocLL, Cur_Doc_Dict_node curDocDicNodeTID) {
    TermLL ttmp1, ttmp2;
    if (curDocDicNodeTID == NULL) {
        return;
    }

    updateDocumentList(curDocLL, curDocDicNodeTID -> left);
    ttmp2 = term_NewNode(curDocDicNodeTID->termID, curDocDicNodeTID->tf);
    curDocLL->doc.dtf += curDocDicNodeTID->tf;

    if (curDocLL->doc.terms == NULL) {
        curDocLL->doc.terms = ttmp2;
    } else {
        ttmp1 = curDocLL->doc.terms;
        while (ttmp1->next != NULL)
            ttmp1 = ttmp1->next;
        ttmp1->next = ttmp2;
    }
    updateDocumentList(curDocLL, curDocDicNodeTID -> right);
}

void initIntString(void) {
    int i;
    for (i = 0; i <= 9; i++)
        intStrings[i] = malloc(1 + sizeof (char));
    intStrings[0] = "0";
    intStrings[1] = "1";
    intStrings[2] = "2";
    intStrings[3] = "3";
    intStrings[4] = "4";
    intStrings[5] = "5";
    intStrings[6] = "6";
    intStrings[7] = "7";
    intStrings[8] = "8";
    intStrings[9] = "9";
    for (i = 10; i <= 20; i++)
        intStrings[i] = malloc(2 * sizeof (char) + 1);
    intStrings[10] = "10";
    intStrings[11] = "11";
    intStrings[12] = "12";
    intStrings[13] = "13";
    intStrings[14] = "14";
    intStrings[15] = "5";
    intStrings[16] = "16";
    intStrings[17] = "17";
    intStrings[18] = "18";
    intStrings[19] = "19";
    intStrings[20] = "20";
}

void getDictTerms(char *sourceDocFileName, FILE **postingsFilePtr, FILE **docsFilePtr, Dict_node dnode, Term_Docs_Node *termsDocsNode, DocLL * docLL, char postingsFileNm[50], char docsFileName[50], char stoplist[STOPLIST_MAX][WORDSIZE_MAX], int stopListCnt, int *term_id) {
    FILE *file;
    //FILE *postingsFilePtr = NULL;
    //Term_Docs_Node termsDocsNode = NULL;
    //char postingsFileNm[20];
    DocLL curDocLL = *docLL;
    int postingsFileCnt, docsFileCnt;
    char word[WORDSIZE_MAX];
    char new_word[WORDSIZE_MAX];
    char postingsFileCntStr[WORDSIZE_MAX];
    char docsFileCntStr[WORDSIZE_MAX];
    char c;
    int i, j, k, doc_id;
    char tag, docID[10], medlineID[10], source[75], title[250], pubType[25];
    file = fopen(sourceDocFileName, "r");
    //postingsFilePtr = fopen(termsFile,"r+");
    i = 0;
    j = 0;
    //skip to first full-stop
    *docLL = NULL;
    while ((c = getc(file)) != '.') {
        ;
    }
    tag = getc(file);
    while (1) {
        //printf("%c %s\n",tag,"&*&*&*&*&*&*&*&");
        switch (tag) {
            case 'I': //docID
            {
                //printf("******NEW DOC ID*********");
                c = getc(file); //skip blank
                i = 0;
                while ((c = getc(file)) != '\n') {
                    docID[i++] = c;
                }
                docID[i] = 0; //printf("%s\n",docID);getchar();
                doc_id = atoi(docID);
                //docsFileCnt = postingsFileCnt = doc_id;
                if ((doc_id - 1) % MAX_POSTINGS_SIZE == 0) {

                    
                    //if (strcmp(postingsFileNm, "") == 0)
                        postingsFileCnt = 1 + doc_id / MAX_POSTINGS_SIZE;
                    //if (strcmp(docsFileName, "") == 0){
                        docsFileCnt = 1 + doc_id / MAX_POSTINGS_SIZE;
                        
                    //}
                    
                    
                    sprintf(postingsFileCntStr, "%d", postingsFileCnt);
                    strcpy(postingsFileNm, "termsWithDocs");
                    strcat(postingsFileNm, postingsFileCntStr);
                    strcat(postingsFileNm, ".txt"); //printf("\n***%s***\n",postingsFileNm);
                    *postingsFilePtr = fopen(postingsFileNm, "w");
                    
                    sprintf(docsFileCntStr, "%d", docsFileCnt);
                    strcpy(docsFileName, "docsWithTerms");
                    strcat(docsFileName, postingsFileCntStr);
                    strcat(docsFileName, ".txt"); //printf("\n***%s***\n",docsFileName);
                    *docsFilePtr = fopen(docsFileName, "w");
                    
                    
                    //printf("%s\n%s\n",postingsFileNm,docsFileName);
                    //if (strcmp(postingsFileNm, "") != 0) {//printf("%%%&&&***++++\n");
                        //*postingsFilePtr = fopen(termsWithDocsFileName, "r+");
                        printf("%s\n", "Preparing to Store Postings");
                        storePostings(*termsDocsNode, *postingsFilePtr);
                        fclose(*postingsFilePtr);
                        printf("%s\n", "Postings stored"); //getchar();
                    //}
                    //if (strcmp(docsFileName, "") != 0) {
                        //*docsFilePtr = fopen(termsWithDocsFileName, "r+");
                        storeDocuments(*docLL, *docsFilePtr);
                        printf("%s\n", "Documents stored");
                        fclose(*docsFilePtr);
                    //}



                    //postingsFileCnt = 1 + doc_id / MAX_POSTINGS_SIZE;
                    
                    //fclose(*postingsFilePtr);
                    //curDocDicNodeAlpha = cur_doc_dictNodeInit (0,"");
                    *termsDocsNode = termDocsNodeInit(0, 0);
                    //docsFileCnt = 1 + doc_id / MAX_POSTINGS_SIZE;
                    
                    *docLL = NULL;

                }
                getc(file);
                tag = getc(file);
                break;
            }
            case 'U': //MEDLINE identifier
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    medlineID[i++] = c;
                }
                medlineID[i] = 0;
                getc(file);
                tag = getc(file);
                break;
            }
            case 'S': //Source journal
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    source[i++] = c;
                }
                source[i] = 0;
                getc(file);
                tag = getc(file);
                break;
            }
            case 'M': //human-assigned MESH terms - skip them 
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    ;
                }
                getc(file);
                tag = getc(file);
                break;
            }
            case 'T': //title
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    title[i++] = c;
                }
                title[i] = 0; //printf("%s\n",title);getchar();
                getc(file);
                tag = getc(file);
                break;
            }
            case 'P': //publication type
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    pubType[i++] = c;
                }
                pubType[i] = 0;
                getc(file);
                tag = getc(file);
                break;
            }
            case 'W': //abstract
            {
                //get abstract text

                //printf("******NEW ABSTRACT*********");
                //getchar();

                getAbsTerms(file, *postingsFilePtr, postingsFileNm, dnode, *termsDocsNode, docLL, &curDocLL, stoplist, stopListCnt, term_id, docID, doc_id);
                //printf("%s\n", "Terms collected");
                getc(file);
                tag = getc(file);
                break;
            }
            case 'A': //authors
            {
                c = getc(file); //skip LF
                i = 0;
                while ((c = getc(file)) != '\n') {
                    ;
                }
                getc(file);
                tag = getc(file);
                break;
            }
            default: //keep skipping until document ID found
            {

                while ((c = getc(file)) != '.')
                    ;
                tag = getc(file);
            }
        }
        j++;
        if ((tag == EOF)) {// || (j > 4773)) {//storeTerms(dnode);
            //printf("%s%c\n",")(*&^%$#@!",tag);
            break;
        }
    }
}
