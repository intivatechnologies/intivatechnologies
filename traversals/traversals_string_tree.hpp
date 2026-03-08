#pragma once

#include "../file_records/filesystem_node.hpp"

using namespace file_records;

static const string TAB_SPACE = "  ";
static string loadedTabSequence = "";

void traverseStringTree(string& listTree, const FilesystemNode& node) {
	loadedTabSequence += TAB_SPACE;
	for (const auto& child : node.getChildren()) {
		listTree += loadedTabSequence + child.getName() + '\n';
		traverseStringTree(listTree, child);
	}
	loadedTabSequence = loadedTabSequence
		.substr(0, loadedTabSequence.size() - TAB_SPACE.size());
}