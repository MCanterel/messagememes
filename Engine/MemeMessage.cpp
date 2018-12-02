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
	for (size_t i = 0; i < phrase.size(); i++) {
		MLetter* letter = new MLetter((char&)phrase[i]);
		PhraseGrid.push_back(letter);
		//PhraseGrid.push_back(std::move(letter));
	}
}

const std::string MemeMessage::getPhrase() const
{
	std::vector <std::string> phraseList;
	std::ifstream phraseFile("meme_list_capitalized.txt");

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
	target.resize(std::min<size_t>(target.size(),maxPhraseSize));
	return target;
}

MemeMessage::MLetter::MLetter(char& ltr)  
{
	ltr = tolower(ltr);
	if (ltr == ' ') {
		letter = letters[(char)LetterNums::SPACE];
		SetLetterWidth((int)LetterSpace::Space);
	}
	else {
		letter = letters[ltr - 'a'];
		if (letter == 'l')
		{
			SetLetterWidth((int)LetterSpace::Narrow);
		}
		else if (IsWideLetter(ltr))
		{
			SetLetterWidth((int)LetterSpace::Wide);
		}
		else
		{
			SetLetterWidth((int)LetterSpace::Medium);
		}
	}

	MLetter::LetterGrid.resize(letterWidth * 5, { -1,-1 });

	//IK this is little weird, but it works so not refactoring now
	//fix this by addressing bitset via [] operator?
	for (int x = 0; x < letterWidth; x++) { 
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

