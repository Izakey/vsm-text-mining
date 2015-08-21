#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "create_inverted_file_index.h"

int main(int argc, char** argv) {
    FILE *postingsFilePtr,*docsFilePtr;
    FILE *termsFilePtr;
    int stoplist_cnt, i;
    int currentTermID = 0;
    char postingsFileNm[50] = "";
    char docsFileNm[50] = "";
    //char t[15];
    int a;
    /*for (a=1;a<50;a++){
        printf("%d%c ",a,a);
    }
    getchar();
     */

    //postingsFilePtr = fopen("termsWithDocs.txt", "w");
    //fclose(postingsFilePtr);
    termsFilePtr = fopen("/home/Izakey/Documents/MmeJoyce/One/ohsumed/terms.txt", "w");
    stoplist_cnt = getstoplist(stoplist);
    /*for (i=0;i<stoplist_cnt;i++){
    printf("$$$%s****",stoplist[i]);getchar();}*/
    /*printf("%d\n", stoplist_cnt);
    getchar();
    for (i = 0; i < stoplist_cnt; i++) {
        printf("%d %s\n",i+1,stoplist[i]);
    }getchar();*/
    /*    StopList_node stopListRoot = stopListNodeInit();
        for (i = 0; i < stoplist_cnt; i++) {
            //printf("%d %s %s\n",strlen(stoplist[i]),stoplist[i],"*");
            insertStopLististElement(stopListRoot, stoplist[i]);
        }*/
    //printf("\n%d %s\n",searchStopLististTree(stopListRoot, "c"),"@@@@####$$$$%%%%");
    //traverseStopListTree(stopListRoot);

    Dict_node dictRoot = dictNodeInit(&currentTermID);
    Term_Docs_Node termsDocsNode = termDocsNodeInit(0, 0);
    DocLL docsNode = NULL;
    initIntString();
    /*strcpy(t,"mno");
    insertDictElement(dictRoot,t);*/
    /*insertDictElement(dictRoot,"ghi");
    /*insertDictElement(dictRoot,"jkl");
    insertDictElement(dictRoot,"abc");
    insertDictElement(dictRoot,"def");
    insertDictElement(dictRoot,"abc");
     */

    //getDictTerms("ohsumed\\ohsumed87b.tar", &postingsFilePtr,&docsFilePtr,  dictRoot, &termsDocsNode, &docsNode, postingsFileNm, docsFileNm,stoplist, stoplist_cnt, &currentTermID);
    getDictTerms("/home/Izakey/Documents/MmeJoyce/One/ohsumed/ohsumed88.tar",&postingsFilePtr,&docsFilePtr,dictRoot,&termsDocsNode,&docsNode,postingsFileNm,docsFileNm,stoplist,stoplist_cnt,&currentTermID);
    getDictTerms("/home/Izakey/Documents/MmeJoyce/One/ohsumed/ohsumed89.tar",&postingsFilePtr,&docsFilePtr,dictRoot,&termsDocsNode,&docsNode,postingsFileNm,docsFileNm,stoplist,stoplist_cnt,&currentTermID);
    getDictTerms("/home/Izakey/Documents/MmeJoyce/One/ohsumed/ohsumed90.tar",&postingsFilePtr,&docsFilePtr,dictRoot,&termsDocsNode,&docsNode,postingsFileNm,docsFileNm,stoplist,stoplist_cnt,&currentTermID);
    getDictTerms("/home/Izakey/Documents/MmeJoyce/One/ohsumed/ohsumed91.tar",&postingsFilePtr,&docsFilePtr,dictRoot,&termsDocsNode,&docsNode,postingsFileNm,docsFileNm,stoplist,stoplist_cnt,&currentTermID);
    storeTerms(dictRoot, termsFilePtr);

    printf("%s\n", "Preparing to Store Postings");
    storePostings(termsDocsNode, postingsFilePtr);
    fclose(postingsFilePtr);
    printf("%s\n", "Postings stored");


    //ALSO STORE DOCUMENTS
    storeDocuments(docsNode,docsFilePtr);
    printf("%s\n", "Documents stored");
    
    return (EXIT_SUCCESS);
}

