#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "trie.h"

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


//allocate memory for 'urlList
char *urlList=malloc(1000);
//error checking
	if(urlList == NULL){
	fprintf(stderr,"could not allocate memory for variable 'urlList'");
	}

//reads from file 'argv[1]', and writes contents to 'urlList'
readURLfile(argv[1], urlList);

//allocate memory for 'pListStart'
struct listNode *pListStart= malloc(sizeof(struct listNode));
//error checking
	if(pListStart == NULL){
	fprintf(stderr,"could not allocate memory for variable 'pListStart'");
	}


/*
method that takes in character string of webpages and number of hopps, 
and a linked list to put each webpage in.
*/


//TODO: make sure this is working correctly
makeUrlList(pListStart,urlList);




//urlList is no longer needed so we free it.
free(urlList);


//allocate memory for words
char* words=malloc(300000);
//error checking
	if(words == NULL){
	fprintf(stderr,"could not allocate memory for variable 'words'");
	}


//allocate memory for 'webPageData'
char* webPageData=malloc(300000);
//error checking
	if(webPageData == NULL){
	fprintf(stderr,"could not allocate memory for variable 'webPageData'");
	}


//allocate memory for 'wordCount'
int *wordCount=(int *)malloc(sizeof(int));
//error checking
	if(wordCount == NULL){
	fprintf(stderr,"could not allocate memory for variable 'wordCount'");
	}


//allocate memory for 'argCount'
int *argCount=(int *)malloc(sizeof(int));
//error checking
if(argCount == NULL){
	fprintf(stderr,"could not allocate memory for variable 'argCount'");
	}


//allocate space for an array of 50 trie structures
struct trie **root=malloc(sizeof(struct trie*)*50);
//error checking
	if(root == NULL){
	fprintf(stderr,"could not allocate memory for variable 'root'");
	}

int i;
int k;
//initialize the tries
for(i=0;i<50;i++){
root[i]=newtrie(0,0);


}


//allocate memory for number of pages.
int *numberOfPagesIndexed=(int *)malloc(sizeof(int));
//error checking
if(numberOfPagesIndexed==NULL){
fprintf(stderr,"could not allocate memory for variable 'numberOfPagesIndexed'");
}

//number of pages that were indexed
*numberOfPagesIndexed=startIndexing(root,pListStart,webPageData, words);


//holds the users query
char **queryTerms=malloc(sizeof(char*)*20);
char **usefulQT=malloc(sizeof(char*)*20);
//error checking
if(queryTerms==NULL){
fprintf(stderr,"could not allocate memory for variable 'queryTerms'");
}

for(i=0;i<20;i++){
queryTerms[i]=malloc(20);
}
for(i=0;i<20;i++){
usefulQT[i]=malloc(20);
}
*argCount=1;
//prompts the user for there query and stores it in queryTerms
query(queryTerms,argCount,usefulQT);

//prints query is...
printf("Query is \"");
for(i=0;i<*argCount;i++){
printf("%s ",usefulQT[i]);
}
printf("\"\n");


//calculates term frequency for each trie
for(i=1;i<*numberOfPagesIndexed;i++){
tf(root[i],queryTerms);
}

//calculates Inverse Document frequency for all tries
idf(root,numberOfPagesIndexed);
//calculates final score
sScore(root,argCount,numberOfPagesIndexed);


//prints out scores
printf("IDF scores are\n");

for(i=0;i<*argCount;i++){
printf("IDF(%s) : %f\n",usefulQT[i],root[1]->idfScore[i]);
}
printf("Web pages :\n");

for(i=1;i<*numberOfPagesIndexed;i++){
printf("%d. %s (score: %f)\n",i,root[i]->addr,root[i]->sScore);

}



//Free it all
free(words);
free(webPageData);
free(wordCount);
free(argCount);
free(numberOfPagesIndexed);
destroyList(pListStart);
for(i=0;i<50;i++){
freeTrieMemory(root[i]);
}

free(root);
for(i=0;i<20;i++){
free(queryTerms[i]);
free(usefulQT[i]);
}
free(usefulQT);
free(queryTerms);

return 0;

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














