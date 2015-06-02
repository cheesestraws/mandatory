#include "backend.h"
#include "charbuffer.h"
#include "manterface.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static char wordBuffer[BUFFER_SIZE + 1];
static pid_t pid;
static ManList* manPages;

int is_identifier_char(char c) {
	/* a bit of a hack: return 1 if char is an identifier char */
	if (c >= 'a' && c <= 'z') {
		return 1;
	}
	if (c >= 'A' && c <= 'Z') {
		return 1;
	}
	if (c >= '0' && c <= '9') { 
		return 1;
	}
	if (c == '_') {
		return 1;
	}
	return 0;
}

char* backend_cb_lastIdentifier(CharBuffer* buf) {
	char* ptr = &wordBuffer[BUFFER_SIZE];
	*ptr = '\0';
	ptr--;
	int index = -1;
	
	/* Until we hit a non-identifier character, or run out of buffer, or
	   run out of word buffer */
	char currentChar = cb_getChar(buf, index);
	while (is_identifier_char(currentChar) &&
	       -(index) <= cb_size(buf)) {

		*ptr = currentChar;
		index--;
		ptr--;
		
		/* check if ptr is ok */
		/* The bit after the && is because, if this is the last
		   character in the circular buffer, it's OK for ptr to point
		   one char before the start of the wordBuffer, becasuse that
		   will never be written to. */
		if (ptr < wordBuffer && index < cb_size(buf)) {
			fprintf(stderr, "INTERNAL ERROR: Tried to fall off start of wordBuffer in backend.c\n");
		}
		
		currentChar = cb_getChar(buf, index);
	}
	return ptr + 1;
}

void backend_init(BackendState* state) {
	state->buffer = cb_new(BUFFER_SIZE);
	manPages = NULL;
	pid = -1;
}

void backend_terminate(BackendState* state) {
	cb_free(state->buffer);
}

void backend_handleChar(BackendState* state, char c) {
	char* last_identifier;

	cb_write(state->buffer, c);

	last_identifier = backend_cb_lastIdentifier(state->buffer);
	ml_populateFromMan(&manPages, last_identifier);

	if (manPages) {
		/* we have one or more man pages.  To display them: */

		/* 1. Kill the previous process if it exists */
		if (pid != -1) {
			kill(pid, SIGTERM);
			waitpid(pid);
		}

		pid = fork();
		if(!pid) {
			/* We are in the child process */
			while(1) {
				ml_showAllManPages(&manPages);
			}
		}

	}
}

void backend_handleBackspace(BackendState* state) {
	if (is_identifier_char(cb_getChar(state->buffer, -1))) {
		cb_backspace(state->buffer);
	}
}

