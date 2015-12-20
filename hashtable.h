//#include "project4.c"
#ifndef hashtable
#define hashtable

#include "llist.h"
#include "ldata.h"

void printtest();

struct LList **initialize(int numbuckets);

void hash_table_insert(char* w, int i, struct LList **ptr, int b);

int hash_code(char* w, int b);

struct LList *read_query(struct LList **ptr, int BUCKETS);

int calcDF(char* w, int code, struct LList **ptr);

void rank(struct LList *s, struct LList **ptr);

struct LList **initialize(int numbuckets);

#endif