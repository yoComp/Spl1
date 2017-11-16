#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
	FileSystem(); //Default constructor

	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system

	virtual ~FileSystem(); //Destructor
	FileSystem(const FileSystem &rhs); //Copy constructor
	FileSystem& operator=(const FileSystem &rhs); //Copy assignment operator
	FileSystem(FileSystem &&rhs); //Move constructor
	FileSystem& operator=(FileSystem &&rhs); //Move assignment operator

};


#endif
