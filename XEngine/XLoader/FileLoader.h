#pragma once

#include <vector>
#include <fstream>

namespace XSystem {
	namespace IO {

		class FileLoader
		{
		public:
			FileLoader();
			~FileLoader();

			bool Load(std::ifstream& file, std::vector<char>& data, int appedForSafe = 0);
			bool Load(const char* fileName, std::vector<char>& data);
			bool Load(const wchar_t* fileName, std::vector<char>& data);

			bool ReadString(const char* fileName, std::vector<char>& data);
			bool ReadString(const wchar_t* fileName, std::vector<char>& data);
		};

	}
}