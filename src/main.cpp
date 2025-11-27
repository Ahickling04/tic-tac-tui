#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <conio.h>

using namespace std;

void displayGrid();
void displayInput();
void fillSquare(int row, int col);
void checkInput();
void checkStatus();
void init();

//ftxui based functions incase i need to rewrite them



//globals
const int ROW = 3;
const int COLS = 3;
char grid[ROW][COLS];
int filledSquares = 0;
char input = ' ';
int currentPlayer = 1;

int main() {
	
	//clear grid and set player to 1
	init();

	do {
	// 	system("cls");
	// 	cout << "Welcome to Noughts and crosses!\n";
	// displayGrid();
	// 	displayInput();
	// 	checkInput();
	// 	checkStatus();

	} while (toupper(input) != 'F');
	//checks to see that the player hasnt quit by pressing f



	return 0;
}

void displayGrid() {
	//old code
	
	// cout << grid[0][0] << "|" << grid[0][1] << "|" << grid[0][2] << '\n';
	// cout << "-|-|-\n";
	// cout << grid[1][0] << "|" << grid[1][1] << "|" << grid[1][2] << '\n';
	// cout << "-|-|-\n";
	// cout << grid[2][0] << "|" << grid[2][1] << "|" << grid[2][2] << '\n';


	//displays the grid using ftxui
	auto document = ftxui::hbox({
		ftxui::vbox({
			ftxui::text(string(1, grid[0][0])) | ftxui::border,
			ftxui::text(string(1, grid[1][0])) | ftxui::border,
			ftxui::text(string(1, grid[2][0])) | ftxui::border,
		}),
		ftxui::vbox({
			ftxui::text(string(1, grid[0][1])) | ftxui::border,
			ftxui::text(string(1, grid[1][1])) | ftxui::border,
			ftxui::text(string(1, grid[2][1])) | ftxui::border,
		}),
		ftxui::vbox({
			ftxui::text(string(1, grid[0][2])) | ftxui::border,
			ftxui::text(string(1, grid[1][2])) | ftxui::border,
			ftxui::text(string(1, grid[2][2])) | ftxui::border,
		}),
	});
	auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));
	ftxui::Render(screen, document);
	screen.Print();
}

void displayInput() {
	//displays the input keys and  the current player

	cout << "Player " << currentPlayer << " its your go!\n";
	cout << "Enter the key as shown below to correspond with the grid\n\n";
	cout << "Q|W|E\n";
	cout << "-|-|-\n";
	cout << "A|S|D\n";
	cout << "-|-|-\n";
	cout << "Z|X|C\n";
}

void fillSquare(int row, int col) {
	
	if (grid[row][col] != ' ') {
		cout << "That square is filled, please try again\n";
		checkInput();
	}
	else {
		if (currentPlayer == 1) {
			grid[row][col] = 'X';
		}
		else {
			grid[row][col] = 'O';
		};

		filledSquares++;
	};

}
static void checkInput() {
	cin >> input;

	if (cin.fail()) {
		cout << "Invalid character, try agian!\n";
		checkInput();
	}

	input = toupper(input);

	switch (input) {
		case 'Q': fillSquare(0, 0); break;
		case 'W': fillSquare(0, 1); break;
		case 'E': fillSquare(0, 2); break;
		case 'A': fillSquare(1, 0); break;
		case 'S': fillSquare(1, 1); break;
		case 'D': fillSquare(1, 2); break;
		case 'Z': fillSquare(2, 0); break;
		case 'X': fillSquare(2, 1); break;
		case 'C': fillSquare(2, 2); break;

	default:
		cout << "Invalid character, try again!\n";
		checkInput();
		break;
	}
}

static void checkStatus() {
	
	//char to check, init to X change to O when P2
	char xo = 'X';
	if (currentPlayer == 2) {
		xo = 'O';
	};
	
	//to check if we are still playing
	//set to false if win/draw
	bool inPlay = true;

	if (
		//row matches
		grid[0][0] == xo && grid[0][1] == xo && grid[0][2] == xo ||
		grid[1][0] == xo && grid[1][1] == xo && grid[1][2] == xo ||
		grid[2][0] == xo && grid[2][1] == xo && grid[2][2] == xo ||

		//collumn matches
		grid[0][0] == xo && grid[1][0] == xo && grid[2][0] == xo ||
		grid[0][1] == xo && grid[1][1] == xo && grid[2][1] == xo ||
		grid[0][2] == xo && grid[1][2] == xo && grid[2][2] == xo ||

		//diagonal matches
		grid[0][0] == xo && grid[1][1] == xo && grid[2][2] == xo ||
		grid[0][2] == xo && grid[1][1] == xo && grid[2][0] == xo
		) {
		cout << "Player " << currentPlayer << " wins!\n";
		inPlay = false;
	}
	else if (filledSquares == ROW * COLS) {
		cout << "It's a draw!\n";
		inPlay = false;
	}

	if (!inPlay) {
		displayGrid();
		init();

		cout << "Play again? Any key. F to quit.\n";
		cin >> input;
	}
	else {
		if (currentPlayer == 1)
			currentPlayer = 2;
		else
			currentPlayer = 1;
	}
}

static void init() {
	currentPlayer = 1;
	filledSquares = 0;

	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COLS; col++) {
			grid[row][col] = ' ';
		}
	}
}