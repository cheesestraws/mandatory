#include "manterface.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ml_append(ManList** list, char* page) {
	if (list == NULL) {
		return;
	}

	if (*list != NULL) {
		ml_append(&((*list)->next), page);
		return;
	}

	char* mypage;
	ManList* mylist;
	size_t len = strlen(page);
	
	mypage = malloc(len + 1);
	if (!mypage) {
		return;
	}
	memcpy(mypage, page, len+1);
	
	mylist = malloc(sizeof(ManList));
	mylist->page = mypage;
	mylist->next = NULL;
	*list = mylist;
}

void ml_free(ManList** list) {
	if (*list == NULL) {
		return;
	}

	if((*list)->next) {
		ml_free(&(*list)->next);
	}

	free((*list)->page);
	free(*list);
	*list = NULL;
}

void ml_populateFromMan(ManList** list, char* entry) {
	/* There *has* to be a better way of doing this... */

	/* First, build the command we're going to run */
	char* prefix = "man -aw '";
	char* postfix ="' 2>&1";
	
	char* command = NULL;
	unsigned int commandLen = strlen(entry) + strlen(prefix) + strlen(postfix);
	command = malloc(commandLen + 1);
	if (!command) {
		return;
	}

	*command = '\0';
	strncat(command, prefix, commandLen);
	strncat(command, entry, commandLen);
	strncat(command, postfix, commandLen);

	/* Clear the list */
	ml_free(list);

	/* Now, we can run the command. */
	FILE* pipe = popen(command, "r");
	/* If paths to man pages are > 256bytes long, you REALLY need to
	   reconsider your directory structure choices. */
	char filename[256];
	while (fgets(filename, 256, pipe) != NULL) {
		filename[strcspn(filename, "\n")] = '\0';
		if (*filename == '/') {
			ml_append(list, filename);
		}
	}
	pclose(pipe);
}

void ml_showManPage(char* page) {
	/* This is a hack for now until I figure out how to do it properly
	   cross-platform-ly. */
	char* pagebuf;
	char* section;
	char* entry;
	int len = strlen(page);

	/* Clone page into pagebuf */
	pagebuf = malloc(len+1);
	if (!pagebuf) {
		return;
	}
	memcpy(pagebuf, page, len+1);
	
	/* find the last . */
	section = pagebuf + len;
	char* ptr;
	for (ptr = pagebuf + len; ptr >= pagebuf; ptr--) {
		if (*ptr == '.') {
			section = ptr + 1;
			*ptr = '\0';
			break;
		}
	}

	/* and find the / before it, if it exists. */
	entry = section;
	for (ptr = section; ptr >= pagebuf; ptr--) {
		if (*ptr == '/') {
			entry = ptr + 1;
			*ptr = '\0';
			break;
		}
	}
	
	/* Build the command line... */
	char* command;
	char* prefix = "man ";
	
	/* The length of the command block is the length of the prefix, plus 
	   the length of the section, plus one for a space, plus the length
	   of the entry name, plus one for the terminating null. */
	   
	command = malloc(strlen(prefix) + strlen(section) + 1 + strlen(entry) + 1);
	if (!command) {
		free(pagebuf);
		return;
	}

	*command = '\0';
	strcat(command, prefix);
	strcat(command, section);
	strcat(command, " ");
	strcat(command, entry);

	/* And go go go */
	system(command);

	/* finally, clean up */
	free(pagebuf);
	free(command);
}

void ml_showAllManPages(ManList** list) {
	ManList* iter = *list;
	while (iter) {
		ml_showManPage(iter->page);
		iter = iter->next;
	}
}
