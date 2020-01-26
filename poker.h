#define _CRT_SECURE_NO_WARNINGS
#define CLEAR system("cls")
#define PAUSE system("pause")
#define N_PLAYERS 2
#define PLAY 1
#define RULES 2
#define EXIT 3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct card {
	int face_value;
	int suit;
} Card;

typedef struct hand {
	Card hand[5];
} Hand;

/*
Function: game;
Description: main function of the game and where
	the game starts;
	@param void;
	@return void;
*/
void game(void);

/*
Function: display_game;
Description: Function that displays the game
	in a nice orderly fashion;
	@param hands		An array of the players hands;
	@param conseal		int flag to conseal cards or not;
	@return void;
*/
void display_game(Hand hands[N_PLAYERS], int conseal);

/*
Function: menu;
Description: displays the main menu;
	@param void;
	@return choice input by user;
*/
int menu(void);

/*
Function: display_rules;
Description: Display rules lol;
	@param void;
	@return void;
*/
void display_rules(void);

/*
Function: selection;
Description: determines what to do with the
	choice input by the user;
	@param choice			choice input by user;
	@return void;
*/
void selection(int choice);

/*
Function: replace_cards;
Description: asks the user what cards to replace
	and replaces them from the top cards of the deck
	@param hand				hand to replace cards;
	@param deck				deck of the card;
	@param *start			starting point of deck to replace with;
	@return void;
*/
void replace_cards(Card hand[], Card deck[52], int* start);

/*
Function: automatic_replace;
Description: used by computer and determines which cards
	to keep and replaces them with the top cards of the deck;
	@param hand				hand to replace cads from;
	@param deck				deck of cards;
	@param *start			starting point of deck to replace cards with;
	@return void;
*/
void automatic_replace(Card hand[], Card deck[52], int* start);

/*
Function: deal;
Description: Deals the cards from the top of the deck to each player
	giving each player a card one by one in a casino deal fashion;
	@param hands			the players hands to deal to;
	@param deck				the deck of cards;
	@param	the card index to start from again;
*/
int deal(Hand hands[N_PLAYERS], Card deck[52]);

/*
Function: init_deck;
Description: function to initialize the deck of cards in order;
	@deck					list for deck of cards;
	@return void;
*/
void init_deck(Card deck[52]);

/*
Function: shuffle_deck;
Description: shuffles the deck in random order;
	@param deck				deck to shuffle;
	@return void;
*/
void shuffle_deck(Card deck[52]);

/*
Function: determine_winner;
Description: function to determine winner based on best combo and cards;
	@param hands			the list players hands;
	@return					player # that won;
*/
int determine_winner(Hand hands[N_PLAYERS]);

/*
Function: n_of_kind;
Description: determines if there is a n of a kind and if so
	returns the number of kind lol;
	@param hand				hand to check pairs or more;
	@param *bypass			integer to not check for (used when checking for 2 pairs);
	@return					number of same values, 0 if none;
*/
int n_of_kind(Card hand[], int *bypass);

/*
Function: is_straight;
Description: determines if hand has a straight;
	@param hand				hand to check for straight;
	@param *start			output for lowest number of the straight;
	@return					1 if true and 0 if not;
*/
int is_straight(Card hand[], int *start);

/*
Function: is_flush;
Description: determines if hand has a flush;
	@param hand				hand to check for flush;
	@param *suit			the suit of the flush;
	@return					1 if true and 0 if not;
*/
int is_flush(Card hand[], int *suit);