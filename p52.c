#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ADDR_LENGTH 1000

/*
 * a node in our linked-list of web addresses
 */


struct listNode{
  char addr[MAX_ADDR_LENGTH];
  int wordCount;
  int searchedTermCount[10];
  struct listNode *next;
};
//our trie structure 
struct trie{
char* addr;
//26 children one for every letter of the alphabet
struct trie *child[26];
//count keeps track of how many times each word occures, also signifies the end of a complete word
int count;
//each node holds a letter that make up the words.
char letter;

int wordCount;
int termcount[10];
double tfScore[10];
double idfScore[10];
double sScore;
};

//function that creates a new trie node.
//sets the initial count, letter of the trie, and puts it on the heap.
struct trie *newtrie(char newchar,char* newAddr) {
    struct trie *newTrie =malloc(sizeof(struct trie));
    memset(newTrie, 0, sizeof(struct trie));
    newTrie->letter=newchar;
    newTrie->count=0;
    newTrie->wordCount=0;
int i;
for(i=0;i<10;i++){
 newTrie->tfScore[i]=0.0;
 newTrie->idfScore[i]=0.0;

}
    newTrie->sScore=0.0;
    newTrie->addr=newAddr;
    return newTrie;
}
void s(struct trie *root[],int *argCount);
void idf(struct trie *root[],int *argCount);
//method declarations
//adds words to trie structure
int addWordOccurrence(const char* word,struct trie *root);
//prints the words in the trie, and how many times each one occures
void printTrieContents(struct trie* root);
//frees all the trie nodes on the heap
int freeTrieMemory(struct trie* root);
//method that gets all the words from the specified webpage
int getText(const char* srcAddr, char* buffer, const int bufSize);
//sorts through all the characters that getText(); pulled from the webpage
char* parseData(char* parsed,char* webPageData);

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr);

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr);

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode);

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode);
  

int getLink(const char* srcAddr, char* link, const int maxLinkLength);


/*
###############################################
MAIN
###############################################
*/
int main(int argc, char** argv){
  
/*error handeling makes sure the correct number of arguments are given at the command line.*/
	if(argc != 2){
	fprintf(stderr,"USAGE: 'URL'\n");
	return 0;
	}


printf("Indexing...\n");

char *urlList=malloc(1000);
readURLfile(argv[1], urlList);
struct listNode *pListStart= malloc(sizeof(struct listNode));

/*
method that takes in character string of webpages and number of hopps, 
and a linked list to put each webpage in.
*/
makeUrlList(pListStart,urlList);

struct trie *root[50];
char* words=malloc(300000);
char* webPageData=malloc(300000);
int *wordCount=(int *)malloc(sizeof(int));
int *argCount=(int *)malloc(sizeof(int));

 
int counter=0;


while(pListStart != NULL){ 
root[counter]=newtrie(0,pListStart->addr);

getText(pListStart->addr, webPageData,300000);
printf("%s\n",pListStart->addr);

parseData(words,webPageData);

addWordOccurrence(words,root[counter]);

root[counter]->wordCount=*wordCount;
free(words);
free(webPageData);
words=malloc(300000);
webPageData=malloc(300000);
counter++;
pListStart=pListStart->next;
}


*argCount=counter-1;
printf("begin while...");
while(query(root,argCount)){
}
printf("end if...");


int ss=0;
while(root[ss]!=NULL){
printf("%s\n",root[ss]->addr);
printf("tfscore%f\n",root[ss]->tfScore[0]);
printf("idfscore%f\n",root[ss]->idfScore[0]);
printf("termcount%d\n",root[ss]->termcount[0]);
printf("wordcount%d\n\n",root[ss]->wordCount);
ss++;
}

/*
/*

idf(root,argCount);


ss=0;
while(root[ss]->addr!=NULL){
printf("%s\n",root[ss]->addr);
printf("tfscore%f\n",root[ss]->tfScore[0]);
printf("idfscore%f\n",root[ss]->idfScore[0]);
printf("termcount%d\n",root[ss]->termcount[0]);
printf("wordcount%d\n\n",root[ss]->wordCount);
ss++;
}

*/
/*
double score1;
double score2;

int m=0;
int n;
while(root[m+1]->addr != NULL){
printf("tfscore=%f\n",root[m]->tfScore[n]);
printf("idfscore=%f\n",root[m]->Score[n]);


for(n=0;n<*argCount;n++){
score1=root[m]->tfScore[n];
printf
score2=root[m]->idfScore[n];
root[m]->sScore+=score1*score2;
}
m++;
}

*/

free(argCount);
free(urlList);
free(words);
free(webPageData);
destroyList(pListStart);
 
return 0;

}












void idf(struct trie *root[],int *argCount){
//inverse document frequency
/*
IDF(t): the inverse document frequency is the natural logarithm of
[(1.0 + number of indexed documents) / (1.0 + number of indexed documents that contain t at least
once)]. The C standard library provides the "log" function to compute the natural logarithm.
*/

int counter;
int i;
int j;
int counter2=*argCount;
double idfScore;
int numberOfIndexedPages=0;
int termPageOcc[10];
double losingmymind;

while(root[numberOfIndexedPages]->addr != NULL){
numberOfIndexedPages++;
printf("numberofindexedpages= %d",numberOfIndexedPages);
}
printf("argcount/counter2= %d",counter2);

for(i=0;i<counter2;i++){
for(j=0; j<numberOfIndexedPages;j++){
if(root[j]->termcount[i]>0){
termPageOcc[i]++;
}
}

}

for(i=0;i<counter2;i++){
for(j=0; j<numberOfIndexedPages;j++){
losingmymind=(1+numberOfIndexedPages)/(1+termPageOcc[i]);
idfScore=log(losingmymind);

root[j]->idfScore[i]=idfScore;
 
}

}

int k;
int l;
double score1;
double score2;

for(k=0; k<numberOfIndexedPages;k++){
for(l=0;l<counter2;l++){


score1=root[k]->idfScore[l];
printf("----------%f\n",score1);
score2=root[k]->tfScore[l];
printf("-----------%f\n",score2);
root[k]->sScore+=score1*score2;

}
}



}

int query(struct trie *root[],int *argCount1){
char query[120];
char words[20][20];
int row=0;
int col=0;
int i=0;
int j;
char ch;
int argCount=1;
double tfscore=0.0;
double idfscore=0.0;
int anothercount=0;

printf("Enter a web query: ");

struct trie *temp;

while ((ch = getchar()) != '\n'){
        query[i] = ch;
	query[i+1]='\0';
	i++;
}
printf("Query is \"%s\"\n" ,query);

int length= strlen(query);

for(j=0;j<length;j++){

if(isupper(query[j])){
printf("Please enter a query containing only lower-case letters.\n");
return -1;
}

if(query[j] == ' '){
argCount++;
words[row][col]='\0';
row++;
col=0;
j++;
}
words[row][col]=query[j];
words[row][col+1]='\0';
col++;
}


int a=0;
int acounter=0;
int newrow;
int newcol;



double totalOcc;
double termOcc;
//while(root[a]->addr !=NULL){
for(a=0;a<*argCount1;a++){
printf("%s\n",root[a]->addr);
totalOcc=root[a]->wordCount;
temp=root[a];
for(newrow=0;newrow<argCount;newrow++){
int tLength=strlen(words[newrow]);
//printf("length of first arg is %d \n",tLength);

for(newcol=0;newcol<tLength;newcol++){
acounter=words[newrow][newcol]-97;
//printf("acounter=%d\n",acounter);
termOcc=0;
if(root[a]->child[acounter]!=NULL){
//printf("root[a]->child[acounter]->letter = %c \n",root[a]->child[acounter]->letter);
root[a]=root[a]->child[acounter];
termOcc=root[a]->count;
}else{
termOcc=0;
}



}
printf(" -- %f/%f \n",termOcc,totalOcc);
root[a]=temp;
//only ex once per term
tfscore = termOcc/totalOcc;
root[a]->tfScore[newcol]=tfscore;
printf("tfscore= %f\n",root[a]->tfScore[newcol]);
root[a]->termcount[newcol]=termOcc;
termOcc=0;
}
//a++;
}

//*argCount1=argCount;
return 0;

}




/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);
  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}


/*
################################################################################################
This method addWordOccurrence is new from project4 might have slight difference from the old one.
################################################################################################
*/

int addWordOccurrence(const char* word,struct trie *root)
{
	int printtab=0;
	/*set initroot to root so we can hop back up to the top of the trie when we need to*/	
    struct trie *initRoot=root;
    //Length of the parsed words.
    int wordLength=strlen(word);

    int i;
    for(i=0; i<wordLength; i++)
    {  	
		if(printtab==0)
		{
			printf("\t");
			printtab=1;
		}

		//if word[i]=a letterPlace=0,b=1,c=2 etc
		int letterPlace=word[i]-97;

		//if nothing at word[i] we break out.
		if(word[i]=='\0')
		{
			break;
		}
		printf("%c", word[i]);
		/*the way parseData is written says that if word[i] is '\n' then thats the end of a word*/
        if(word[i]=='\n')
        {
			//so we back up to the last letter of the word
			letterPlace=word[i-1]-97;
			//increment the count saying that it is the end of the word and keeping track of occurances
			root->count++;
			//jump back to the top of the trie
            root=initRoot;
			printtab=0;
			continue;
			//if the root->child[letterPlace] is NULL we create one
        }
		else if (root->child[letterPlace] == NULL)
			root->child[letterPlace] = newtrie(word[i],NULL);
			
	/*here we set the new root to be where the child is this is how we traverse down the trie setting the correct nodes*/
    root=root->child[letterPlace];
    }
    return 0;
}

/*
################################################################################
this parseData method is new taken from project4 it may need modification
################################################################################
*/

  char* parseData(char* parsed,char* webPageData)
	{
      char* acceptable = "abcdefghijklmnopqrstuvwxyz\0"; //All acceptable chars.
      int counter = 0; //The iterator through the unparsed string.
      int placeHolder = 0; //The iterator through the parsed string.
	  
      for (counter; counter < 300000; counter++)
	  {
		//If the page has ended, nothing left to read. End function.
		if (webPageData[counter] == '\0')
		{
			break;
		}
		//Test if there is punctuation inside a string.
		if((webPageData[counter] == ',' || webPageData[counter] == '.'|| webPageData[counter] == '-'|| webPageData[counter] == '_') && parsed[placeHolder - 1] != '\n')
		{
			parsed[placeHolder] = '\n';
			placeHolder++;
		}
		//Test if there is a number inside the string.
		if((isdigit(webPageData[counter])) && parsed[placeHolder - 1] != '\n')
		{
			parsed[placeHolder] = '\n';
			placeHolder++;
		}
		//If the current char is a space or newline AND the previous character was not a newline, add a new line.
		if ( (webPageData[counter] == '\n' || webPageData[counter] == ' ') && parsed[placeHolder - 1] != '\n')
		{
			//Insert a newline in the parsed string.
			parsed[placeHolder] = '\n'; 
			placeHolder++; 
		}

		//If the current character is found in the list of acceptable strings.
		if (strchr(acceptable, tolower(webPageData[counter])) != NULL)
		{ 
			parsed[placeHolder] = tolower(webPageData[counter]);
			placeHolder++;
		}
      }
	  //Return the parsed string.
      return parsed;
    }










int makeUrlList(struct listNode *pListStart,char* initialList){
char* word=malloc(1000);
int numHops;
char hops;
int i;
int k=0;
int j=100;

for(i=0;i<j && initialList[i] != '\0';i++){
if(initialList[i]==' '){
i++;
hops=atoi(&initialList[i]);
i+=2;
crawler(word,hops,pListStart);
k=0;
free(word);
word=malloc(1000);
}
word[k]=initialList[i];
k++;
}
free(word);


return 0;
}


int crawler(char* startAddr, int numHops, struct listNode *pListStart){
  long seed;
 char destAddr[MAX_ADDR_LENGTH];
  int hopNum;
  
  /* initialization */
    seed = time(NULL);
    //printf("URL=%s seed = %ld\n",startAddr, seed);
 
 srand(seed);
insertBack(pListStart, startAddr);
if(pListStart == NULL){
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return -2;
  }

  /* start the crawling */
  for(hopNum=1; hopNum <= numHops; hopNum++){
    int res = getLink(startAddr, destAddr, MAX_ADDR_LENGTH);

    if(!res){
     // printf("Dead end on hop %d: no outgoing links\n", hopNum);
      break;
    }

    if(contains(pListStart, destAddr)){
   // printf("Cycle detected on hop %d: address %s\n", hopNum,destAddr);
      hopNum--; // try again for this hop in the next iteration
    }else
	{
      insertBack(pListStart, destAddr);  
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH);  
    }
  }

  return 0;

}





int readURLfile(char* filename, char* URLlist){

//counter for reading in URL list
int i=0;
char c;
//open file from command line  
FILE *fp = fopen(filename,"r");
//error handeling
if(fp == NULL){
fprintf(stderr, "Could not open file (%s).\n", filename);
return -1;
}else{
//reads the list from the file into *text
while((c=getc(fp)) != EOF){
URLlist[i]=c;
i++;
URLlist[i]='\0';
}
}
//closes the file
fclose(fp);
}




/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
	

	if(pNode == NULL){
	return 0;
	}	

	if(strcmp(pNode->addr, addr)==0){
		return 1;
         }else{ 

	return 0+contains(pNode->next, addr);
         } 
	
}

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr){


if(pNode->next != NULL){

insertBack(pNode->next, addr);

}else{

struct listNode *last=malloc(sizeof(struct listNode));
strncpy(last->addr, addr, strlen(addr)+1);
pNode->next=last;
last->next=NULL;


}

}


/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */

void makeAddressList(const struct listNode *pNode,char* list){
if(pNode !=NULL){
	printf("%s\n",pNode->addr);
	printAddresses(pNode->next);
	}

}
void printAddresses(const struct listNode *pNode){
	if(pNode !=NULL){
	printf("%s\n\n",pNode->addr);
	printAddresses(pNode->next);
	}
}







/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode){
 

if(pNode != NULL){
destroyList(pNode->next);
free(pNode);
}

}
  






int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
	break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
	*pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}


//temp temporarily holds the word that will be printed
char temp[256];
void printTrieContents(struct trie* root){
  /*checks if the current root has a count, if its greater than 0 then it is a complete word. and prints it along with the count*/
  if (root->count > 0)
   printf("%s: %d\n", temp, root->count);
  //i is used to iterate through the current root's children
  int i;
  for(i=0; i<26;i++){
    if(root->child[i]!=NULL){
      //iOriginalLength holds the length for the word in temp
      int iOriginalLength = strlen( temp );
      temp[ iOriginalLength ] = root->child[i]->letter;
      temp[ iOriginalLength + 1 ] = '\0';
      printTrieContents(root->child[i]);
    }
  }
  temp[strlen(temp) - 1] = '\0';
}


int freeTrieMemory(struct trie* root){
  /*Frees the entire Trie by recursively iterating through each of the
    root node's child nodes and deleting the low nodes first, one by one*/
  int nodeLetter;
  for(nodeLetter = 0; nodeLetter < 26; nodeLetter++){
    if(root->child[nodeLetter]!=NULL){
      freeTrieMemory(root->child[nodeLetter]);
      free(root->child[nodeLetter]);
    }
  }

}
















/*
VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
SCRAP METHODS
VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
*/


/*
####################################################################################
old method addWordOccurrence
####################################################################################
*/


/*
int addWordOccurrence(const char* word,struct trie *root,int *wordCount)
{
int count=0;
printf("%s\n",root->addr);
//set initroot to root so we can hop back up to the top of the trie when we need to	
    struct trie *initRoot=root;
    
    //length of the parsed words
    int wordLength=strlen(word);
    //i is just used to iterate through all the chars in the list words
    int i;
    for(i=0; i<wordLength; i++)
    {  
	//if word[i]=a letterPlace=0,b=1,c=2 etc
        int letterPlace=word[i]-97;
   //this prints the words out so we know what info the trie is getting
	printf("%c", word[i]);
	//if nothing at word[i] we break out.
	if(word[i]=='\0'){
	break;
	}
	//the way parseData is written says that if word[i] is '\n' then thats the end of a word
        if(word[i]=='\n')
        {count++;
	//so we back up to the last letter of the word
	    letterPlace=word[i-1]-97;
  //increment the count saying that it is the end of the word and keeping track of occurances
	    root->count++;
	//jump back to the top of the trie
            root=initRoot;
	    printf("\t");
	    continue;
//if the root->child[letterPlace] is NULL we create one
        }else if (root->child[letterPlace] == NULL)
        root->child[letterPlace] = newtrie(word[i],NULL);
	//here we set the new root to be where the child is this is how we traverse down the trie setting the correct nodes
    root=root->child[letterPlace];
    }
    printf("\n");
*wordCount=count;
    return 0;
}
*/





/*
#################################################################
Old parseData method
#################################################################
*/

/*
// TODO: define the functions corresponding to the above prototypes 
    char* parseData(char* parsed,char* webPageData){
      char* acceptable = "abcdefghijklmnopqrstuvwxyz\0"; //All acceptable chars
      int counter = 0; //The iterator through the unparsed string.
      int placeHolder = 0; //The iterator through the parsed string.
      for (counter; counter < 300000; counter++){
	if (webPageData[counter] == '\0'){ //If the page has ended...
	  parsed[placeHolder] = '\n'; //Insert a newline character,
	  break; //And exit the loop
	}
	//The following conditional prevents double newlines:
	if ( (webPageData[counter] == '\n' || webPageData[counter] == ' ') && parsed[placeHolder - 1] != '\n'){ //If the current char is a space or newline AND the previous character was not a newline...
	  parsed[placeHolder] = '\n'; //Insert a newline in the parsed string.
	  placeHolder++; 
	}
	else if (strchr(acceptable, tolower(webPageData[counter])) != NULL){ //If the current character is found in the list of acceptable strings
	  parsed[placeHolder] = tolower(webPageData[counter]);
	  placeHolder++;
	}
      }
	
      return parsed; //Return the parsed string.
    }

*/


/*
struct trie{
char* addr;
//26 children one for every letter of the alphabet
struct trie *child[26];
//count keeps track of how many times each word occures, also signifies the end of a complete word
int count;
//each node holds a letter that make up the words.
char letter;

int wordCount;
int termcount[10];
double tfScore[10];
double idfScore[10];
double sScore[10];
};

*/














