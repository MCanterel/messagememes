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
		MLetter* letter = new MLetter((const char&)phrase[i]);
		PhraseGrid.push_back(letter);
		//PhraseGrid.push_back(std::move(letter));
	}
}

const std::string MemeMessage::getPhrase() const
{
	std::vector <std::string> phraseList;
	std::ifstream phraseFile("meme_list.txt");

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

MemeMessage::MLetter::MLetter(const char& ltr)  
//fix this by addressing bitset via [] operator
{
	if (ltr == ' ') {
		letter = letters[(char)LetterNums::SPACE];
		SetLetterWidth((int)LetterSpace::Narrow);
	}
	else {
		letter = letters[ltr - 'a'];
		if (IsWideLetter(ltr))
		{
			SetLetterWidth((int)LetterSpace::Wide);
		}
		else
		{
			SetLetterWidth((int)LetterSpace::Medium);
		}
	}
	//int lastLetterCol = 0;  //i can probs refactor this to just use the letterWidth value later
	//for (int x = 0; x < 6; x++) {
	//	for (int y = 0; y < 5; y++) {
	//		int bitshift = 30 - (x * 5 + y);
	//		if ((std::bitset<1>(letter >> bitshift)) == 1) {
	//			lastLetterCol = x;
	//		}
	//	}
	//}
	int lastLetterCol = letterWidth - 1;  //haha that wuz easy
	MLetter::LetterGrid.resize(letterWidth * 5, { -1,-1 });
	
	for (int x = 0; x <= lastLetterCol; x++) {  //IK this is little weird, but it works so not refactoring now
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

