#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "text_tree/text_tree.hpp"

using namespace std;
using namespace text_tree;

static vector<string> includeForFileContents;
static map<string, string> fileContents;

int main(int argc, char* argv[]) {
	if (argc > 1) {
		if (argc > 2) {
			for (int i = 2; i < argc; i++)
				includeForFileContents.push_back(argv[i]);
		}

		cout << "> PROJECT STRUCTURE:" << endl;
		cout << TextTree::startAt(argv[1]) << endl;

		if (argc > 2) {
			cout << "> FILE CONTENTS" << endl;
			for (const auto& [key, value] : fileContents)
			{
				cout << ">> \"" << key << '\"' << endl;
				cout << value << endl << endl;
			}
		}

		return 0;
	}

	cout << "An input directory could not be found." << endl;
	return -1;
}