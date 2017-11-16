//
// Created by yonatan on 11/11/17.
//

#include "Files.h"
#include <algorithm>
#include <iostream>

//BaseFile methods
BaseFile::BaseFile(string name):name(name){ //Constructor
    //this->name=name;
}
//BaseFile::BaseFile():name("empty"){};
string BaseFile::getName() const{
    return name;
}
void BaseFile::setName(string newName){
    name=newName;
}



//File methods
File::File(string name, int size):BaseFile(name),size(size){} //Constructor
int File::getSize() {
    return size;
}
bool File::typeCheck() {return false;}

//Directory rule of 5+Default constructor
Directory::Directory(string name, Directory *parent):BaseFile(name),children(),parent(parent){} //Constructor
Directory::~Directory(){
    clean();
} //Destructor
Directory::Directory(const Directory &rhs):BaseFile(rhs.getName()){
    copy(rhs);
}//Copy Constructor
Directory& Directory::operator=(const Directory &rhs) {
    if(this!=&rhs){
        clean();
        copy(rhs);
    }
    return *this;
}//Copy assignment operator(=)
Directory::Directory(Directory &&rhs):BaseFile("temp") {
    steal(rhs);
}//Move constructor
Directory& Directory::operator=(Directory &&rhs) {
    if(this!=&rhs){
        clean();
        steal(rhs);
    }
    return *this;
}//Move assignment operator(=)

//Directory methods

//Directory Methods
Directory* Directory::getParent() const {
    return parent;
}
void Directory::setParent(Directory *newParent) {
    parent=newParent;
}
void Directory::addFile(BaseFile *file) {
    children.push_back(file);
    if(file->typeCheck()){
        dynamic_cast<Directory*>(file)->setParent(this);
    }
}
void Directory::removeFile(string name) {
    for (int i = 0; i < children.size(); ++i) {
        if(children[i]->getName()==name){
            children[i]->clean();
            children.erase(children.begin()+i);
            break;
        }
    }
}
void Directory::removeFile(BaseFile *file) {
    for(int i=0; i<children.size(); i++){
        if(children[i] == file){
            children[i]->clean();
            children.erase(children.begin()+i);
            break;
        }
    }
}
vector<BaseFile*> Directory::getChildren() {
    return children;
}
string Directory::getAbsolutePath() {
    string path="";
    Directory* me = this;
    while(me!=nullptr) {
        path = '/' + me->getName() + path;
        me=me->getParent();
    }
    return path;
}
bool nameComp (BaseFile *a, BaseFile *b){
    return (*a).getName()<(*b).getName();
}
bool sizeComp (BaseFile *a, BaseFile *b){
    if((*a).getSize()==(*b).getSize()){
        return nameComp(a, b);}
    return (*a).getSize()<(*b).getSize();
}
void Directory::sortByName() {
    sort(children.begin(), children.end(), nameComp);
}
void Directory::sortBySize() {
    sort(children.begin(), children.end(), sizeComp);
}
int Directory::getSize() {
    int sum=0;
    for (int i = 0; i < children.size(); ++i) {
        if(children[i]->typeCheck()){
            sum=sum+(dynamic_cast<Directory*>(children[i])->getSize());
        }else{
            sum=sum+dynamic_cast<File*>(children[i])->getSize();
        }
    }
    return sum;
}

//Assisting methods
void Directory::copy(const Directory &rhs){
    this->setName(rhs.getName());
    this->parent=rhs.parent;

    for (int i = 0; i < rhs.children.size(); ++i) {
        children.push_back(rhs.children[i]);
    }
}
void Directory::steal(Directory &rhs){
    this->setName(rhs.getName());
    rhs.setName("void");
    this->parent=rhs.parent;
    rhs.parent=nullptr;
    for(int i=0; i<children.size(); i++){
        children[i]->clean();
    }
    children.clear();
    children=rhs.children;
    rhs.children=vector<BaseFile*>();
}
void Directory::clean() {
    for (int i = 0; i < children.size(); ++i) {
        children[i]->clean();
    }
    children.clear();
    parent=nullptr;
}
void Directory::print(){
    cout<<"folder: "<<this->getName()<<". ";
    if(parent!=nullptr) {
        cout << "Parent: " << this->parent->getName() << "." << endl;
    }else{ cout<<"parent is NULL!"<<endl;}
    if(!children.empty()){
        for(int i=0; i<children.size(); i++){
            cout<<children[i]->getName()<<"-"<<children[i]->getSize()<<" :: ";
        }
    }
    cout<<endl;


}
bool Directory::typeCheck() {return true;}

