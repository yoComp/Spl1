#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "FileSystem.h"

using namespace std;
class BaseCommand {
private:
	string args;

public:
	BaseCommand(string args);
	string getArgs();
	virtual void execute(FileSystem & fs) = 0;
	virtual string toString() = 0;
    virtual ~BaseCommand(){};
	virtual BaseCommand* clone() = 0;
};

class PwdCommand : public BaseCommand {
private:
public:
	PwdCommand(string args);
	void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
	virtual string toString();
    virtual ~PwdCommand(){};

	BaseCommand *clone();
};

class CdCommand : public BaseCommand {
private:
public:
	CdCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~CdCommand(){};

	BaseCommand *clone();
};

class LsCommand : public BaseCommand {
private:
    void lsPrint(Directory &pwd);
public:
	LsCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~LsCommand(){};

	BaseCommand *clone();
};

class MkdirCommand : public BaseCommand {
private:
public:
	MkdirCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~MkdirCommand(){};

	BaseCommand *clone();
};

class MkfileCommand : public BaseCommand {
private:
public:
	MkfileCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~MkfileCommand(){};

	BaseCommand *clone();
};

class CpCommand : public BaseCommand {
private:
public:
	CpCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~CpCommand(){};

	BaseCommand *clone();
};

class MvCommand : public BaseCommand {
private:
public:
	MvCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~MvCommand(){};

	BaseCommand *clone();
};

class RenameCommand : public BaseCommand {
private:
public:
	RenameCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~RenameCommand(){};

	BaseCommand *clone();
};

class RmCommand : public BaseCommand {
private:
public:
	RmCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~RmCommand(){};

	BaseCommand *clone();
};

class HistoryCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
public:
	HistoryCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
    virtual ~HistoryCommand(){};

	BaseCommand *clone();
};

class VerboseCommand : public BaseCommand {
private:
public:
	VerboseCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~VerboseCommand(){};

	BaseCommand *clone();
};

class ErrorCommand : public BaseCommand {
private:
public:
	ErrorCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    virtual ~ErrorCommand(){};

	BaseCommand *clone();
};

class ExecCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
	bool execCmdCheck(string arg);
public:
	ExecCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
    virtual ~ExecCommand(){};

	BaseCommand *clone();
};


#endif
