#ifndef BACKEND_H
#define BACKEND_H

#include "charbuffer.h"
#define BUFFER_SIZE 32

typedef struct BackendState {
	CharBuffer* buffer;
} BackendState;

void backend_init(BackendState* state);
void backend_terminate(BackendState* state);
void backend_handleChar(BackendState* state, char c);
void backend_handleBackspace(BackendState* state);

#endif
