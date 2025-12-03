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

void gameLoop();
void displayGrid();
void displayInput();
void fillSquare(int row, int col);
static void checkInput();
static void checkStatus();
static void init();

//ftxui based functions incase i need to rewrite them
void displayMenuFTXUI();
void clearScreen();


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

void displayMenuFTXUI() {
    using namespace ftxui;


	//creates the screen and menu component
    auto screen = ScreenInteractive::TerminalOutput();
    std::vector<std::string> menu_entries = {
        "Start Game",
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
    if (selected == 1) { // "Quit" is at index 1
        clearScreen();
		exit(0); 
    }
    else if (selected == 0) { //"Start Game" is at index 0

        gameLoop(); 
    }
}

void gameLoop() {
	do{
		clearScreen(); 
		cout << "Welcome to Noughts and crosses!\n";
		displayGrid();
		displayInput();
		checkInput();
		checkStatus();
	} while (toupper(input) != 'F');
}

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		//assume Unix based
		system("clear");
	#endif
}