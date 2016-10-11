#include <stdio.h>
#include <stdlib.h>
#include "Board.cc"

int main(int argc, char ** argv)
{
	//system("ls -al");
	printf("%s\n",argv[0]);
	printf("%s\n",argv[1]);
	printf("\n");
	Board a(argv[1]);
	a.print();
	printf("liberties(1,2): %d\n",a.liberties(1,2));
	return 0;
}
