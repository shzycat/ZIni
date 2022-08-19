/*  ZIni: A Real Crude Cross-Platform Ini-File-Reader Written by C++
*
*   version  :  v0.1.1
*   Author   :  SHZY
*   Email    :  188813950@qq.com
*   now-time :  2022-2-23
*
***************************************
*
*   Happy Using!
*
*/

#ifndef __INI_H__
#define __INI_H__

#include <map>
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>

class ZIni
{
public:
	explicit ZIni(const char *filePath);
	virtual ~ZIni() { update(); };
public:
	int getInt(const char *mainKey, const char *subKey, int defaultValue = 0);

	long long getLl(const char *mainKey, const char *subKey, long long defaultValue = 0);

	unsigned long getUl(const char *mainKey, const char *subKey, unsigned long defaultValue = 0);

	unsigned long long getUll(const char *mainKey, const char *subKey, unsigned long long defaultValue = 0);

	float getFloat(const char *mainKey, const char *subKey, float defaultValue = 0.0f);

	double getDouble(const char *mainKey, const char *subKey, double defaultValue = 0.0);

	std::string get(const char *mainKey, const char *subKey, const char *defaultCStr = "");

	std::map<std::string, std::string>& operator[](std::string mainKey);

	bool operator!();

	explicit operator bool();

	bool is_open();

	bool set(const char *mainKey, const char *subKey, const char *subValue);

	bool remove(const char *mainKey, const char *subKey = NULL);

	void enableFastMode();

	bool update();

private:
	std::string filepath;
	std::string filestring;
	std::map<std::string, std::map<std::string, std::string>> mainMap;
	std::map<std::string, std::string> emptyMap;
	bool m_bOpen = false;
	bool fastMode = false;
	bool needWrite = false;

private:
	std::string::size_type removeSpace(std::string &str);

	bool isSubKeyIndexValid(unsigned int index);

	//users do not need to call this.
	bool writeIntoFile();
};

inline ZIni::ZIni(const char *filePath)
{
	FILE *fp = fopen(filePath, "rb");
	if (!fp)
	{
		return;
	}
	m_bOpen = true;
	filepath = filePath;

	fseek(fp, 0, SEEK_END);
	int sizeOfBytes = ftell(fp);
	rewind(fp);
	filestring.reserve(sizeOfBytes * 2);   //prepare for the write member function.(avoid memory alloc)
	filestring.resize(sizeOfBytes);
	fread((void *)filestring.c_str(), 1, sizeOfBytes, fp);  //C++11 only
	fclose(fp);

	std::string mainKey(""), subKey(""), subValue("");
	std::map<std::string, std::string> subMap;
	std::string::size_type lastLineBreakIndex;  //record the last linebreak position£¬for traversal only once. when reaching a '=', get the keyname directly by using this index.
	std::string::size_type firstValidIndex = filestring.find_first_of('[');
	auto i = firstValidIndex;
	while (i < filestring.length())
	{
		switch (filestring[i])
		{
		case '[':
			//take last section's content before this section into the map.
			if (removeSpace(mainKey) != 0 && subMap.size() != 0)
			{
				mainMap.insert({ mainKey, subMap });
			}
			mainKey.clear();
			subMap.clear();
			for (auto j = i + 1; j < filestring.length(); ++j)
			{
				if (filestring[j] == ']')
				{
					std::string buf(filestring, i + 1, j - i - 1);
					mainKey = std::move(buf);
					i = j;  //make the index(i) jump to the ']' position.
					break;
				}
			}
			break;
		case ';':
			for (auto j = i + 1; j < filestring.length(); ++j)
			{
				if (filestring[j] == '\n')
				{
					i = j;
					lastLineBreakIndex = i;
					break;
				}
			}
			break;
		case '\n':
			lastLineBreakIndex = i;
			break;
		case '=':
			std::string buf(filestring, lastLineBreakIndex + 1, i - lastLineBreakIndex - 1);
			subKey = std::move(buf);
			bool isLastLine = true; //tell if reaching the last line of this file.(note: even if this value is false, doesn't mean this line is definitely not the last line)
			for (auto j = i + 1; j < filestring.length(); ++j)
			{
				if (filestring[j] == '\n')
				{
					isLastLine = false;
					std::string buffer(filestring, i + 1, j - i - 1);
					subValue = std::move(buffer);
					i = j;
					lastLineBreakIndex = i;
					break;
				}
			}
			if (isLastLine)
			{
				std::string buffer(filestring, i + 1, std::string::npos);
				subValue = std::move(buffer);
			}
			if ((removeSpace(subKey) != 0) && (removeSpace(subValue) != 0))
			{
				subMap.insert({ subKey, subValue });
			}
			subKey.clear();
			subValue.clear();
			break;
		}
		i++;
	}
	if (removeSpace(mainKey) != 0 && subMap.size() != 0)
	{
		mainMap.insert({ mainKey, subMap });
	}
}

inline int ZIni::getInt(const char *mainKey, const char *subKey, int defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stoi((*subIter).second);
}

inline long long ZIni::getLl(const char *mainKey, const char *subKey, long long defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stoll((*subIter).second);
}

inline unsigned long ZIni::getUl(const char *mainKey, const char *subKey, unsigned long defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stoul((*subIter).second);
}

inline unsigned long long ZIni::getUll(const char *mainKey, const char *subKey, unsigned long long defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stoull((*subIter).second);
}

inline float ZIni::getFloat(const char *mainKey, const char *subKey, float defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stof((*subIter).second);
}

inline double ZIni::getDouble(const char *mainKey, const char *subKey, double defaultValue)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultValue;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultValue;
	}
	return std::stod((*subIter).second);
}

inline std::string ZIni::get(const char *mainKey, const char *subKey, const char *defaultCStr)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	if ((mainIter = mainMap.find(mainKey)) == mainMap.end())
	{
		return defaultCStr;
	}
	if ((subIter = (*mainIter).second.find(subKey)) == (*mainIter).second.end())
	{
		return defaultCStr;
	}
	return (*subIter).second;
}

inline std::map<std::string, std::string> & ZIni::operator[](std::string mainKey)
{
	auto iter = mainMap.find(mainKey);
	if (iter == mainMap.end())
	{
		return emptyMap;
	}
	return (*iter).second;
}

inline bool ZIni::operator!()
{
	return !m_bOpen;
}

inline ZIni::operator bool()
{
	return m_bOpen;
}

inline bool ZIni::is_open()
{
	return m_bOpen;
}

inline bool ZIni::set(const char *mainKey, const char *subKey, const char *subValue)
{
	if (!m_bOpen) return false;
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	bool mainKeyExist = ((mainIter = mainMap.find(mainKey)) != mainMap.end());
	bool subKeyExist = mainKeyExist ? ((subIter = ((*mainIter).second.find(subKey))) != ((*mainIter).second.end())) : false;

	if (!mainKeyExist)
	{
		std::map<std::string, std::string> subMap;
		subMap.insert({ subKey, subValue });
		mainMap.insert({ mainKey, subMap });
		std::string buffer("\n\n[");
		buffer.reserve(128);
		buffer += mainKey; buffer += "]\n"; buffer += subKey; buffer += " = "; buffer += subValue;
		filestring += buffer;
	}
	else
	{
		if (subKeyExist)
		{
			(*subIter).second = subValue;
			std::string buffer("[");
			buffer.reserve(128);
			buffer += mainKey; buffer += ']';
			auto lineBeginIndex = filestring.find(subKey, filestring.find(buffer));
			//check if the index is valid, because there may be a value which is the same as the key.
			while (lineBeginIndex != std::string::npos)
			{
				if (isSubKeyIndexValid(lineBeginIndex + strlen(subKey)))
					break;
				lineBeginIndex = filestring.find(subKey, lineBeginIndex);
			}
			auto lineEndIndex = filestring.find('\n', lineBeginIndex);
			if (lineEndIndex != std::string::npos)    //did not reach the end-of-file
			{
				buffer = subKey; buffer += " = "; buffer += subValue;
				filestring.replace(lineBeginIndex, lineEndIndex - lineBeginIndex, buffer);
			}
			else
			{
				buffer = subKey; buffer += " = "; buffer += subValue;
				filestring.replace(lineBeginIndex, filestring.length() - lineBeginIndex, buffer);
			}
		}
		else
		{
			(*mainIter).second.insert({ subKey, subValue });
			std::string buffer("[");
			buffer.reserve(128);
			buffer += mainKey; buffer += ']';
			auto mainKeyIndex = filestring.find(buffer);
			auto writeIndex = filestring.find('\n', mainKeyIndex);
			buffer = subKey; buffer += " = "; buffer += subValue; buffer += '\n';
			filestring.insert(writeIndex + 1, buffer);
		}
	}
	if (fastMode)
	{
		needWrite = true;
		return true;
	}
	return writeIntoFile();
}

inline bool ZIni::remove(const char *mainKey, const char *subKey)
{
	if (!m_bOpen) return false;
	std::map<std::string, std::map<std::string, std::string>>::iterator mainIter;;
	std::map<std::string, std::string>::iterator subIter;
	bool mainKeyExist = ((mainIter = mainMap.find(mainKey)) != mainMap.end());
	bool subKeyExist = false;
	if (subKey)
	{
		subKeyExist = mainKeyExist ? ((subIter = ((*mainIter).second.find(subKey))) != ((*mainIter).second.end())) : false;
		if (!subKeyExist)  //it's a uncorrect parameter;
			return false;
	}
	if (!mainKeyExist)     //it's a uncorrect parameter;
	{
		return false;
	}
	else
	{
		if (subKeyExist)
		{
			(*mainIter).second.erase(subIter);
			std::string buffer("[");
			buffer.reserve(128);
			buffer += mainKey; buffer += ']';
			auto lineBeginIndex = filestring.find(subKey, filestring.find(buffer));
			//check if the index is valid, because there may be a value which is the same as the key.
			while (lineBeginIndex != std::string::npos)
			{
				if (isSubKeyIndexValid(lineBeginIndex + strlen(subKey)))
					break;
				lineBeginIndex = filestring.find(subKey, lineBeginIndex);
			}
			auto lineEndIndex = filestring.find('\n', lineBeginIndex);
			if (lineEndIndex != std::string::npos)    //did not reach the end-of-file
			{
				filestring.erase(lineBeginIndex, lineEndIndex - lineBeginIndex + 1);
			}
			else
			{
				filestring.erase(lineBeginIndex, filestring.length() - lineBeginIndex);
			}
		}
		else
		{
			mainMap.erase(mainIter);
			std::string buffer("[");
			buffer.reserve(128);
			buffer += mainKey; buffer += ']';
			auto mainKeyIndex = filestring.find(buffer);
			auto endIndex = filestring.find("\n[", mainKeyIndex);
			if (endIndex != std::string::npos)    //did not reach the end-of-file
			{
				filestring.erase(mainKeyIndex, endIndex - mainKeyIndex + 1);
			}
			else
			{
				filestring.erase(mainKeyIndex, filestring.length() - mainKeyIndex);
			}
		}
	}
	if (fastMode)
	{
		needWrite = true;
		return true;
	}
	return writeIntoFile();
}

inline void ZIni::enableFastMode()
{
	filestring.reserve((filestring.capacity()) * 2);
	fastMode = true;
}

inline bool ZIni::update()
{
	if (!needWrite)
		return true;
	return writeIntoFile();
}

inline std::string::size_type ZIni::removeSpace(std::string &str)
{
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" \r") + 1); //in case there is a '\r' left in the end-line.
	return str.length();
}

inline bool ZIni::isSubKeyIndexValid(unsigned index)
{
	return '=' == filestring[filestring.find_first_not_of(' ', index)];
}

inline bool ZIni::writeIntoFile()
{
	FILE *fp = fopen(filepath.c_str(), "wb");
	if (!fp) return false;
	fwrite(filestring.c_str(), 1, filestring.length(), fp);
	fclose(fp);
	needWrite = false;
	return true;
}


#endif
