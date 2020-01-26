/*

	Author: Garett Pascual-Folster
	Date: 11/17/2019
	Description: A game of 5-card draw poker

*/

#include "poker.h"

int main(void) {
	int choice = 0, playing = 1;
	srand((unsigned)time(NULL));

	do {
		selection(choice);
		choice = menu();

		if (choice == EXIT) {
			playing = 0;
		}
	} while (playing);

	CLEAR;
	printf("Come again!\n");

	return 0;
}