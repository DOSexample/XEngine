#include "FileLoader.h"

namespace {

	std::wstring to_wstring(const std::string& str)
	{
		std::wstring wstr;
		size_t size;
		wstr.resize(str.length());
		mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
		return wstr;
	}
	std::string to_string(const std::wstring& wstr)
	{
		std::string str;
		size_t size;
		str.resize(wstr.length());
		wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
		return str;
	}

	bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}
	bool replaceAll(std::string& str, const std::string& from, const std::string& to) {
		while (replace(str, from, to))
		{
		}
		return true;
	}

	bool replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}
	bool replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
		while (replace(str, from, to))
		{
		}
		return true;
	}
}

namespace XSystem {
	namespace IO {

		FileLoader::FileLoader()
		{
		}

		FileLoader::~FileLoader()
		{
		}

		bool FileLoader::Load(std::ifstream& file, std::vector<char>& data, int appendForSafeString)
		{
			if (!file.is_open())
				return {};

			// Find out the size of the file
			size_t fileSize = file.tellg();
			data.resize(fileSize);

			// Move the file pointer back to the beginning and read all bytes at once
			file.seekg(0, std::ios::beg);
			file.read(data.data(), fileSize);

			if (appendForSafeString > 0)
			{
				for (int i = 0; i < appendForSafeString; ++i)
					data.push_back(0);
			}

			file.close();

			return data.size() > 0 && fileSize;
		}

		bool FileLoader::Load(const char* fileName, std::vector<char>& data)
		{
			std::string fName = fileName;
			replaceAll(fName, "\\", "/");

			std::ifstream file(fName, std::ios::binary | std::ios::ate);

			return Load(file, data);
		}

		bool FileLoader::Load(const wchar_t* fileName, std::vector<char>& data)
		{
			std::wstring fName = fileName;
			replaceAll(fName, L"\\", L"/");

			std::ifstream file(fName, std::ios::binary | std::ios::ate);

			return Load(file, data);
		}

		bool FileLoader::ReadString(const char* fileName, std::vector<char>& data)
		{
			std::string fName = fileName;
			replaceAll(fName, "\\", "/");

			std::ifstream file(fName, std::ios::binary | std::ios::ate);

			return Load(file, data, 1);
		}

		bool FileLoader::ReadString(const wchar_t* fileName, std::vector<char>& data)
		{
			std::wstring fName = fileName;
			replaceAll(fName, L"\\", L"/");

			std::ifstream file(fName, std::ios::binary | std::ios::ate);

			return Load(file, data, 2);
		}

	}
}