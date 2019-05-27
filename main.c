#include <stdio.h>
#include "fileTagger.c"

int main(int argc, char* argv[]) {

	int result = verify_input(argc, argv);
	printf("%d", result);

	return 0;
}
