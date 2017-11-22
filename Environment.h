#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

	void callToCmd(string command);

public:
	Environment();
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands


	//Rule of 5
	virtual ~Environment(); //Destructor
	Environment(const Environment &rhs); //Copy constructor
	Environment& operator=(const Environment &rhs); //Copy assignment operator
	Environment(Environment &&rhs); //Move constructor
	Environment& operator=(Environment &&rhs); //Move assignment operator

};

#endif