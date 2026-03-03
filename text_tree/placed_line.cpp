#include "placed_line.hpp"

using namespace text_tree;

const vector<string> PlacedLine::ignoreDirectoryNames{ ".git", "build" };

vector<PlacedLine> PlacedLine::lookInsideDirectory(string root) {
	//whenever getPlacedLines() is called, it starts by tabulating its return content
	PlaceOperation nextOperation = PlaceOperation::TABULATE;
	vector<PlacedLine> lines;

	//examines each name in the directory
	for (const auto& entry : directory_iterator(root)) {
		//extract the file/folder name [destinyName]
		string eps = entry.path().string(), destinyName = eps.substr(root.size() + 1, eps.size() - 1);

		if (entry.is_regular_file()) {
			lines.push_back(PlacedLine(destinyName + ",", nextOperation));

			/*
			//store file content if extension is favoured
			const int dotPosition = destinyName.find_last_of('.');
			if (includeForFileContents.size() > 0 && dotPosition != -1) {
				const string extension = destinyName.substr(dotPosition, destinyName.size() - 1);
				auto includeForContentsCheck = find(includeForFileContents.begin(),
					includeForFileContents.end(), extension);

				if (includeForContentsCheck != includeForFileContents.end()) {
					ifstream file(root + '\\' + destinyName);
					if (!file)
						throw std::runtime_error("Failed to open file");

					ostringstream buffer;
					buffer << file.rdbuf(); //read entire file

					fileContents.insert({ root + '\\' + destinyName, buffer.str() });
				}
			}
			*/
		}
		else {
			auto ignoreNameCheck = find(ignoreDirectoryNames.begin(), ignoreDirectoryNames.end(), destinyName);
			if (ignoreNameCheck != ignoreDirectoryNames.end())
				//ignores all folder contents with a single line
				lines.push_back(PlacedLine(destinyName + ": { ... },", nextOperation));
			else
				//function recalls itself to acquire inner/nested content
				writeDirectory(lines, root + '\\' + destinyName, destinyName, nextOperation);
		}

		if (nextOperation != PlaceOperation::CONTINUE)
			nextOperation = PlaceOperation::CONTINUE;
	}

	return lines;
}

void PlacedLine::writeDirectory(vector<PlacedLine>& container, string absPath, string rootName,
PlaceOperation startingOperation) {
	vector<PlacedLine> nestedLines = lookInsideDirectory(absPath);

	if (nestedLines.size() > 0) {
		container.push_back(PlacedLine(rootName + ": {", startingOperation));
		for (const auto& nestedEntry : nestedLines)
			container.push_back(nestedEntry);
		container.push_back(PlacedLine("},", PlaceOperation::DETABULATE));
	}
	else
		container.push_back(PlacedLine(rootName + ": {},", startingOperation));
}