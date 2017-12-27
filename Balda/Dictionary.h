//словарь русского языка

#pragma once

#include <string>
#include <set>

class CDictionary {

public:
	CDictionary() {};
	~CDictionary() {};

	void Insert(const std::string &newWord);
	bool IsFound(const std::string &newWord) const;
	void Clear();

private:
	std::set <std::string> words;
};

