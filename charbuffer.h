/* charbuffer.h
   ============

   A CharBuffer is a circular buffer of characters.  It has a size and a next
   insert pointer, and some *very* rudimentary backspace handling.

   CharBuffer* cb = cb_new(32);
   ... then for each character ...
   cb_write(cb, yourCharacter);

   ... then either get stuff out using cb_copyString or cb_getChar.
*/

#ifndef CHARBUFFER_H
#define CHARBUFFER_H

typedef struct CharBuffer {
	unsigned int size;
	unsigned int nextInsert;
	char* buffer;
} CharBuffer;


CharBuffer* cb_new(unsigned int size);
void cb_free(CharBuffer* buf);
void cb_write(CharBuffer* buf, char c);
void cb_writeString(CharBuffer* buf, char* s);
void cb_dumpBuffer(CharBuffer* buf);
void cb_dumpString(CharBuffer* buf);
void cb_copyString(CharBuffer* buf, char* dst);
void cb_backspace(CharBuffer* buf);
char cb_getChar(CharBuffer* buf, int idx);

#endif
