/*  ZIni: A Real Crude Cross-Platform Ini-File-Reader Written by C++
*
*   version  :  v0.1.0
*   Author   :  SHZY
*   Email    :  188813950@qq.com
*   now-time :  2021-1-28
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
	int getInt(const char *mainKey, const char *subKey, int defaultValue = 0)
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
	long long getLl(const char *mainKey, const char *subKey, long long defaultValue = 0)
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
	unsigned long getUl(const char *mainKey, const char *subKey, unsigned long defaultValue = 0)
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
	unsigned long long getUll(const char *mainKey, const char *subKey, unsigned long long defaultValue = 0)
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
	float getFloat(const char *mainKey, const char *subKey, float defaultValue = 0.0f)
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
	double getDouble(const char *mainKey, const char *subKey, double defaultValue = 0.0)
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
	std::string get(const char *mainKey, const char *subKey, const char *defaultCStr = "")
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
	std::map<std::string, std::string> & operator[] (std::string mainKey)
	{
		if (mainMap.count(mainKey) == 0)
		{
			return emptyMap;
		}
		return mainMap[mainKey];
	}
	bool is_open()
	{
		return b_open;
	}
	bool set(const char *mainKey, const char *subKey, const char *subValue)
	{
		if (!b_open) return false;
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

	void enableFastMode()
	{
		filestring.reserve((filestring.capacity()) * 2);
		fastMode = true;
	}
	bool update()
	{
		if (!needWrite)
			return true;
		return writeIntoFile();
	}

private:
	std::string filepath;
	std::string filestring;
	std::map<std::string, std::map<std::string, std::string>> mainMap;
	std::map<std::string, std::string> emptyMap;
	bool b_open = false;
	bool fastMode = false;
	bool needWrite = false;

private:
	std::string::size_type removeSpace(std::string &str)
	{
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" \r") + 1); //in case there is a '\r' left in the end-line.
		return str.length();
	}
	bool isSubKeyIndexValid(unsigned int index)
	{
		return '=' == filestring[filestring.find_first_not_of(' ', index)];
	}
	bool writeIntoFile()
	{
		FILE *fp = fopen(filepath.c_str(), "wb");
		if (!fp) return false;
		fwrite(filestring.c_str(), 1, filestring.length(), fp);
		fclose(fp);
		needWrite = false;
		return true;
	}
};

inline ZIni::ZIni(const char *filePath)
{
	FILE *fp = fopen(filePath, "rb");
	if (fp)
	{
		b_open = true;
		filepath = filePath;
	}
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



#endif