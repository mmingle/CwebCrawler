#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ADDR_LENGTH 1000
/*
###############################################
 A node in our linked-list of web addresses
###############################################
 */
struct listNode{
  char addr[MAX_ADDR_LENGTH];
  int wordCount;
  int searchedTermCount[10];
  struct listNode *next;
};


struct trie;
/*
###############################################
 Returns 1 if the list starting at pNode contains the address addr,
 and returns 0 otherwise
###############################################
 */
int contains(const struct listNode *pNode, const char *addr);


/*
###############################################
 Inserts the address addr as a new listNode at the end of
 the list
############################################### 
*/
void insertBack(struct listNode *pNode, const char *addr);


/*
###############################################
 Prints the addresses from pNode to the end of the list,
 one on each line
############################################### 
*/
void printAddresses(const struct listNode *pNode);


/*
###############################################
 Frees the memory associated with this node and all subsequent nodes
###############################################
 */
void destroyList(struct listNode *pNode);


//make addresses?
/*
###############################################
..gets links
###############################################
*/
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

/*
###############################################
crawler..
###############################################
*/
int crawler(char* startAddr, int numHops, struct listNode *pListStart);


/*
###############################################
Make URL list
###############################################
*/
int makeUrlList(struct listNode *pListStart,char* initialList);


/*
###############################################
 int readURLfile(char* filename, char* URLlist)

 takes in two char*,
 first is the name of the file to be read,
 second is where the contents of the file will be written to
###############################################
*/

int readURLfile(char* filename, char* URLlist);



/*
###############################################
 Start Indexing..
###############################################
*/

int startIndexing(struct trie *root[],struct listNode *pListStart, char *webPageData,char *words);













