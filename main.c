#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ldata.h"
#include "llist.h"
#include "hashtable.h"

#define MAXLINE 300

struct LList **training(int BUCKETS){
	struct LList **ptr = initialize(BUCKETS);	//MOST IMPORTANT, CREATES THE HASHTABLE DATA STRUCTURE, returns into **ptr

	char D1stream[MAXLINE]; 	//Since the specifications say all words will be on one line, this sets the max characters in a line, which is actually much longer than typical
	char D2stream[MAXLINE];		// ^
	char D3stream[MAXLINE];		// ^^
	FILE *fp;			// file pointer for D1.txt
	FILE *fp2;			// ^ for D2
	FILE *fp3;			// ^ for D3
	
	//------------> OPEN AND PARSE D1.TXT <-------------------------------------
	//--------------------------------------------------------------------------
	fp = fopen("D1.txt", "r");						//open D1.txt
	if (fp == NULL) {
		fprintf(stderr, "\n Error opening txts");	//error checking test
		exit(1);
	}

	while(fscanf(fp, "%s ", D1stream)!=EOF)			//use fscanf to scan D1.txt, using " " as a delimiter, while not END OF FILE
	{
		hash_table_insert(D1stream, 1, ptr, BUCKETS);	//insert each separated word into the proper hashtable: see hash_table_insert function
	}
	
	//------------> OPEN AND PARSE D2.TXT <-------------------------------------
	//--------------------------------------------------------------------------

	fp2 = fopen("D2.txt", "r");						//open D2.txt
	if (fp2 == NULL) {
		fprintf(stderr, "\n Error opening txts");	//error checking test
		exit(1);
	}

	while(fscanf(fp2, "%s ", D2stream)!=EOF)			//use fscanf to scan D2.txt into D2stream, using " " as a delimiter, while not END OF FILE
	{
		hash_table_insert(D2stream, 2, ptr, BUCKETS);	//insert each separated word into the proper hashtable: see hash_table_insert function
	}
	
	//------------> OPEN AND PARSE D3.TXT <-------------------------------------
	//--------------------------------------------------------------------------

	fp3 = fopen("D3.txt", "r");						//open D2.txt
	if (fp3 == NULL) {
		fprintf(stderr, "\n Error opening txts");	//error checking test
		exit(1);
	}

	while(fscanf(fp3, "%s ", D3stream)!=EOF)			//use fscanf to scan D3.txt into D3stream, using " " as a delimiter, while not END OF FILE
	{
		hash_table_insert(D3stream, 3, ptr, BUCKETS);	//insert each separated word into the proper hashtable: see hash_table_insert function
	}
	
	return ptr;										//return pointer to the HashTable
}


///////-->as per specifications, needs to loop after ranking display is done

void loop_retrieval(struct LList **ptr, int BUCKETS) {			//loop the searching and retrieval
	char x = ' ';												//placeholder x
	printf("character input is 'S'\n");							//since only way to get here is if input is S in main
	printf("Enter your search query, separated by spaces\n");	
	struct LList *sq = create_empty_list();						//********EMPTY LIST TO STORE QUERY WORDS********
	fseek(stdin,0,SEEK_END);									//clear the buffer in preparation for query string input
	sq = read_query(ptr, BUCKETS);								//GO TO READ_QUERY FUNCTION, set linked list to its return value
	rank(sq, ptr);												//GO TO RANK FUNCTION parameters are the query words list, and the hashtable
	printf("Enter 'S' for search, 'X' to exit\n");				//prompt
	scanf("%c", &x);											//scan
	getchar();													//clear buffer
	if (x == 'S') {												// if S, recursive funciton into loop
		loop_retrieval(ptr, BUCKETS);
	}
	else if (x == 'X') {										//otherwise, exit
		printf("Thanks for playing!\n");
		return;
	}
}

	/////        MAIN FUNCTION			/////

int main() {
	struct LList **ptr;		//THIS IS A POINTER THE HASHTABLE DATA STRUCTURE - a linked list of linked lists, created in training(), in initialize()
	int BUCKETS = 0;		//number of buckets, determined by user at run-time

	//actual program starts here//
	//Initial prompt!!
	printf("Enter size of hash table\n");
	scanf("%d", &BUCKETS);
	getchar();
	ptr = training(BUCKETS); //TRAINING w/ docs returns a double pointer to the hashtable structure

	char x= ' ';
	printf("Enter 'S' for search, 'X' to exit\n");	//prompt to continue or exit
	scanf("%c", &x);
	getchar();								//clear \n character
	if (x == 'S') {
		loop_retrieval(ptr, BUCKETS);	//if char is 'S', go to loop_retrieval, pass in the hashtable pointer and #buckets
	}
	else if (x == 'X') {	//otherwise, exit
		return 0;
	}
	return 0;
}


