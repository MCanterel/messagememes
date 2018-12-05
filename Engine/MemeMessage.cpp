#pragma once
#include <Windows.h>
#include "MemeMessage.h"
#include <algorithm>
#include <string>

MemeMessage::MemeMessage()
{
	phrase = getPhrase();
	buildMessage(phrase);
}

MemeMessage::~MemeMessage() {
	for (MLetter* l : PhraseGrid)
	{
		delete l;
		l = nullptr;
	}
}

void MemeMessage::buildMessage(const std::string& phrase)
{
	for (size_t i = 0; i < phrase.size(); i++) {
		//had trouble making this a unique_ptr
		MLetter* letter = new MLetter((char&)phrase[i]);
		PhraseGrid.push_back(letter);
	}
}

std::string MemeMessage::getPhrase() const
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

	// if the target is longer than the width, then insert a line break special tab char in here
	// need some work here
	if (target.size() > maxLettersPerLine)
	{
		auto found = target.rfind(" ", maxLettersPerLine);
		if (found != std::string::npos)
		{
			target.replace(found, 1, "\t");
		}
	}
	return target;
}

MemeMessage::MLetter::MLetter(char& ltr)  
{
	ltr = tolower(ltr);
	if (ltr == ' ') {
		letter = letters[(char)LetterNums::SPACE];
		SetLetterWidth((int)LetterSpace::Space);
	}
	else if (ltr == '\t')
	{
		isTab = true;
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

