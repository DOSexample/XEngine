#pragma once

#include <vector>

namespace XSystem {
	namespace IO {
		class BinaryReader
		{
		public:
			BinaryReader();
			BinaryReader(std::vector<char>* curData, size_t curOffset, size_t maxSize);
			~BinaryReader();

			bool CanRead(int count = 1);
			bool CanRead(size_t count = 1);

			bool ReadEmpty(int count = 1);

			char ReadByte();
			char* ReadBytes(void* pOut, int count);
			unsigned char ReadUByte();

			short ReadShort();
			unsigned short ReadUShort();

			int ReadInt();
			unsigned int ReadUInt();

			long ReadLong();
			unsigned long ReadULong();

			float ReadFloat();
			double ReadDouble();

			char* ReadString(char* pOut, int count);

		private:
			size_t m_CurOffset = 0;
			size_t m_MaxSize = 0;
			std::vector<char>* m_CurData = nullptr;
		};
	}
}