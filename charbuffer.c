#include "charbuffer.h"

#include <stdlib.h>
#include <string.h>

/* See charbuffer.h for usage instructions. */


/* A utility.  cb_mod(-1, 8) is 7, not -1! */
unsigned int cb_mod(int a, unsigned int b) {
	int intermediate = a % b;
	if (intermediate < 0) {
		intermediate += b;
	}
	return intermediate;
}

CharBuffer* cb_new(unsigned int size) {
	CharBuffer* newBuffer;
	char* characters;

	/* Allocate space for the structure. */
	newBuffer = malloc(sizeof(CharBuffer));
	if (!newBuffer) {
		return NULL;
	}
	memset(newBuffer, 0, sizeof(CharBuffer));

	/* Allocate space for the buffer itself. */
	characters = malloc(size);
	if (!characters) {
		return NULL;
	}
	memset(characters, 0, size);

	/* And initialise the members. */
	newBuffer->size = size;
	newBuffer->nextInsert = 0;
	newBuffer->buffer = characters;

	/* Return the finished object */
	return newBuffer;
}

void cb_free(CharBuffer* buf) {
	if (!buf) {
		return;
	}
	if (buf->buffer) {
		free(buf->buffer);
	}
	free(buf);
}

void cb_write(CharBuffer* buf, char c) {
	buf->buffer[buf->nextInsert] = c;
	buf->nextInsert = cb_mod(buf->nextInsert + 1, buf->size);
}

void cb_writeString(CharBuffer* buf, char* s) {
	if (!s) {
		return;
	}

	char* ptr = s;
	while (*ptr) {
		cb_write(buf, *ptr);
		ptr++;
	}
}

void cb_dumpBuffer(CharBuffer* buf) {
	int i;
	for (i = 0; i < buf->size; i++) {
		if (buf->buffer[i] < ' ') {
			putchar('.');
		} else {
			putchar(buf->buffer[i]);
		}
	}
	putchar('\n');
	for (i = 0; i < buf->nextInsert; i++) {
		putchar(' ');
	}
	putchar('^');
	putchar('\n');
}

void cb_dumpString(CharBuffer* buf) {
	/* The oldest character is the one pointed to by nextInsert */
	int i = buf->nextInsert;
	do {
		if (buf->buffer[i] < ' ') {
			putchar('.');
		} else {
			putchar(buf->buffer[i]);
		}
		i = cb_mod(i+1, buf->size);
	} while (i != buf->nextInsert);

	putchar('\n');
}

void cb_copyString(CharBuffer* buf, char* dst) {
	int i = buf->nextInsert;
	int j = 0;
	do {
		dst[j] = buf->buffer[i];
		i = cb_mod(i+1, buf->size);
		j++;
	} while (i != buf->nextInsert);
	dst[j] = '\0';
}

void cb_backspace(CharBuffer* buf) {
	buf->nextInsert = cb_mod(buf->nextInsert-1, buf->size);
}

char cb_getChar(CharBuffer* buf, int idx) {
	unsigned int realIndex = cb_mod(buf->nextInsert + idx, buf->size);
	return buf->buffer[realIndex];
}

unsigned int cb_size(CharBuffer* buf) {
	return buf->size;
}
