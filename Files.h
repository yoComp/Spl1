#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;
class BaseFile {
private:
	string name;
	
public:
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
    virtual bool typeCheck() = 0;
	virtual void clean(){};

	void print(){cout<<"BaseFile: "<<name<<"."<<endl;}

};

class File : public BaseFile {
private:
	int size;
	void clean(){};
public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
    bool typeCheck();


	void print(){cout<<"File: "<<getName()<<", size: "<<getSize()<<"."<<endl;}
	
};

class Directory : public BaseFile {
private:
	vector<BaseFile*> children;
	Directory *parent;


	void clean();
    void copy(const Directory &rhs);
    void steal(Directory &rhs);


public:
	Directory(string name, Directory *parent); // Constructor
	virtual ~Directory(); // Destructor
	Directory(const Directory &rhs); //Copy Constructor
	Directory& operator=(const Directory &rhs); //Copy assignment operator
	Directory(Directory &&rhs); //Move constructor
	Directory& operator=(Directory &&rhs);//Move assignment operator

	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	string getAbsolutePath();  //Return the path from the root to this
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	bool typeCheck();

    void print();


};

#endif