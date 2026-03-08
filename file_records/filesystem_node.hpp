#pragma once
#ifndef FILESYSTEM_NODE_HPP
#define FILESYSTEM_NODE_HPP

#include <string>
#include <vector>
#include <filesystem>

using namespace std;

namespace file_records {
	class FilesystemNode {
		string name, extension;
		filesystem::path path;
		vector<FilesystemNode> children;

	public:
		FilesystemNode(filesystem::directory_entry* directoryEntry) {
			name = directoryEntry->path().filename().string();
			path = directoryEntry->path();
			if (directoryEntry->is_regular_file())
				extension = directoryEntry->path().extension().string();
		}

		void addChild(FilesystemNode child) {
			children.push_back(child);
		}

		const string& getName() const {
			return name;
		}

		const string& getExtension() const {
			return extension;
		}

		const filesystem::path getPath() const {
			return path;
		}

		const vector<FilesystemNode>& getChildren() const {
			return children;
		}
	};
}

#endif