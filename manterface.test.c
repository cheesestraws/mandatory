#include <stdlib.h>
#include <stdio.h>

#include "manterface.h"

int main(int argc, char** argv) {
	ManList* list = NULL;

        printf("MANTERFACE\n");
        printf("==========\n");
	
	char* one = "one";
	char* two = "two";

	ml_append(&list, one);
	
	if (list != NULL) {
		printf("[pass] Something's been allocated.\n");
	} else {
		printf("[fail] Nothing has been allocated.  Incoming segfault in 3... 2... 1...\n");
	}

	if (list->page != one) {
		printf("[pass] The list should not just point to the old page name\n");
	} else {
		printf("[fail] The page has been arbitrarily imported into the node.\n");
	}

	if (strcmp(list->page, one) == 0) {
		printf("[pass] The page name has been cloned into the list.\n");
	} else {
		printf("[fail] The page name has not been cloned into the list.\n");
	}

	ManList* oldlist = list;
	ml_append(&list, two);

	if(list == oldlist) {
		printf("[pass] Adding a second item didn't stomp all over the first.\n");
	} else {
		printf("[fail] Adding a second item broke the first.\n");
	}

	if (list->next) {
		printf("[pass] There is now a next node.\n");
	} else {
		printf("[fail] There is no next node.  Segfault incoming...\n");
	}

	if (strcmp(two, list->next->page) == 0) {
		printf("[pass] Next node has the right label.\n");
	} else {
		printf("[fail] Next node does not have the right label.\n");
	}

	ml_free(&list);

	if (list == NULL) {
		printf("[pass] ml_free at least nulls the variable.\n");
	} else {
		printf("[fail] ml_free is broken.\n");
	}
	
	ml_populateFromMan(&list, "printf");
	printf("[????] Please check that the two outputs below match.\n");
	ManList* iterator = list;
	while (iterator) {
		printf("%s\n", iterator->page);
		iterator = iterator->next;
	}

	printf("       ... and ...\n");
	system("man -aw printf");
	printf("       ... and press return.");
	getchar();
}	
