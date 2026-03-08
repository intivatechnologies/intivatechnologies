#pragma once

#include "../file_records/filesystem_node.hpp"

using namespace file_records;

void traverseContentRootsByExtension(vector<string>& contentRoots, const FilesystemNode& node,
	const vector<string>& contentExtensions) {
	for (FilesystemNode child : node.getChildren()) {
		string childRoot = child.getPath().string();

		if (child.getChildren().size() > 0)
			traverseContentRootsByExtension(contentRoots, child, contentExtensions);
		else for (const auto& contentExtension : contentExtensions)
			if (child.getExtension() == contentExtension) {
				contentRoots.push_back(childRoot);
				break;
			}
	}
}