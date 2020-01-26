#include "poker.h"

void game(void) {
	Hand hands[N_PLAYERS];
	Card deck[52];
	int card_n = 0, player;

	// init & shuffle deck
	init_deck(deck);
	shuffle_deck(deck);

	// deal out cards from top of deck
	card_n = deal(hands, deck);
	display_game(hands, 1);

	//choose cards to replace
	for (player = 0; player < N_PLAYERS; player++) {
		if (!player) {
			replace_cards(hands[player].hand, deck, &card_n);
		}
		else {
			automatic_replace(hands[player].hand, deck, &card_n);
		}
	}

	// show winner
	CLEAR;
	display_game(hands, 0);
	printf("\nThe winner is player %d!\n", determine_winner(hands));
	PAUSE;
}

void display_game(Hand hands[N_PLAYERS], int conseal) {
	const char* suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	const char* face_value[13] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight",
		"Nine", "Ten", "Jack", "Queen", "King" };

	int i = 0, card = 0;

	for (i; i < N_PLAYERS; i++) {
		printf("\n--- Player %d cards ---\n", i + 1);
		for (card = 0; card < 5; card++) {
			if (i == 0 || !conseal) {
				printf("%5s of %-8s\n", face_value[hands[i].hand[card].face_value],
					suit[hands[i].hand[card].suit]);
			}
			else {
				printf("***** of *******\n");
			}
		}
	}
}

int menu(void) {
	int choice = 0;

	do {
		CLEAR;
		printf("--- Welcome to 5-Card Draw ---\n");
		printf("1. Play Game\n");
		printf("2. Rules\n");
		printf("3. Exit\n");
		scanf("%d", &choice);
	} while (choice < PLAY || choice > EXIT);

	return choice;
}

void display_rules(void) {
	CLEAR;
	printf("--- RULES ---\n\n");
	printf("In this game you are player 1 and the rest of the\nplayers are the computer.");
	printf("The combination of cards apply the same way\nand the game start with the dealer shuffling the deck\n");
	printf("The cards are dealt out to each player and\nyou will be able to choose which cards to discard.\n");
	printf("You will get to replace those cards with new cards and\nthe winner with best combo wins the game.\n");
	PAUSE;
}

void selection(int choice) {
	
	switch (choice) {
		case PLAY:
			CLEAR;
			game();
			break;
		case RULES:
			display_rules();
			break;
	}

}

void replace_cards(Card hand[], Card deck[52], int* start) {
	int n_cards = 0, cards_selected[5] = { 0 }, i;

	do { // ask to replace
		printf("\nHow much cards do you want to replace: ");
		scanf("%d", &n_cards);
	} while (n_cards < 0 || n_cards > 5);

	// pick cards to replace
	printf("List the cards to replace (1 = top; 5 = bottom):\n");
	for (i = 0; i < n_cards; i++) {
		do {
			printf("card %d to replace: ", i + 1);
			scanf("%1d", &cards_selected[i]);
		} while (cards_selected[i] < 1 || cards_selected[i] > 5);
	}

	// replace cards
	for (i = 0; i < n_cards; i++) {
		hand[cards_selected[i] - 1].face_value = deck[*start].face_value;
		hand[cards_selected[i] - 1].suit = deck[*start].suit;
		(*start)++;
	}
}

void automatic_replace(Card hand[], Card deck[52], int* start) {
	int keep[5] = { 0 }, bypass = -1, card = 0;
	Card high = { 0,0 };

	// check for highest value
	for (card = 0; card < 5; card++) {
		if (hand[card].face_value > high.face_value) {
			high.face_value = hand[card].face_value;
			high.suit = hand[card].suit;
		}
		else if (hand[card].face_value == high.face_value) {
			if (hand[card].suit > high.suit) {
				high.suit = hand[card].suit;
			}
		}
	}
	for (card = 0; card < 5; card++) {
		if (hand[card].face_value == high.face_value) {
			if (hand[card].suit == high.suit) {
				keep[card] = 1;
			}
		}
	}

	// check for pair
	if (n_of_kind(hand, &bypass) == 2) {
		for (card = 0; card < 5; card++) {
			if (hand[card].face_value == bypass) {
				keep[card] = 1;
			}
			else {
				keep[card] = 0;
			}
		}

		// check for another pair
		if (n_of_kind(hand, &bypass) == 2) {
			for (card = 0; card < 5; card++) {
				if (hand[card].face_value == bypass) {
					keep[card] = 1;
				}
			}
		}
	}

	// check for 3 of a kind
	if (n_of_kind(hand, &bypass) == 3) {
		for (card = 0; card < 5; card++) {
			if (hand[card].face_value == bypass) {
				keep[card] = 1;
			}
			else {
				keep[card] = 0;
			}
		}
	}

	// check for 4 of a kind
	if (n_of_kind(hand, &bypass) == 4) {
		for (card = 0; card < 5; card++) {
			if (hand[card].face_value == bypass) {
				keep[card] = 1;
			}
			else {
				keep[card] = 0;
			}
		}
	}

	// check for a flush or straight
	if (is_flush(hand, &bypass) == 5 || is_straight(hand, &bypass) == 5) {
		for (card = 0; card < 5; card++) {
			keep[card] = 1;
		}
	}

	// check if 4 in a row
	if (is_straight(hand, &bypass) == 2) {
		for (card = 0; card < 5; card++) {
			if (hand[card].face_value >= bypass &&
				hand[card].face_value <= bypass + 4) {
				keep[card] = 1;
			}
			else {
				keep[card] = 0;
			}
		}
	}

	// check for 4 same suits
	if (is_flush(hand, &bypass) == 2) {
		for (card = 0; card < 5; card++) {
			if (hand[card].suit == bypass) {
				keep[card] = 1;
			}
			else {
				keep[card] = 0;
			}
		}
	}

	
	// replace cards
	for (card = 0; card < 5; card++) {
		if (!keep[card]) {
			hand[card].face_value = deck[*start].face_value;
			hand[card].suit = deck[*start].suit;
			(*start)++;
		}
	}
}

int deal(Hand hands[N_PLAYERS], Card deck[52]) {
	int player = 0, card = 0;

	for (int i = 0; i < 5; i++) {
		for (player = 0; player < N_PLAYERS; player++) {
			hands[player].hand[i].face_value = deck[card].face_value;
			hands[player].hand[i].suit = deck[card++].suit;
		}
	}

	return card;
}

void init_deck(Card deck[52]) {
	int value = 0, suit = 0, card = 0;

	for (suit; suit < 4; suit++) {
		for (value = 0; value < 13; value++) {
			deck[card].face_value = value;
			deck[card++].suit = suit;
		}
	}
}

void shuffle_deck(Card deck[52]) {
	int value, suit, card;
	int nDeck[4][13] = { {0} };

	for (card = 0; card < 52; card++) {

		do {
			value = rand() % 13;
			suit = rand() % 4;
		} while (nDeck[suit][value] != 0);

		nDeck[suit][value] = 1;
		deck[card].face_value = value;
		deck[card].suit = suit;

	}
}

int determine_winner(Hand hands[N_PLAYERS]) {
	int player, bypass = -1, winner = 0, highest_score = 0, max = 0, i;
	int scores[N_PLAYERS] = { 0 };

	for (player = 0; player < N_PLAYERS; player++) {
		// check for pair
		if (n_of_kind(hands[player].hand, &bypass) == 2) {
			scores[player] = 1;

			// check for double pair
			if (n_of_kind(hands[player].hand, &bypass) == 2) {
				scores[player] = 2;
			}
		}
		
		// check for 3 of kind
		if (n_of_kind(hands[player].hand, &bypass) == 3) {
			scores[player] = 3;
		}

		// check for 4 of kind
		if (n_of_kind(hands[player].hand, &bypass) == 4) {
			scores[player] = 4;
		}

		bypass = -1;
		// check for flush
		if (is_flush(hands[player].hand, &bypass) == 1) {
			scores[player] = 5;
		}

		// check for straight
		if (is_flush(hands[player].hand, &bypass) == 1) {
			scores[player] = 6;
		}
	}

	for (player = 0; player < N_PLAYERS; player++) {
		if (scores[player] > highest_score) {
			highest_score = scores[player];
			winner = player;
		}
		else if (scores[player] == highest_score) {
			switch (scores[player]) {
			case 0: // highest card (no combo)
				max = 0, i = 0;
				for (i = 0; i < 5; i++) {
					if (hands[winner].hand[i].face_value > max) {
						max = hands[winner].hand[i].face_value;
					}
				}
				for (i = 0; i < 5; i++) {
					if (hands[player].hand[i].face_value > max) {
						winner = player;
					}
				}
				break;

			case 1:
			case 3:
			case 4:
				n_of_kind(hands[winner].hand, &max);
				n_of_kind(hands[winner].hand, &bypass);

				if (bypass > max) {
					winner = player;
				}
				break;
			}
		}
	}

	return winner + 1;
}

int n_of_kind(Card hand[], int *bypass) {
	int i = 0, max = 0, values[13] = { 0 };

	for (; i < 5; i++) { // tally each value
		if (hand[i].face_value != *bypass) {
			values[hand[i].face_value]++;
		}
	}

	// check for max
	for (i = 0; i < 13; i++) {
		if (values[i] > max) {
			max = values[i];
			*bypass = i;
		}
	}

	if (max == 1) {
		max = 0;
	}

	return max;
}

int is_straight(Card hand[], int *start) {
	int i = 0, tally = 0, values[13] = { 0 }, is_straight = 0;

	for (; i < 5; i++) { // tally each value
		values[hand[i].face_value]++;
	}

	for (i = 0; i < 13; i++) {
		if (values[i] == 1) {
			*start = i;
			tally++;
		}
		else if (tally < 3) {
			tally = 0;
		}
		else { // tally is 4
			i = 13;
		}
	}

	if (tally == 4) {
		is_straight = 2;
		*start -= 4;
	}
	else if (tally == 5) {
		is_straight = 1;
		*start -= 5;
	}

	return is_straight;
}

int is_flush(Card hand[], int *suit) {
	int i = 0, suits[4] = { 0 }, is_flush = 0;

	for (; i < 5; i++) { // tally each suit
		suits[hand[i].suit]++;
	}

	for (i = 0; i < 4; i++) {
		if (suits[i] == 5) {
			is_flush = 1;
		}
		else if (suits[i] == 4) {
			is_flush = 2;
			*suit = i;
		}
	}

	return is_flush;
}