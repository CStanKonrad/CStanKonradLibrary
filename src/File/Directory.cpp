/*
The MIT License (MIT)

Copyright (c) 2016 CStanKonrad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "Directory.hpp"
//#include <iostream>

namespace CSKL
{

	void CFile::openDir(std::string dirPath_)
	{
		dirPath_ += ".";
		directory = opendir(dirPath_.c_str());

		if (directory == nullptr)
			throw (std::string("CFile::openDir: Can't open directory: ") + dirPath_);
	}

	void CFile::closeDir()
	{
		closedir(directory);
		return;
	}

	std::string CFile::nextFile()
	{
		if (directory == nullptr)
			throw "CFile::nextFile: Directory not opened";

		dirElement = readdir(directory);

		if (dirElement == nullptr)
			return std::string("");
		else
			return std::string(dirElement->d_name);
	}


	std::string readLine(std::ifstream &in_)
	{
		std::string result = "";
		while (in_.peek() != '\n' && in_.eof() == false)
		{
			result += in_.get();
		}
		in_.get();
		return result;
	}

	void createDir(std::string dir_)
	{
		CFile dir;
		try
		{
			dir.openDir(dir_);
		}
		catch (std::string _bug)
		{
			//std::cout << "Creating: " << toDir_ << std::endl;
			system((std::string("mkdir -p \"") + dir_ + std::string("\"")).c_str());
		}
	}

	void copyFile(std::string from_, std::string toDir_, std::string toFile_)
	{
		createDir(toDir_);
		system((std::string("cat \"") + from_ + std::string("\" > \"") +  toDir_ + toFile_ + std::string("\"")).c_str());
	}

}
