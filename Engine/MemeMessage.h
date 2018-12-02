#pragma once
#include <vector>
#include "Vei2.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <random>
#include <algorithm>

class MemeMessage
{
public:
	MemeMessage();
	~MemeMessage();
	MemeMessage(const MemeMessage&) = delete;
	class MLetter
	{
	public:
		MLetter() = default;
		MLetter(char& ltr);
		std::vector <Vei2> LetterGrid;
		const int GetLetter()
		{
			return letter;
		}
		const int GetLetterWidth()
		{
			return letterWidth;
		}
		void SetLetterWidth(int size)
		{
			letterWidth = size;
		}
		bool IsWideLetter(const char& c)
		{
			return (std::count(WideLetters.begin(), WideLetters.end(), c) > 0);
		}
	private:
		enum class LetterNums { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, SPACE };
		enum class LetterSpace {
			Narrow = 4,
			Medium = 5,
			Wide = 6,
			Space = 1
		};
		int letters[27] =
		{
			0b011111010010100011110000000000,
			0b111111010110101010100000000000,
			0b011101000110001010100000000000,
			0b111111000110001011100000000000,
			0b111111010110101101010000000000,
			0b111111010010100100000000000000,
			0b011101000110101101011011000000,
			0b111110010000100001001111100000,
			0b100011111110001000000000000000,
			0b100101000111111100000000000000,
			0b111110010001010100010000000000,
			0b111110000100001000010000000000,
			0b111110100000100010001111100000,
			0b111110100000100000101111100000,
			0b011101000110001100010111000000,
			0b111111010010100111000000000000,
			0b011101000110001011110000100000,
			0b111111010010110010010000000000,
			0b111011010110101101110000000000,
			0b100001000011111100001000000000,
			0b111100000100001000011111000000,
			0b111000001000001000101110000000,
			0b111110001000100000101111100000,
			0b100010101000100010101000100000,
			0b100000100000111010001000000000,
			0b100011001110101110011000100000,
			0b000000000000000000000000000000
		};
		int letter;  //this is the final chosen letter
		int letterWidth;
		const std::vector <char> WideLetters { 'g', 'h' ,'m', 'n', 'o', 'q', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	};
	const int maxPhraseSize = 20;
private:
	void buildMessage(const std::string phrase);
	const std::string getPhrase() const;
public:
	std::vector <MLetter*> PhraseGrid;
};