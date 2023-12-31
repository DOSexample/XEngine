#pragma once

#include <vector>

namespace XSystem {
	namespace IO {
		class BinaryReader
		{
		public:
			BinaryReader();
			BinaryReader(std::vector<char>* curData, size_t curOffset = -1, size_t maxSize = -1);
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

			size_t GetCurrentOffset() { return m_CurOffset; }

			void SetCurrentOffset(size_t offset)
			{
				m_CurOffset = offset;
			}

			void SetBuffer(void* pSrc, int count, size_t offset)
			{
				memcpy( &m_CurData->data()[offset], pSrc, count );
			}

		private:
			size_t m_CurOffset = 0;
			size_t m_MaxSize = 0;
			std::vector<char>* m_CurData = nullptr;
		};
	}
}