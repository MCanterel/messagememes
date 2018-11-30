#include <Windows.h>
#include "MemeMessage.h"

MemeMessage::MemeMessage()
{
	const std::string phrase = getPhrase();
	buildMessage(phrase);
}

MemeMessage::~MemeMessage() {
	for (MLetter* l : PhraseGrid)
	{
		delete l;
		l = nullptr;
	}
}

void MemeMessage::buildMessage(const std::string phrase)
{
	size_t lll = phrase.size();
	for (size_t i = 0; i < phrase.size(); i++) {
		MLetter* letter = new MLetter((const char&)phrase[i]);
		PhraseGrid.push_back(letter);
		//PhraseGrid.push_back(std::move(letter));
	}
}

const std::string MemeMessage::getPhrase() const
{
	std::vector <std::string> phraseList;
	std::ifstream phraseFile("useful_phrases.txt");

	for (std::string line; std::getline(phraseFile, line);) {
		if (line.empty()) {
			continue;
		}
		else {
			phraseList.push_back(line);
		}
	}
	phraseFile.close();
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, (int)phraseList.size() - 1);
	std::string target = phraseList[dist(rng)];
	//target.resize(maxPhraseSize);
	return target;
}

MemeMessage::MLetter::MLetter(const char& ltr)
{
	if (ltr == ' ') {
		letter = letters[(char)LetterNums::SPACE];
	}
	else {
		letter = letters[ltr - 'a'];
	}

	int lastLetterCol = 0;
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 5; y++) {
			int bitshift = 30 - (x * 5 + y);
			if ((std::bitset<1>(letter >> bitshift)) == 1) {
				lastLetterCol = x;
			}
		}
	}
	MLetter::LetterGrid.resize((lastLetterCol+1) * 5, { 0,0 });

	for (int x = 0; x <= lastLetterCol; x++) {  //hallefrikkinlullah. I was looping through too many x's (5)...
		for (int y = 0; y < 5; y++) {
			int bitshift = 29 - (x * 5 + y);
			auto testBit = (std::bitset<1>(letter >> bitshift));
			if ( testBit== 1) {
				LetterGrid[x * 5 + y].x = x;
				LetterGrid[x * 5 + y].y = y;
			}
		}
	}
}

