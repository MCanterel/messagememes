#pragma once
#include <string>

inline std::wstring delExtraSpaces(std::wstring &str)
{
	int size = str.length();
	for (int j = 0; j <= size; j++)
	{
		for (int i = 0; i <= j; i++)
		{
			if (str[i] == ' ' && str[i + 1] == ' ')
			{
				str.erase(str.begin() + i);
			}
			else if (str[0] == ' ')
			{
				str.erase(str.begin());
			}
			else if (str[i] == '\0' && str[i - 1] == ' ')
			{
				str.erase(str.end() - 1);
			}
		}
	}
	return str;
}
