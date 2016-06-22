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

	void CFile::openDir(std::string _dirPath)
	{
		_dirPath += ".";
		directory = opendir(_dirPath.c_str());

		if (directory == nullptr)
			throw (std::string("CFile::openDir: Can't open directory: ") + _dirPath);
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


	std::string readLine(std::ifstream &_in)
	{
		std::string result = "";
		while (_in.peek() != '\n' && _in.eof() == false)
		{
			result += _in.get();
		}
		_in.get();
		return result;
	}

	void createDir(std::string _dir)
	{
		CFile dir;
		try
		{
			dir.openDir(_dir);
		}
		catch (std::string _bug)
		{
			//std::cout << "Creating: " << _toDir << std::endl;
			system((std::string("mkdir -p \"") + _dir + std::string("\"")).c_str());
		}
	}

	void copyFile(std::string _from, std::string _toDir, std::string _toFile)
	{
		createDir(_toDir);
		system((std::string("cat \"") + _from + std::string("\" > \"") +  _toDir + _toFile + std::string("\"")).c_str());
	}

}
