#pragma once
#include <vector>
#include "Vei2.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <random>
//xx

class MemeMessage
{
private:
	class MLetter {
	public:
		MLetter() = default;
		MLetter(const char&);
		std::vector <Vei2> LetterGrid;
	private:
		enum class LetterNums { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, SPACE };
		unsigned int letters[26];
		unsigned int letter;
	};

public:
	MemeMessage();
	~MemeMessage();

private:
	void buildMessage(const std::string phrase);
	const std::string getPhrase() const;

public:
	std::vector <MLetter*> PhraseGrid;
};

