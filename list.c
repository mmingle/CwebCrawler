#include "trie.h"
#include "list.h"


/*
#######################################################################
 Returns 1 if the list starting at pNode contains the address addr,
 and returns 0 otherwise
#######################################################################
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
############################################################
 Inserts the address addr as a new listNode at the end of
 the list
#############################################################
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
##########################################################
Prints addresses
##########################################################
*/
void printAddresses(const struct listNode *pNode){
	if(pNode !=NULL){
	printf("%s\n\n",pNode->addr);
	printAddresses(pNode->next);
	}
}



/*
##########################################################
 prints the addresses from pNode to the end of the list,
 one on each line
##########################################################
 */
//TODO: i dont think this is used....

void makeAddressList(const struct listNode *pNode,char* list){
if(pNode !=NULL){
	printf("%s\n",pNode->addr);
	printAddresses(pNode->next);
	}

}


/*
#######################################################################
 Frees the memory associated with this node and all subsequent nodes
#######################################################################
 */
void destroyList(struct listNode *pNode){
 

if(pNode != NULL){
destroyList(pNode->next);
free(pNode);
}

}

/*
#######################################################################
Crawler
#######################################################################
*/

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


/*
###############################################
 make url List
###############################################
*/
int makeUrlList(struct listNode *pListStart,char* initialList){


char* word=malloc(1000);
int numHops;
char hops;
int i;
int k=0;
int j=100;

for(i=0;initialList[i] != '\0';i++){
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



/*
###############################################
int readURLfile(char* filename, char* URLlist)

 takes in two char*,
 first is the name of the file to be read,
 second is where the contents of the file will be written to
###############################################
*/


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
###############################################
gets links..
###############################################
*/
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


/*
###############################################
 Start Indexing..
###############################################
*/

int startIndexing(struct trie *root[],struct listNode *pListStart, char *webPageData,char *words){



printf("Indexing...\n");

int counter=0;
while(pListStart != NULL){ 

//root[counter]=newtrie(0,pListStart->addr);

root[counter]->addr=pListStart->addr;
getText(pListStart->addr, webPageData,300000);


printf("%s\n",pListStart->addr);



parseData(words,webPageData);

//add Word occurrence returns the number of words.
root[counter]->wordCount=addWordOccurrence(words,root[counter]);

pListStart=pListStart->next;
counter++;
}


return counter;



}








