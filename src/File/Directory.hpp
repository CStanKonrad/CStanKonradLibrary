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

#ifndef CSTANKONRADLIBRARY_FILE_DIRECTORY
#define CSTANKONRADLIBRARY_FILE_DIRECTORY

#include <string>
#include <fstream>

#include <dirent.h>

namespace CSKL
{

	class CFile
	{
	private:
		DIR *directory = nullptr;
		dirent *dirElement = nullptr;
	public:
		~CFile()
		{
			if (directory != nullptr)
				closeDir();
		}
		/* throw string exception if directory can't be opened
		 * throws "CFile::openDir: Can't open directory: dirPath_" */
		void openDir(std::string dirPath_);  // for example "my/directory/" will enable browsing files from this directory
		void closeDir();

		/* nextFile()
		* return next file or directory name if exists or void string if doesn't exist
		* throws "CFile::nextFile: Directory not opened"
		*/
		std::string nextFile();
	};

	/* read characters to string while (next_character != new_line) and read new_line (but not to string) */
	std::string readLine(std::ifstream &in_);
	/* If no directory named dir_ exists create one */
	void createDir(std::string dir_);
	/* copy content from from_ to toDir_/toFile_ */
	void copyFile(std::string from_, std::string toDir_, std::string toFile_);

}
#endif // CSTANKONRAD_SOLUTIONCHECKER_FILE_HPP_INCLUDED
