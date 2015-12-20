/*
 * data_rec.h
 */

#ifndef LDATA_H_
#define LDATA_H_

struct LData {
	char word[20];
	int id;
	int num;
};

// Creates and returns an empty data record, where
// city = "", lat = 0.0, lon = 0.0
struct LData *create_empty_data();

// Creates and returns a data record with the specified values
struct LData *create_data(char *w, int id, int num);

void addCount(struct LData *ptr);

void set_id(struct LData *data, int hash);
// Returns true if and only if the city of data1 comes
// alphanumerically before or is the same as the city of data2
//int are_ordered(struct LData *data1, struct LData *data2);

// Returns true if and only if the city of data1 equals the city of data2
//int are_equal(struct LData *data1, struct LData *data2);

// Swaps the contents of data1 and data2
void swap_data(struct LData *data1, struct LData *data2);

// Resets the information in data to default values (see create_empty_data)
void clear_data(struct LData *data);

// Prints the data in the format: (city, lat, lon)
// where lat and lon are printed to one decimal place
void print_data(struct LData *data);

#endif /* LDATA_H_ */
