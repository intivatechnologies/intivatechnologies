#pragma once


#include <map>
#include <string>
#include <vector>

using namespace std;

namespace client {
	struct Flags {
		map<string, vector<string>> flags;

		enum ModeFlag {
			MF_NONE = 0,
			MF_STRUCTURE = 1 << 0,
			MF_CONTENT = 1 << 1
		} MODE_FLAG = ModeFlag::MF_NONE;
	};

	/*
	void printAllFlagsAndTags(const Flags& conf) {
		for (const auto& pair : conf.flags) {
			cout << "--" << pair.first;
			for (const auto& tag : conf.flags.at(pair.first))
				cout << ' ' << tag;
			cout << endl;
		}
		cout << endl;
	}
	*/

	//populate map 'flags' with all flags and tags
	Flags parseFlags(const int FLAGS_INDEX, int argc, char* argv[]) {
		Flags conf;

		auto compareToEscapeCharacter = [](char beingCompared) {
			char escapeCharacters[] = { '\\', '\"', '\'', '\n', '\t', '\r' };
			for (char escapeChar : escapeCharacters)
				if (beingCompared == escapeChar)
					return true;
			return false;
		};

		string currentFlag = "";
		for (int i = FLAGS_INDEX; i < argc; i++) {
			string token = argv[i];
			cout << "token: " << token << endl;
			while (!token.empty() && compareToEscapeCharacter(token.front()))
				token = token.substr(1);

			if (!token.empty()) {
				/*
				if (compareToEscapeCharacter(token.back()))
					token.pop_back();
					*/

				//process for --flags
				if (token.rfind("--", 0) == 0)
				{
					currentFlag = token.substr(2);
					conf.flags[currentFlag]; // ensure key exists
				}

				//process for tags
				else if (currentFlag.size() > 0)
					conf.flags[currentFlag].push_back(token);
			}
		}

		//print all flags and tags
		//printAllFlagsAndTags(conf);

		return conf;
	}

	//assign tags and refine where needed
	void installFlags(Flags& conf, const map<string, vector<string>>::iterator FLAG_INCLUDE_EXTENSIONS_ITER) {
		//ensure file extensions have a dot char at the beginning
		if (FLAG_INCLUDE_EXTENSIONS_ITER != conf.flags.end())
			for (auto& tag : FLAG_INCLUDE_EXTENSIONS_ITER->second) {
				if (tag[0] != '.')
					tag.insert(tag.begin(), '.');
			}

		//extract one or modes into K_MODE
		auto modeIter = conf.flags.find(K_MODE);
		if (modeIter != conf.flags.end())
			for (auto& m : modeIter->second) {
				if (m == "structure")
					conf.MODE_FLAG = (Flags::ModeFlag)(conf.MODE_FLAG | conf.MF_STRUCTURE);

				if (m == "content")
					conf.MODE_FLAG = (Flags::ModeFlag)(conf.MODE_FLAG | conf.MF_CONTENT);
			}

		//print all flags and tags
		//printAllFlagsAndTags(conf);
	}
}