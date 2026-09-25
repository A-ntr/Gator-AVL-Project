#include <iostream>
#include <string>

#include "AVLTree.h"
using namespace std;

int main() {
	string command;
	AVLTree* tree = new AVLTree;
	while (getline(cin, command)) {
		tree->execute(command);
		if (command == "exit") break;
	}

	delete[] tree;
	return 0;
}

//src/main.exe
