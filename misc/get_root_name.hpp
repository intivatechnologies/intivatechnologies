#pragma once
#ifndef GET_ROOT_NAME_HPP
#define GET_ROOT_NAME_HPP

#include <string>
#include <filesystem>

using namespace std;

static string getRootName(filesystem::path path) {
	return path.string().substr(path.string().find_last_of("\\") + 1, path.string().size() - 1);
}

#endif