#ifndef STEMMER_H
#define	STEMMER_H

#ifdef	__cplusplus
extern "C" {
#endif


#define FALSE 0
#define TRUE 1
#define PARSIZE_MAX 20000
/*stemmer declarations*/
struct stemmer;

extern struct stemmer * create_stemmer(void);
extern void free_stemmer(struct stemmer * z);
extern int stem(struct stemmer * z,char word[200],int k);
/*extern int stem(struct stemmer * z, char * b, int k);
*/




/* stemmer is a structure for a few local bits of data,
*/

struct stemmer {
   char * b;       /* buffer for word to be stemmed */
   int k;          /* offset to the end of the string */
   int j;          /* a general offset into the string */
};



#ifdef	__cplusplus
}
#endif

#endif	/* STEMMER_H */
