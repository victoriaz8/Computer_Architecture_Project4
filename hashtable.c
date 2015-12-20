#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "llist.h"
#include "ldata.h"

struct LList **initialize(int numbuckets) {		//INITIALIZE HASHTABLE, RETURNS A POINTER TO IT
	struct LList **ptr;
	int i = 0;
	ptr = (struct LList**) malloc(sizeof(struct LList) * numbuckets);	//allocates memory for linked list * user entered #buckets
	for (i = 0; i < numbuckets; i++) {
		ptr[i] = create_empty_list();  		//for each linked list entry, points to and creates another linked list
	}
	return ptr;								//returns ptr to hashtable
}

void hash_table_insert(char* w, int i, struct LList **ptr, int b) {
	int idx =  hash_code(w, b);				//find idx of where to insert the word: calls hashcode on the word and total number of (b)uckets
	struct LData *data = create_data(w, i, 1);	//creates new LData from the create_data() function, returns a data struct for a linked list
//		print_data(data);
	if (is_empty(ptr[idx])) {					//if linked list is empty, add first. else, add last
		add_first(ptr[idx], data);				//add at idx of hashcode
		return;
	}
	else {
		struct LNode *temp = ptr[idx]->root;	//temp node to iterate through existing linked list at idx, search for word
		while(temp->next != NULL) {				//while temp->next exists
			if ((*w == *(temp->data->word))&&(i == temp->data->id)) {	//if word = word in node, and document id = id in node, (same doc, same word)
				addCount(temp->data);		//DO NOT ADD NEW NODE, instead, increment existing count
				return;						// and return
			}
			temp = temp->next;				//otherwise, keep looking
		}
		add_last(ptr[idx], data);			//if not in list already, add new node to LL
	}

}

int hash_code(char* w, int b) {				//HASH CODE FUNCTION - takes word and buckets parameters
	int i = 0;		//to iterate through word[idx] using pointer arithmetic
	int sum = 0;	//sum of all characters in word
	int code = 0;	//final hashcode
    while (w[i] != '\0') {	//if character at w[i] is not '\0' 
        sum = sum + w[i];	//add character ascii value to sum
        i++;				
    }
    code = sum % b;			//modulo sum by bucket
    return code;			//return hashcode
}

struct LList *read_query(struct LList **ptr, int BUCKETS) {	//READ_QUERY and separate query into words
	char *search_query = NULL;			//placeholder for dynamic search query
	char *token;						//word pointer for token
	char *temp = NULL;					//temp search_query size
	int p = 0;							//hashcode of search term
	int df = 0;							//document frequency
	char tempword[20];					//tempword of MAX CHARS 20
	size_t size = 0;					//size_t variable - size of string
	size_t idx = 0;						//size_t variable - index
	int end = EOF;						//representation of EOF
	struct LList *searchterms = create_empty_list();	//create empty list to store search terms
	
	while(end) {						//while 'end' int is not 0 or '\n'
		end = getc(stdin);				//get character from standard input
		if(end == EOF || end == '\n') end = 0;	//if character is EOF or '\n', exit loop
		if(size <= idx) {				//if size of string is <= index, realloc more memory to dynamically incrase the size
			size += sizeof(char*);		//total size increases
			temp = realloc(search_query, size);	//realloc
			if(!temp) {					//error checking
				free(search_query);		// ^
				search_query = NULL;	// ^
				break;
			}
			search_query = temp;		// ^
		}
		search_query[idx++] = end;		//store next character into search_query using pointer arithmetic
	}

	struct LData *x;					//temp LData variable
	token = strtok(search_query, " ");	//use strtok to separate words in search_query
	while(token) {						//while token exists
		p = hash_code(token, BUCKETS);	//find hashcode of token
		df = calcDF(token, p, ptr);		//calculate the document frequency by searching through the hashtable (ptr)
		x = create_data(token, p, df);	//create the LData and fill w/ word, hashcode, and df - THIS IS A DIFF FORMAT THAN WORDS IN THE DOCUMENT
										// ^ instead of id, the p is hashcode. instead of num occurences, in searchterms LL it is doc freq. Used in rank()
		add_first(searchterms, x);		//add to searchterms linked list
		token = strtok(NULL," ");		//get next search term
	}
//	print_list(ptr[p]);
	return searchterms;					//return searchterms linked list
}

int calcDF(char* w, int code, struct LList **ptr) {		//CALCULATE DOCUMENT FREQUENCY
	int df = 0;
	int i = 0;
	int size = ptr[code]->size;
	struct LNode *temp = ptr[code]->root;				//go through the hashtable @ index of hashcode
		while(i < size) {								//to find how many documents that word is
			if (strcmp(w, temp->data->word)==0) {		//present in, and increment the df count
				df++;
			}
			if (temp->next != NULL) {
				temp = temp->next;
			}
			i++;
		}
	return df;
}

void rank(struct LList *s, struct LList **ptr) {		//function to rank the documents in order of relevance
	double tf_idf[3] = {0.0, 0.0,0.0};					//tf_idf[0] = D1, [1]  = D2, [2] = D3
	int store[3] = {-1, -2, -3};						//initialize with negatives so if unmodified, will be negative
	double logtmp = 0.0;
	int i = 0;						//iterator
	int df = 0;						//document frequency
	int tf = 0;						//term frequency
	struct LNode *temp2;			//used to move through hashmap[index] linked list (ptr[?])
	struct LNode *temp = s->root;	//used to move through the search term linked list
	for(i = 0; i < s->size; i++) {	
		if (temp->next == NULL) { break; }
		temp2 = ptr[temp->data->id]->root;	//set temp2 to the root of the hashcode of the word in temp, used as idx
		int j;
		for(j = 0; j < ptr[temp->data->id]->size; j++) {			//iterate through the nodes in the bucket indicated by the hashcode
			if (strcmp(temp->data->word, temp2->data->word)==0) { 	//if strings are equal
				df =  temp->data->num;								//doc freq = doc frequency in searchterm list
				tf = temp2->data->num;								//term freq = term frequency already in hashtable LData struct
				store[temp2->data->id-1] = temp2->data->id;			//replace the associated document in store[] with the doc id of the word that matches, no longer negative
																	//--so that -3 becomes 3, -2 becomes 2, and -1 becomes 1, matching the document ID
				logtmp = log10(3.0/df);								//calculates log of 3 / doc freq
				tf_idf[(temp2->data->id)-1] = tf_idf[(temp2->data->id)-1] + tf*logtmp;	//calculates tf_idf for that word, stores into matching idx spot(minus one, b/c starts at 0)
			}  
			temp2 = temp2->next;		//iterates down the bucket Llist
		}	
		temp = temp->next;				//iterates through the search terms ll
	}
//	print_list(ptr[0]);
	///	BELOW IS ESSENTIALLY A BUBBLE SORT TO SORT VALUES IN ORDER
	int c = 0;
	int y = 0;
	int st;		//temp variable
	double t;
	while(c < 3) {	//num docs, also size of array
		y = 0;
		while(y <2) {	// var -------------FOR WHICHEVER VALUE IN tf_idf is greater, swaps the places, and swaps the numbers in store[ ]. 
			if(tf_idf[y] <= tf_idf[y+1]) {	//--such that when store is printed in order, the doc elements will be printed in greatest-least order
				st = store[y];
				store[y] = store[y+1];		//bubble sort continues
				store[y+1] = st;			//switch values in store - essentially switches places of D1, D2, D3
				t = tf_idf[y];				
				tf_idf[y] = tf_idf[y+1];	//also must switch values in tf_idf so there's no infinite loop, since we're sorting based on tf_idf
				tf_idf[y+1] = t;
			}
			y++;
		}
		c++;
	}
//	printf("%f", tf_idf[0]);
//	printf("%f", tf_idf[1]);
//	printf("%f", tf_idf[2]);
	printf("Document results in order of greatest relevance:\n");	//print out document results
	for(c = 0; c < 3; c++) {
		if (store[c] >= 0)	printf("D%d\n", store[c]);			//if the value in store is positive, print out D(number). Otherwise, that means that document
	}															//		did not contain any of the search terms.	
}											////   aaaand done!! :D thanks for reading! :)