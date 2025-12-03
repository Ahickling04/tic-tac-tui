#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <iostream>
#include <stdio.h>
//#include <tchar.h>
#include <string>

//if the code is compiled on windows include conio.h for clearScreen
#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

void singleplayerGameLoop(int selection);
void multiplayerGameLoop();
void computerMove(int selection);
void displayGrid();
void displayInput();
void fillSquare(int row, int col);
void singleFillSquare(int row, int col);
static void checkInput();
static void singleCheckInput();
static void checkStatus();
static void init();

//ftxui based functions incase i need to rewrite them
void displayMenuFTXUI();
void clearScreen();
void difficultySelection();


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
		clearScreen();
		displayMenuFTXUI();
	} while (true || toupper(input) != 'F');
	//runs the menu until user decides to quit

	return 0;
}

void displayGrid() {
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

void displayMenuFTXUI() {
    using namespace ftxui;


	//creates the screen and menu component
    auto screen = ScreenInteractive::TerminalOutput();
    std::vector<std::string> menu_entries = {
        "1P Mode",
		"2P Mode",
        "Quit"
    };
    int selected = 0; //sets the initial selected menu item

    //Define the component for the menu logic.
    MenuOption option;
    //When the user presses Enter, exit the loop.
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&menu_entries, &selected, option) | border | center;

    //Defines the main document layout (Title + Menu)
    auto title = text("Welcome to Noughts and Crosses!") | bold | hcenter;
    auto component_document = ftxui::Renderer(menu, [menu, title] {
        auto document = vbox({
            title,
            separator(),
            menu->Render() //This is called on every screen update
        }) | border | center;
        
        return document; 
    });
    
    //Set the initial focus and run the interactive loop.
    screen.Loop(component_document);
    
    //After the user exits the loop (by pressing Enter or clicking on a menu item),
    //check the 'selected' index.
    if (selected == 2) { // "Quit" is at index 2
        clearScreen();
		exit(0); 
    }
    else if (selected == 1) { //"2P mode" is at index 1

        multiplayerGameLoop(); 
    }
	else if(selected == 0){ //"Options" is at index 0
		difficultySelection();
	}
}

void multiplayerGameLoop() {
	do{
		clearScreen(); 
		cout << "Welcome to Noughts and crosses!\n";
		displayGrid();
		displayInput();
		checkInput();
		checkStatus();
	} while (toupper(input) != 'F');
}

void singleplayerGameLoop(int selection) {
		do{
		clearScreen(); 
		cout << "Welcome to Noughts and crosses!\n";
		
		displayGrid();
		computerMove(selection);
		displayInput();
		singleCheckInput();
		checkStatus();

	} while (toupper(input) != 'F');
	
	
	// switch(selection){
	// 	case 0: 
	// 		cout << "Easy mode selected\n"; 
	// 		break;
	// 	case 1:
	// 		cout << "Medium mode selected\n";
	// 		break;
	// 	case 2:
	// 		cout << "Hard mode selected\n";
	// 		break;
	// }
}

void computerMove(int selection) {
	//selection determines the difficulty
	//0 = easy, 1 = medium, 2 = hard
	switch (selection) {
	case 0: { //easy mode, random move
		int row, col;
		srand((unsigned int)time(NULL));
		do {
			row = rand() % 3;
			col = rand() % 3;
		} while (grid[row][col] != ' ');

		grid[row][col] = 'O';
		filledSquares++;
		break;
	}
	case 1: { //medium mode, block player win or random
		//check if player is about to win and block
		bool blocked = false;
		//check rows
		for (int row = 0; row < ROW; row++) {
			if (grid[row][0] == 'X' && grid[row][1] == 'X' && grid[row][2] == ' ') {
				grid[row][2] = 'O';
				blocked = true;
				filledSquares++;
				break;
			}
			else if (grid[row][0] == 'X' && grid[row][2] == 'X' && grid[row][1] == ' ') {
				grid[row][1] = 'O';
				blocked = true;
				filledSquares++;
				break;
			}
			else if (grid[row][1] == 'X' && grid[row][2] == 'X' && grid[row][0] == ' ') {
				grid[row][0] = 'O';
				blocked = true;
				filledSquares++;
				break;
			}
		}
		//if not blocked, random move
		if (!blocked) {
			int row, col;
			srand((unsigned int)time(NULL));
			do {
				row = rand() % 3;
				col = rand() % 3;
			} while (grid[row][col] != ' ');

			grid[row][col] = 'O';
			filledSquares++;
		}
		break;
	}
	case 2: { //hard mode, to be implemented
		//for now just random move
		int row, col;
		srand((unsigned int)time(NULL));
		do {
			row = rand() % 3;
			col = rand() % 3;
		} while (grid[row][col] != ' ');

		grid[row][col] = 'O';
		filledSquares++;
		break;
	}
	}
}

void difficultySelection() {
		using namespace ftxui;


	//creates the screen and menu component
    auto screen = ScreenInteractive::TerminalOutput();
    std::vector<std::string> menu_entries = {
		"Easy",
		"Medium",
		"Hard",
		"Back to Main Menu"
    };
    int selected = 0; //sets the initial selected menu item

    //Define the component for the menu logic.
    MenuOption option;
    //When the user presses Enter, exit the loop.
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&menu_entries, &selected, option) | border | center;

    //Defines the main document layout (Title + Menu)
    auto title = text("Choose your Difficulty") | bold | hcenter;
    auto component_document = ftxui::Renderer(menu, [menu, title] {
        auto document = vbox({
            title,
            separator(),
            menu->Render() //This is called on every screen update
        }) | border | center;
        
        return document; 
    });
    
    //Set the initial focus and run the interactive loop.
    screen.Loop(component_document);
    
    //After the user exits the loop (by pressing Enter or clicking on a menu item),
    //check the 'selected' index.
    if (selected == 3) { // "Back to main menu" is at index 1
        clearScreen();
		displayMenuFTXUI(); 
    }
    else if (selected == 0) { //"easy" is at index 0

        singleplayerGameLoop(0);
    }
	else if (selected == 1){ //"medium" is at index 2
	
		singleplayerGameLoop(1);
	}
	else if (selected == 2){
		singleplayerGameLoop(2);
	}
}

static void singleCheckInput() {
	cin >> input;

	if (cin.fail()) {
		cout << "Invalid character, try agian!\n";
		checkInput();
	}

	input = toupper(input);

	switch (input) {
		case 'Q': singleFillSquare(0, 0); break;
		case 'W': singleFillSquare(0, 1); break;
		case 'E': singleFillSquare(0, 2); break;
		case 'A': singleFillSquare(1, 0); break;
		case 'S': singleFillSquare(1, 1); break;
		case 'D': singleFillSquare(1, 2); break;
		case 'Z': singleFillSquare(2, 0); break;
		case 'X': singleFillSquare(2, 1); break;
		case 'C': singleFillSquare(2, 2); break;

	default:
		cout << "Invalid character, try again!\n";
		checkInput();
		break;
	}
}

void singleFillSquare(int row, int col) {
	
	if (grid[row][col] != ' ') {
		cout << "That square is filled, please try again\n";
		checkInput();
	}
	else {
		grid[row][col] = 'X';
		filledSquares++;
	};

}


void clearScreen() {
	//crossplatform code to clear the terminal
	
	#ifdef _WIN32
		system("cls");
	#else
		//assume Unix based
		system("clear");
	#endif
}