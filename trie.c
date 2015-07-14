#include "trie.h"
/*
###############################################
function that creates a new trie node.
sets the initial count, 
letter of the trie,
and puts it on the heap.
###############################################
*/
struct trie *newtrie(char newchar,char* newAddr) {
    struct trie *newTrie =malloc(sizeof(struct trie));
    memset(newTrie, 0, sizeof(struct trie));
    newTrie->letter=newchar;
    newTrie->count=0.0;
    newTrie->wordCount=0.0;
int i;
for(i=0;i<10;i++){
 newTrie->tfScore[i]=0.0;
 newTrie->idfScore[i]=0.0;
 newTrie->termcount[i]=999.0;
}
    newTrie->sScore=0.0;
    newTrie->addr=newAddr;
    return newTrie;
}


/*
###############################################
Frees the entire Trie 
by recursively iterating through each of the
root node's child nodes and 
deleting the low nodes first, one by one
###############################################
*/
int freeTrieMemory(struct trie* root){
  int nodeLetter;
  for(nodeLetter = 0; nodeLetter < 26; nodeLetter++){
    if(root->child[nodeLetter]!=NULL){
      freeTrieMemory(root->child[nodeLetter]);
      free(root->child[nodeLetter]);
    }
  }

}


/*
###############################################
prints the contents of the trie
###############################################
*/
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


/*
################################################################################################
This method addWordOccurrence is new from project4 might have slight difference from the old one.
################################################################################################
*/
int addWordOccurrence(const char* word,struct trie *root)
{
	
	int printtab=0;
	int counter=0;
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
	counter++;
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
    return counter;
}


/*
########################################################################
 getText()
 You should not need to modify this function 
########################################################################
*/
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



/*
################################################################################
query stuff
################################################################################
*/

//inverse document frequency
/*
IDF(t): the inverse document frequency is the natural logarithm of
[(1.0 + number of indexed documents) / (1.0 + number of indexed documents that contain t at least
once)]. The C standard library provides the "log" function to compute the natural logarithm.
*/


void idf(struct trie *root[],int *numberOfPagesIndexed){

int i;
int j;
int k=0;
double wordDocOc[20];

for(i=0;i<20;i++){
wordDocOc[i]=0.0;
}
i=1;
while(root[i]->termcount[k] != 999){

for(j=1;j<*numberOfPagesIndexed;j++){

if(root[i]->tfScore[k]>0){
wordDocOc[k]+=1.0;
}
}
k++;
}

i=1;
k=0;

for(i=1;i<*numberOfPagesIndexed;i++){

while(root[i]->termcount[k] != 999){
root[i]->idfScore[k]=log(1.0+*numberOfPagesIndexed)/(1.0 + wordDocOc[k]);
k++;
}
k=0;
}

}


/*
################################################################################
query takes in a char** of words 
then places the query terms in it.
################################################################################
*/

int query(char **words, int *argCount, char **usefulQT){

char query[120];
char ch;
int i=0;
int j=0;



printf("Enter a web query: ");

//prompt user for query
while ((ch = getchar()) != '\n'){
	
	if(isupper(ch)){
	printf("Please enter a query containing only lower-case letters.\n");
	return -1;
	}
	if(ch == ' '){
	*argCount+=1;
	usefulQT[i][j]='\0';
	words[i][j]='\n';	
	i++;
	j=0;
	
	continue;
	}
	usefulQT[i][j]=ch;
        words[i][j] = ch;
	words[i][j+1]='\0';
	usefulQT[i][j+1]='\0';
	j++;
}


}

/*
################################################################################
 Term Frequency
TF(d,t): the term frequency of a term t and document d is the number of times that t occurs in d divided
by the number of times any word occurs in d. In other words, it is the fraction of words in d that are
equal to t. Efficient computation of the TF score is the reason to store the sum of all word counts for
each document when you index it.

################################################################################
*/

int tf(struct trie *root, char **queryTerms){
int i=0;
int j=0;
int k=0;

int on=0;

struct trie *initRoot=root;

while(queryTerms[i][j] != '\0'){

if(queryTerms[i][j] == '\n'){
root->termcount[k]=initRoot->count;
k++;
j=0;
i++;
initRoot=root;
}


if(initRoot->child[queryTerms[i][j]-97]!=NULL){
initRoot=initRoot->child[queryTerms[i][j]-97];
}

j++;
}

if(queryTerms[i][j] == '\0'){
root->termcount[k]=initRoot->count;
}


k=0;


while(root->termcount[k] != 999){
root->tfScore[k]=(root->termcount[k]/root->wordCount);
k++;
}

}

/*
################################################################################
//calculates final score
################################################################################
*/

int sScore(struct trie *root[],int *argCount, int *numberOfPagesIndexed){
int k=0;
int i=0;
for(i=1;i<*numberOfPagesIndexed;i++){
while(root[i]->termcount[k] != 999){
root[i]->sScore+=(root[i]->tfScore[k]*root[i]->idfScore[k]);
k++;
}
k=0;
}



}






