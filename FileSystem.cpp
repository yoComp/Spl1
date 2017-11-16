//
// Created by yonatan on 11/15/17.
//

#include "FileSystem.h"

FileSystem::FileSystem(): rootDirectory(new Directory("root", nullptr)), workingDirectory(rootDirectory){};//constructor

Directory& FileSystem::getRootDirectory() const {
    return *rootDirectory;
}
Directory& FileSystem::getWorkingDirectory() const {
    return *workingDirectory;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory){
    workingDirectory=newWorkingDirectory;
}

//Rule of 5 implementation
FileSystem::~FileSystem() {
    delete rootDirectory;
    rootDirectory=nullptr;
    workingDirectory=nullptr;
} //Destructor
FileSystem::FileSystem(const FileSystem &rhs) {
    delete rootDirectory;
    rootDirectory=rhs.rootDirectory;
    workingDirectory=rootDirectory;
}//Copy constructor
FileSystem& FileSystem::operator=(const FileSystem &rhs) {
    if(this!=&rhs) {
        delete rootDirectory;
        rootDirectory = rhs.rootDirectory;
        workingDirectory = rootDirectory;
    }
    return *this;
}//Copy assignment operator
FileSystem::FileSystem(FileSystem &&rhs) {
    delete rootDirectory;
    rootDirectory=rhs.rootDirectory;
    workingDirectory=rootDirectory;
    rhs.rootDirectory=nullptr;
    rhs.workingDirectory=nullptr;
}//Move constructor
FileSystem& FileSystem::operator=(FileSystem &&rhs) {
    if(this!=&rhs) {
        delete rootDirectory;
        rootDirectory=rhs.rootDirectory;
        workingDirectory=rootDirectory;
        rhs.rootDirectory=nullptr;
        rhs.workingDirectory=nullptr;
    }
    return *this;
}//Move assignment operator