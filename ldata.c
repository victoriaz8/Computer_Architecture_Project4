#include <stdio.h>
#include "llist.h"
#include "ldata.h"
#include <stdlib.h>
#include <string.h>

struct LData *create_empty_data() {
	struct LData *lol;
	lol = (struct LData*) malloc(sizeof(struct LData));
    lol->id = 0;
    lol->num = 0;
    return lol;
}

struct LData *create_data(char *w, int id, int num) {
	struct LData *data;
	data = (struct LData*) malloc(sizeof(struct LData));
	strcpy(data->word,w);
	data->id = id;
	data->num = num;
	return data;
}

void addCount(struct LData *ptr) {
    ptr->num++;
}

// Returns true if and only if the city of data1 comes
// alphanumerically before or is the same as the city of data2
/*int are_ordered(struct LData *data1, struct LData *data2) {
	if (*(data1->city) < *(data2->city)) return 1;
	return 0;
}

// Returns true if and only if the city of data1 equals the city of data2
int are_equal(struct LData *data1, struct LData *data2) {
	if (*(data1->city) == (*(data2->city))) return 1;
	return 0;
}*/

// Swaps the contents of data1 and data2
void swap_data(struct LData *data1, struct LData *data2) {
	struct LData temp = *data1;
	*data1 = *data2;
	*data2 = temp;
}

// Resets the information in data to default values (see create_empty_data)
void clear_data(struct LData *data) {
	data->word[0] = '\0';
    data->id = 0;
    data->num= 0;
}

// Prints the data in the format: (city, lat, lon)
// where lat and lon are printed to one decimal place
void print_data(struct LData *data) {
	printf("(%s, %d, %d)", (data->word), (data->id), (data->num));
}

