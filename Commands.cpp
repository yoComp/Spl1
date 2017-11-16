//
// Created by yonatan on 11/15/17.
//

#include "Commands.h"

BaseCommand::BaseCommand(string args):args(args) {};//Constructor
string BaseCommand::getArgs() {
    return args;
}

PwdCommand::PwdCommand(string args):BaseCommand(args){}; //Constructor
void PwdCommand::execute(FileSystem &fs) {
    cout<< fs.getWorkingDirectory().getAbsolutePath();
}

CdCommand::CdCommand(string args):BaseCommand(args) {}; //Constructor
void CdCommand::execute(FileSystem &fs) {
    string path = getArgs();
    if(path[0] == '/') { //absolute path
        fs.setWorkingDirectory(&fs.getRootDirectory());
        path.substr(1);
    }
    changePath(path, fs);
}
Directory* findChildrenByName(string name, Directory& pwd){
    if(pwd.getChildren().size() == 0) {
        return nullptr;
    }else {
        BaseFile *v = pwd.getChildren()[0];
        for (int i = 0; i < pwd.getChildren().size(); ++i) {
            if(v[i].getName() == name) {
                if (v[i].typeCheck()) { return ((Directory *) &v[i]); }
                else { return nullptr; }
            }
        }
        return nullptr;
    }
} //changePath help method, returns nullptr if not found
void BaseCommand::changePath(string path, FileSystem &fs) {
    if (path.length() != 0) {
        string str = "";
        int i;
        for (i = 0; (i < path.length()) && path[i] != '/'; ++i) {
            str = str + path[i];
        }
        if (str == "..") {
            if (fs.getWorkingDirectory().getParent() == nullptr) {
                cout << "The system cannot find the path specified" << endl;
                return;
            } else {
                fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                if (i + 1 > path.length()) { i--; }
                changePath(path.substr(i+1), fs);
            }
        } else {
            Directory *dir = findChildrenByName(str, fs.getWorkingDirectory());
            if (dir == nullptr) {
                cout << "The system cannot find the path specified" << endl;
                return;
            } else {
                fs.setWorkingDirectory(dir);
                if (i + 1 > path.length()) { i--; }
                changePath(path.substr(i+1), fs);
            }
        }
    }
}
string CdCommand::toString() { return "cd"; }

void LsCommand::lsPrint(Directory &pwd){
    BaseFile *f = pwd.getChildren()[0];
    for (int i = 0; i < pwd.getChildren().size(); ++i) {
        if(f[i].typeCheck()){
            cout<< "DIR     " << f[i].getName() << "    "<< f[i].getSize()<<endl;
        }else{
            cout<<"FILE     "<< f[i].getName() <<  "    "<< f[i].getSize()<<endl;
        }
    }
}// Prints out the subdirectories
LsCommand::LsCommand(string args) :BaseCommand(args){}; //Constructor
void LsCommand::execute(FileSystem &fs) {
    string command = getArgs();
    if(command.length()==0 || command.compare("-s")==0){
        if(command.length()==0){//prints pwd contents by name
            fs.getWorkingDirectory().sortByName();
            lsPrint(fs.getWorkingDirectory());
        }else{//prints pwd contents by size
            fs.getWorkingDirectory().sortBySize();
            lsPrint(fs.getWorkingDirectory());
        }
    }else if (command.compare(0,2, "-s ")){ //prints sorted by size, handles [-s] modifier
        command=command.substr(3);
        CdCommand cd(command);
        cd.execute(fs);
        LsCommand ls("-s");
        ls.execute(fs);
    }else{ //prints sorted by name
        CdCommand cd(command);
        cd.execute(fs);
        LsCommand ls("");
        ls.execute(fs);
    }
}
string LsCommand::toString() {
    string command = getArgs();
    if(command.compare(0,1, "-s")){ return "ls -s";}
    else{ return "ls";}
};


bool nameCheck(string name){
    bool valid;
    string legal ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i=0; i<name.length(); i++){
        valid=false;
        for(int j=0; j<legal.length() && !valid; j++){
            if(name[i]==legal[j]){
                valid = true;
            }
        }
        if(!valid){return false;};
    }
    return true;
} //name is valid: a-z, A-Z, 0-9 chars only
bool nameExists(string name, Directory& pwd){
    BaseFile *f = pwd.getChildren()[0];
    for (int i = 0; i < pwd.getChildren().size(); ++i) {
        if(f[i].getName()==name){
            return true;
        }
    }
    return false;
} //given name exists @ target folder
bool isPath(string path){
    if (path.find("/")==path.npos){
        return false;
    }
    return true;
} //checks a string for '/' appearances
MkdirCommand::MkdirCommand(string args):BaseCommand(args){};//Constructor
string MkdirCommand::toString() {return "mkdir";}
void MkdirCommand::execute(FileSystem &fs) {
    Directory *savedPwd = fs.getWorkingDirectory();
    string path = getArgs();
    if(path[0] == '/'){
        fs.setWorkingDirectory(&fs.getRootDirectory());
        path=path.substr(1);
    }
    while(isPath(path)){//input is a path //change directory needed
        int split = path.find("/");
        string nextDirName = ("");
        for (int i = 0; i < split; ++i) {
            nextDirName = nextDirName+path[i];
        }
        path=path.substr(split+1);
        if(nextDirName==".."){
            fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
        }else{
            Directory *next = findChildrenByName(nextDirName, fs.getWorkingDirectory());
            if(next==nullptr){//no sub directory in given name: create
                if(!nameCheck(nextDirName)){
                    cout<<"Illegal name input"<<endl;
                    fs.setWorkingDirectory(savedPwd);
                    return;
                }
                Directory* dir=new Directory(nextDirName, fs.getWorkingDirectory());
                fs.getWorkingDirectory().addFile(dir);
                fs.setWorkingDirectory(dir);
            } else{
                if (!next->typeCheck()){//given name is a file!
                    cout<<"The directory already exists"<<endl;
                    fs.setWorkingDirectory(savedPwd);
                    return;
                }
                fs.setWorkingDirectory(next);
            }
        }
    }
    //we are at the specified PWD or we started at it
    if(!nameCheck(path)){       //IS IT NEEDED?
        cout<<"Illegal name input"<<endl;
        fs.setWorkingDirectory(savedPwd);
        return;
    }
    if(nameExists(path, fs.getWorkingDirectory())){
        cout<<"The directory already exists"<<endl;
        fs.setWorkingDirectory(savedPwd);
        return;
    }
    Directory *dir=new Directory(path, fs.getWorkingDirectory());
    fs.getWorkingDirectory().addFile(dir);
    fs.setWorkingDirectory(savedPwd);
}

MkfileCommand::MkfileCommand(string args):BaseCommand(args) {};
string MkfileCommand::toString() {return "mkfile";};
void MkfileCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory *pwd = fs.getWorkingDirectory();

    if(path[0]=='/'){
        pwd=fs.getRootDirectory();
    }
    while(isPath(path)){
        int split = path.find('/');
        string next=("");
        for (int i = 0; i < split; ++i) {
            next+=path[i];
        }
        path=path.substr(split+1);
        if(next = ".."){
            pwd=pwd->getParent();
        }else{
            Directory *f = findChildrenByName(next, pwd);
            if (f==nullptr){
                cout<<"The system cannot find the path specified"<<endl;
                return;
            }else{
                pwd=f;
            }
        }
    }//finished navigating path holds <name> <size>
    int spaceLoc = path.find(" ");
    string fname="";
    for (int j = 0; j < spaceLoc; ++j) {
        fname+=path;
    }
    if(!nameCheck(fname)){
        cout<<"Illegal name input"<<endl;
        return;
    }
    if(nameExists(fname, pwd)){
       cout<<"File already exists"<endl;
        return;
    }
    string fsize=path.substr(spaceLoc+1);
    int size=stoi(fsize);
    File *f = new File(fname, size);
    pwd->addFile(f);
}