#include <iostream>

int menu();

int main(){
	//int occupiedArea[3][3] = {
	//	{0, 0, 0},
	//	{0, 0, 0},
	//	{0, 0, 0}
	//};

	do {
		menu();
	} while (menu() != 3);
}

int menu() {
	
	int userInput;
	
	std::cout << "Welcome to Noughts and Crosses!\n";
	std::cout << "What would you like to do?\n1. Play 2P mode\n2. Play 1P mode (unavailable)\n3. Close program\n";
	std::cin >> userInput;

	switch (userInput) {
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	default:
		std::cout << "Invalid option\n";
		return -1;
	};
}