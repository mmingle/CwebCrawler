#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
###############################################
//Lexographical trie
###############################################
*/
struct trie{
char* addr;
//26 children one for every letter of the alphabet
struct trie *child[26];
//count keeps track of how many times each word occures, also signifies the end of a complete word
int count;
//each node holds a letter that make up the words.
char letter;

double wordCount;
double termcount[10];
double tfScore[10];
double idfScore[10];
double sScore;
};
/*
###############################################
Creates a new trie
###############################################
*/
struct trie *newtrie(char newchar,char* newAddr);






/*
###############################################
Adds words to trie structure
###############################################
*/
int addWordOccurrence(const char* word,struct trie *root);


/*
###############################################
prints the words in the trie,
 and how many times each one occures
###############################################
*/
void printTrieContents(struct trie* root);


/*
###############################################
Frees the entire Trie 
by recursively iterating through each of the
root node's child nodes and 
deleting the low nodes first, one by one
###############################################
*/
int freeTrieMemory(struct trie* root);



/*
###############################################
method that gets all the words from 
the specified webpage
###############################################
*/
int getText(const char* srcAddr, char* buffer, const int bufSize);


/*
###############################################
Parses webpage data
###############################################
*/
char* parseData(char* parsed,char* webPageData);

/*
###############################################
calculates idf score..
###############################################
*/
void idf(struct trie *root[],int *argCount);

/*
###############################################
...Not Sure What this Does
TODO: figure out what this does.
pretty sure it does nothing but ill leave it here
###############################################
*/
void s(struct trie *root[],int *argCount);


/*
###############################################
calculates term frequency
###############################################
*/
int tf(struct trie *root, char **queryTerms);


/*
###############################################
 gets query String
###############################################
*/

int query(char **words, int *argCount, char **usefulQT);


/*
###############################################
 calculates final score
###############################################
*/
int sScore(struct trie *root[],int *argCount, int *numberOfPagesIndexed);






