#include "stdafx.h"
#include "Dictionary.h"
#include <fstream>

void CDictionary::Insert(const std::string &word)
{
	if (!words.count(word)) {
		words.insert(word);
	}
}

bool CDictionary::IsFound(const std::string &word) const
{
	return words.count(word);
}

void CDictionary::Clear()
{
	words.clear();
}