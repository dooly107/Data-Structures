#include <stdio.h>
#include <stdlib.h>
#include "packing.h"

int main(int argc, char** argv) {
	int success = 0;
	Node* packing = Load_BST_From_File(argv[1]);
	Node* pack = Packing_From_BST(packing);
	success = Save_Packing_To_File(pack, argv[2]);
	return EXIT_SUCCESS;
}
