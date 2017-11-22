//
// Created by yonatan on 11/15/17.
//

#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"
#include "../include/Commands.h"

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
    if(verbose==1 || verbose==3) {
        cout << "FileSystem::~Filesystem()" << endl;
    }
} //Destructor
FileSystem::FileSystem(const FileSystem &rhs): rootDirectory(new Directory(*rhs.rootDirectory)), workingDirectory(nullptr) {
    //rootDirectory=new Directory(*rhs.rootDirectory);
    string absPath = rhs.getWorkingDirectory().getAbsolutePath();
    CdCommand cd (absPath);
    cd.execute(*this);
    if(verbose==1 || verbose==3) {
        cout << "FileSystem::FileSystem(const FileSystem &rhs)" << endl;
    }
}//Copy constructor
FileSystem& FileSystem::operator=(const FileSystem &rhs) {
    if(this!=&rhs) {
        delete rootDirectory;
        rootDirectory = new Directory(*rhs.rootDirectory);
        string absPath = rhs.getWorkingDirectory().getAbsolutePath();
        CdCommand cd (absPath);
        cd.execute(*this);
    }
    if(verbose==1 || verbose==3) {
        cout << "FileSystem& FileSystem::operator=(const FileSystem &rhs)" << endl;
    }
    return *this;
}//Copy assignment operator
FileSystem::FileSystem(FileSystem &&rhs): rootDirectory(rhs.rootDirectory), workingDirectory(nullptr){
    //rootDirectory=rhs.rootDirectory;
    string absPath = rhs.getWorkingDirectory().getAbsolutePath();
    CdCommand cd (absPath);
    cd.execute(*this);
    rhs.rootDirectory=nullptr;
    rhs.workingDirectory=nullptr;
    if(verbose==1 || verbose==3) {
        cout << "FileSystem::FileSystem(FileSystem &&rhs)" << endl;
    }
}//Move constructor
FileSystem& FileSystem::operator=(FileSystem &&rhs) {
    if(this!=&rhs) {
        delete rootDirectory;
        rootDirectory=rhs.rootDirectory;
        string absPath = rhs.getWorkingDirectory().getAbsolutePath();
        CdCommand cd (absPath);
        cd.execute(*this);
        rhs.rootDirectory=nullptr;
        rhs.workingDirectory=nullptr;
    }
    if(verbose==1 || verbose==3) {
        cout << "FileSystem& FileSystem::operator=(FileSystem &&rhs)" << endl;
    }
    return *this;
}//Move assignment operator