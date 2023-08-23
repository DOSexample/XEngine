#pragma once

#include <vector>

namespace XSystem {
	namespace IO {

		class BinaryReader;

		class Zlib
		{
		public:
			Zlib();
			Zlib(BinaryReader& br);
			~Zlib();

			int m_OriginalSize;
			int m_CompressSize;

			std::vector<char> m_OriginalData;
			std::vector<char> m_CompressData;

			static bool Decompress(Zlib& z);

		};
	}
}