#include "charbuffer.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	CharBuffer* buf = cb_new(32);
	char output[33];
	unsigned char i;
	
	printf("CHARBUFFER\n");
	printf("==========\n");

	/* Circularity of buffer and cbCopyString */
	cb_writeString(buf, "helloworldhelloworldhelloworldhelloworld");
	cb_copyString(buf, output);
	if (strcmp(output, "ldhelloworldhelloworldhelloworld") == 0) {
		printf("[pass] Buffer is circular\n");
	} else {
		printf("[fail] Buffer is not circular or cb_copyString is broken.\n");
		printf("       %s\n", output);
	}

	/* Backspace handling */
	cb_backspace(buf);
	cb_backspace(buf);
	cb_copyString(buf, output);
	if (strcmp(output, "ldldhelloworldhelloworldhellowor") == 0) {
		printf("[pass] Backspace handling is as working as it gets\n");
	} else {
		printf("[fail] Backspace handling is not working.\n");
		printf("       %s\n", output);
	}

	/* getChar */
	output[0] = cb_getChar(buf, 0);
	output[1] = cb_getChar(buf, 32);
	output[2] = cb_getChar(buf, 1);
	output[3] = cb_getChar(buf, -1);
	output[4] = cb_getChar(buf, 2);
	output[5] = cb_getChar(buf, -2);
	output[6] = '\0';
	if (strcmp(output, "lldrlo") == 0) {
		printf("[pass] cb_getChar is behaving plausibly.\n");
	} else {
		printf("[fail] cb_getChar is acting weird: output %s\n", output);
	}

	cb_free(buf);
}
