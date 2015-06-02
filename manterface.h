/* Contains functions for interfacing with 'man'. */

#ifndef MANTERFACE_H
#define MANTERFACE_H

typedef struct ManList {
	char* page;
	struct ManList* next;
} ManList;

void ml_append(ManList** list, char* page);
void ml_free(ManList** list);
void ml_populateFromMan(ManList** list, char* entry);
void ml_showManPage(char* page);
void ml_showAllManPages(ManList** list);

#endif
