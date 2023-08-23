#include "Zlib.h"

#include "BinaryReader.h"

#include "miniz.c"

namespace XSystem {
	namespace IO {

		Zlib::Zlib()
		{
		}

		Zlib::Zlib(BinaryReader& br)
		{
			m_OriginalSize = br.ReadInt();
			m_CompressSize = br.ReadInt();

			m_CompressData.resize(m_CompressSize);
			if (br.ReadBytes(m_CompressData.data(), m_CompressSize))
			{
				m_OriginalData.resize(m_OriginalSize);
			}
		}

		Zlib::~Zlib()
		{
			if (!m_OriginalData.empty()) m_OriginalData.clear();
			if (!m_CompressData.empty()) m_CompressData.clear();
		}

		bool Zlib::Decompress(Zlib& z)
		{
			return
				z.m_CompressData.size() > 0 &&
				z.m_OriginalData.size() > 0 &&
				mz_uncompress((unsigned char*)z.m_OriginalData.data(), (mz_ulong*)&z.m_OriginalSize, (const unsigned char*)z.m_CompressData.data(), (mz_ulong)z.m_CompressSize) == MZ_OK;
		}
	}
}
