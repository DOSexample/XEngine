#include "BinaryReader.h"

namespace XSystem {
	namespace IO {

		BinaryReader::BinaryReader()
		{
		}

		BinaryReader::BinaryReader(std::vector<char>* curData, size_t curOffset, size_t maxSize) 
			: 
			m_CurData(curData), 
			m_CurOffset(curOffset), 
			m_MaxSize(maxSize)
		{
		}

		BinaryReader::~BinaryReader()
		{
			m_CurData = nullptr;
			m_CurOffset = -1;
			m_MaxSize = -1;
		}

		bool BinaryReader::CanRead(int count)
		{
			return CanRead( (size_t)count );
		}
		bool BinaryReader::CanRead(size_t count)
		{
			if (m_CurOffset == -1 || m_MaxSize == -1)
				return false;

			if (m_CurOffset + count > m_MaxSize)
				return false;

			return true;
		}

		bool BinaryReader::ReadEmpty(int count)
		{
			if (CanRead(count))
			{
				m_CurOffset += count;
				return true;
			}
			return false;
		}


		char BinaryReader::ReadByte()
		{
			size_t s = sizeof(char);
			if (CanRead(s))
			{
				char tmp = m_CurData->data()[m_CurOffset];
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		char* BinaryReader::ReadBytes(void* pOut, int count)
		{
			if (CanRead(count))
			{
				memcpy(pOut, &m_CurData->data()[m_CurOffset], count);
				m_CurOffset += count;
				return (char*)pOut;
			}
			return nullptr;
		}
		unsigned char BinaryReader::ReadUByte()
		{
			size_t s = sizeof(unsigned char);
			if (CanRead(s))
			{
				unsigned char tmp = m_CurData->data()[m_CurOffset];
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		short BinaryReader::ReadShort()
		{
			size_t s = sizeof(short);
			if (CanRead(s))
			{
				short tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}
		unsigned short BinaryReader::ReadUShort()
		{
			size_t s = sizeof(unsigned short);
			if (CanRead(s))
			{
				unsigned short tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		int BinaryReader::ReadInt()
		{
			size_t s = sizeof(int);
			if (CanRead(s))
			{
				int tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}
		unsigned int BinaryReader::ReadUInt()
		{
			size_t s = sizeof(unsigned int);
			if (CanRead(s))
			{
				unsigned int tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		long BinaryReader::ReadLong()
		{
			size_t s = sizeof(long);
			if (CanRead(s))
			{
				long tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}
		unsigned long BinaryReader::ReadULong()
		{
			size_t s = sizeof(unsigned long);
			if (CanRead(s))
			{
				unsigned long tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		float BinaryReader::ReadFloat()
		{
			size_t s = sizeof(float);
			if (CanRead(s))
			{
				float tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}
		double BinaryReader::ReadDouble()
		{
			size_t s = sizeof(double);
			if (CanRead(s))
			{
				double tmp;
				memcpy(&tmp, &m_CurData->data()[m_CurOffset], s);
				m_CurOffset += s;
				return tmp;
			}
			return 0;
		}

		char* BinaryReader::ReadString(char* pOut, int count)
		{
			if (CanRead(count))
			{
				memcpy(pOut, &m_CurData->data()[m_CurOffset], count);
				pOut[count] = '\0';
				m_CurOffset += count;
				return pOut;
			}

			return nullptr;
		}

	}
}