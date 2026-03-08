#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <functional>

#include "misc/get_root_name.hpp"
#include "misc/get_content_from_files.hpp"
#include "file_records/filesystem_node.hpp"
#include "traversals/traversals_string_tree.hpp"
#include "traversals/traversals_content_roots.hpp"

using namespace std;
using namespace file_records;

static vector<string> dontIncludeFolderNests = { ".git", "build" };

void buildTree(FilesystemNode& rootNode) {
	for (filesystem::directory_entry entry : filesystem::directory_iterator(rootNode.getPath())) {
		FilesystemNode entryNode(&entry);

		if (entry.is_directory()) {
			bool isADisincludedFolderNest = false;

			for (const auto& disincludeFolderNest : dontIncludeFolderNests)
				if (entryNode.getName() == disincludeFolderNest) {
					isADisincludedFolderNest = true;
					break;
				}

			if(!isADisincludedFolderNest)
				buildTree(entryNode);
		}

		rootNode.addChild(entryNode);
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		//then we have a root path to work with
		filesystem::path rootPath(argv[1]);
		filesystem::directory_entry rootEntry(rootPath);
		
		//build the tree
		FilesystemNode rootNode(&rootEntry);
		buildTree(rootNode);

		//list the tree
		string tree = rootNode.getName() + '\n';
		traverseStringTree(tree, rootNode);
		cout << "> PROJECT STRUCTURE:" << endl << tree << endl;

		if (argc > 2) {
			//then we have extensions dedicated to content extraction
			vector<string> contentExtensions;
			for (int i = 2; i < argc; i++)
				contentExtensions.push_back(argv[i]);

			cout << "> CONTENT EXTENSIONS:" << endl;
			for(string contentExtension : contentExtensions)
				cout << '-' << contentExtension << endl;
			cout << endl;

			//list all roots with extensions that are dedicated to content extraction
			vector<string> contentRoots;
			traverseContentRootsByExtension(contentRoots, rootNode, contentExtensions);

			cout << "> CONTENT ROOTS:" << endl;
			for(string contentRoot : contentRoots)
				cout << ">> " << contentRoot << endl;
			vector<string> contents = getContentFromFiles(contentRoots);

			/*
			cout << "> CONTENTS:" << endl;
			for (int i = 0; i < contentRoots.size(); i++) {
				cout << ">> " << contentRoots[i] << endl;
				cout << contents[i] << endl << endl;
			}
			*/
		}
	}

	return 0;
}