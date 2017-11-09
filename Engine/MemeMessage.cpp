#include "MemeMessage.h"

MemeMessage::MemeMessage()
{
	//std::string phrase = getPhrase();
	buildMessage(getPhrase());
}

MemeMessage::~MemeMessage()
{
}

void MemeMessage::buildMessage(const std::string phrase)
{
	for (char c : phrase) {
		MLetter* l = new MLetter(c);
		PhraseGrid.push_back(l);
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

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, (int)phraseList.size());
	const std::string target = phraseList[dist(rng)];
	return target;
}

MemeMessage::MLetter::MLetter(const char& ltr)
{
	letters[(char)LetterNums::A] = 0b011111010010100011110000000000;
	letters[(char)LetterNums::B] = 0b111111010110101011100000000000;
	letters[(char)LetterNums::C] = 0b011101000110001010100000000000;
	letters[(char)LetterNums::D] = 0b111111000110001011100000000000;
	letters[(char)LetterNums::E] = 0b111111010110101101010000000000;
	letters[(char)LetterNums::F] = 0b111111010010100100000000000000;
	letters[(char)LetterNums::G] = 0b011101000110101101011011000000;
	letters[(char)LetterNums::H] = 0b111110010000100001001111100000;
	letters[(char)LetterNums::I] = 0b100011111110001000000000000000;
	letters[(char)LetterNums::J] = 0b100101000111111100000000000000;
	letters[(char)LetterNums::K] = 0b111110010001010100010000000000;
	letters[(char)LetterNums::L] = 0b111110000100001000010000000000;
	letters[(char)LetterNums::M] = 0b111110100000100010001111100000;
	letters[(char)LetterNums::N] = 0b111110100000100000101111100000;
	letters[(char)LetterNums::O] = 0b011101000110001100010111000000;
	letters[(char)LetterNums::P] = 0b111111010010100111000000000000;
	letters[(char)LetterNums::Q] = 0b011101000110001011110000100000;
	letters[(char)LetterNums::R] = 0b111111010010110010010000000000;
	letters[(char)LetterNums::S] = 0b111011010110101101110000000000;
	letters[(char)LetterNums::T] = 0b100001000011111100001000000000;
	letters[(char)LetterNums::U] = 0b111100000100001000011111000000;
	letters[(char)LetterNums::V] = 0b111000001000001000101110000000;
	letters[(char)LetterNums::W] = 0b111110001000100000101111100000;
	letters[(char)LetterNums::X] = 0b100010101000100010101000100000;
	letters[(char)LetterNums::Y] = 0b100000100000111010001000000000;
	letters[(char)LetterNums::Z] = 0b100011001110101110011000100000;
	letters[(char)LetterNums::SPACE] = 0b000000000000000000000000000000;

	if (ltr == ' ') {
		letter = letters[(char)LetterNums::SPACE];
	}
	else {
		letter = letters[ltr - 'A'];
	}

	int lastLetterCol = 0;
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 5; y++) {
			if ((std::bitset<1>((letter >> 30 - (x * 5 + y)))) == 1) {
				lastLetterCol = x;
			}
		}
	}
	MLetter::LetterGrid.resize(lastLetterCol * 5, { 0,0 });

	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 5; y++) {
			if ((std::bitset<1>((letter >> 30 - (x * 5 + y)))) == 1) {
				LetterGrid[x * 5 + y].x = x;
				LetterGrid[x * 5 + y].y = y;
			}
		}
	}
}
