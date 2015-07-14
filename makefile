CC = gcc
CFLAGS = -g -lm
OBJS = list.c trie.c webSearch.c
webSearch:
	$(CC) $(CFLAGS) $(OBJS) -o webSearch 
clean:
	rm *.o *~ webSearch
	





