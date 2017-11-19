//
// Created by yonatan on 11/15/17.
//

#include "Commands.h"
#include "Files.h"
BaseCommand::BaseCommand(string args):args(args) {};//Constructor
string BaseCommand::getArgs() {
    return args;
}

bool isPath(string path){
    if (path.find("/")==path.npos){
        return false;
    }
    return true;
} //checks a string for '/' appearances
Directory* findChildrenByName(string name, const vector<BaseFile*> &v){
    if (v.size() == 0){
        return nullptr;
    }else{
        for (int i = 0; i < v.size(); i++) {
            if(v[i]->getName()==name) {
                if (v[i]->typeCheck()) { return dynamic_cast<Directory *>(v[i]); }
                else { return nullptr; }
            }
        }
    }/*
    if(pwd.getChildren().size() == 0) {
        return nullptr;
    }else {
        vector<BaseFile*> v = pwd.getChildren();

        for (int i = 0; i < pwd.getChildren().size(); ++i) {
            if(v[i]->getName() == name) {
                if (v[i]->typeCheck()) { return ((Directory *) &v[i]); }
                else { return nullptr; }
            }
        }
        return nullptr;
    }*/
}
PwdCommand::PwdCommand(string args):BaseCommand(args){}; //Constructor
void PwdCommand::execute(FileSystem &fs) {
    cout<< fs.getWorkingDirectory().getAbsolutePath()<<endl;
}
string PwdCommand::toString() {return "pwd";}

CdCommand::CdCommand(string args):BaseCommand(args) {}; //Constructor
/*void CdCommand::execute(FileSystem &fs) {
    string path = getArgs();
    if(path[0] == '/') { //absolute path
        fs.setWorkingDirectory(&fs.getRootDirectory());
        path = path.substr(1);
    }
    changePath(path, fs);
}*/
void CdCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory* pwd = &fs.getWorkingDirectory();
    if(path[0]=='/'){
        pwd=&fs.getRootDirectory();
        path=path.substr(1);
    }
    if(path.length()==0){
        fs.setWorkingDirectory(&fs.getRootDirectory());
        return; }
    while(isPath(path)){
        int split = path.find('/');
        string next=("");
        for (int i = 0; i < split; ++i) {
            next+=path[i];
        }
        path=path.substr(split+1);
        if(next == ".."){
            pwd=pwd->getParent();
        }else{
            Directory *f = findChildrenByName(next, pwd->getChildren());
            if (f==nullptr){
                cout<<"The system cannot find the path specified"<<endl;
                return;
            }else{
                pwd=f;
            }
        }
    }//finished navigating path holds <name> of last dir
    if(path == ".."){
        fs.setWorkingDirectory(pwd->getParent());
    }else {
        pwd = findChildrenByName(path, pwd->getChildren());
        if (pwd == nullptr) {
            cout << "The system cannot find the path specified" << endl;
            return;
        } else {
            fs.setWorkingDirectory(pwd);
        }
    }
}
/*
void BaseCommand::changePath(string path, FileSystem &fs) {
    if (path.length() > 0) {
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
                path=path.substr(i+1);
                changePath(path, fs);
            }
        }
    }
}
*/
string CdCommand::toString() { return "cd"; }

void LsCommand::lsPrint(Directory &pwd){
    vector<BaseFile*> v = pwd.getChildren();
    for (int i = 0; i < v.size(); ++i) {
        if(v[i]->typeCheck()){
            cout<<"DIR    "<< v[i]->getName() <<"    "<< v[i]->getSize()<<endl;
        }else{
            cout<<"FILE   "<< v[i]->getName() <<"    "<< v[i]->getSize()<<endl;
        }
    }
}// Prints out the subdirectories
LsCommand::LsCommand(string args) :BaseCommand(args){}; //Constructor
void LsCommand::execute(FileSystem &fs) {
    string command = getArgs();
    Directory *pwd = &fs.getWorkingDirectory();
    if(command.length()==0 || command.compare("-s")==0){
        if(command.length()==0){//prints pwd contents by name
            fs.getWorkingDirectory().sortByName();
            lsPrint(fs.getWorkingDirectory());
        }else{//prints pwd contents by size
            fs.getWorkingDirectory().sortBySize();
            lsPrint(fs.getWorkingDirectory());
        }
    }else if (command.find("-s ")==0){ //prints sorted by size, handles [-s] modifier
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
    fs.setWorkingDirectory(pwd);
}
string LsCommand::toString() {return "ls";};


bool nameCheck(string name){
    bool valid;
    if(name.length()==0){ return false;}
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
bool nameExists(string name, const vector<BaseFile*> &v){
    for (int i = 0; i < v.size(); ++i) {
        if(v[i]->getName()==name){
            return true;
        }
    }
    return false;
} //given name exists @ target folder

MkdirCommand::MkdirCommand(string args):BaseCommand(args){};//Constructor
string MkdirCommand::toString() {return "mkdir";}
void MkdirCommand::execute(FileSystem &fs) {
    Directory *pwd = &fs.getWorkingDirectory();
    string path = getArgs();
    if(path[0] == '/'){
        pwd=&fs.getRootDirectory();
        path=path.substr(1);
    }
    while(isPath(path)) {
        int split = path.find("/");
        string next = ("");
        for (int i = 0; i < split; ++i) {
            next +=path[i];
        }
        path = path.substr(split + 1);
        if (next == "..") {
            pwd = pwd->getParent();
        } else {
            Directory *nextDir = findChildrenByName(next, pwd->getChildren());
            if (nextDir == nullptr) {//create subfolder to continue
                if (!nameCheck(next)) {
                    cout << "Illegal name input" << endl;
                    return;
                }
                Directory *newDir = new Directory(next, pwd);
                pwd->addFile(newDir);
                pwd=newDir;
            } else {
                if (!nextDir->typeCheck()) {
                    cout << "The directory already exists" << endl;
                    return;
                }
                pwd = nextDir;
            }
        }
    }
    if(!nameCheck(path)){
        cout<<"Illegal name input"<<endl;
        return; }
    if(nameExists(path, pwd->getChildren())){
        cout<<"The directory already exists"<<endl;
        return; }
    Directory *dir=new Directory(path, pwd);
    pwd->addFile(dir);
}

MkfileCommand::MkfileCommand(string args):BaseCommand(args) {};//Constructor
string MkfileCommand::toString() {return "mkfile";};
void MkfileCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory *pwd = &fs.getWorkingDirectory();
    if(path[0]=='/'){
        pwd=&fs.getRootDirectory();
        path=path.substr(1);
    }
    while(isPath(path)) {
        int split = path.find("/");
        string next = ("");
        for (int i = 0; i < split; ++i) {
            next +=path[i];
        }
        path = path.substr(split + 1);
        if (next == "..") {
            pwd = pwd->getParent();
        } else {
            Directory *nextDir = findChildrenByName(next, pwd->getChildren());
            if (nextDir == nullptr) {//create subfolder to continue
                if (!nameCheck(next)) {
                    cout << "Illegal name input" << endl;
                    return;
                }/*
                Directory *newDir = new Directory(next, pwd);
                pwd->addFile(newDir);
                pwd=newDir;*/
            } else {
                if (!nextDir->typeCheck()) {
                    cout << "The directory already exists" << endl;
                    return;
                }
                pwd = nextDir;
            }
        }
    }//finished navigating path holds <name> <size>
    int spaceLoc = path.find(" ");
    string fname="";
    for (int j = 0; j < spaceLoc; ++j) {
        fname+=path[j];
    }
    string fsize=path.substr(spaceLoc+1);
    int size=stoi(fsize);
    if(!nameCheck(fname)){
        cout<<"Illegal name input"<<endl;
        return;
    }
    if(nameExists(fname, pwd->getChildren())){
        cout<<"File already exists"<<endl;
        return; }

    File *f = new File(fname, size);
    pwd->addFile(f);
}
BaseFile* findFileByName(string name, const vector<BaseFile*> &v){
    if(v.size() == 0) {
        return nullptr;
    }else {
        for (int i = 0; i < v.size(); ++i) {
            if(v[i]->getName() == name) {
                 return v[i];
            }
        }
        return nullptr;
    }
}

CpCommand::CpCommand(string args):BaseCommand(args){};//Constructor
string CpCommand::toString() {return "cp";}
void CpCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory *src = &fs.getWorkingDirectory();
    Directory *dest = src;
    int spaceLoc=path.find(" ");
    string srcPath = path.substr(0, spaceLoc);
    string destPath = path.substr(spaceLoc+1);
    if(srcPath[0]=='/'){
        src=&fs.getRootDirectory();
        srcPath=srcPath.substr(1);
    }
    if(destPath[0]=='/'){
        dest=&fs.getRootDirectory();
        destPath=destPath.substr(1);
    }
    while(isPath(srcPath)){
        int split = srcPath.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next+=srcPath[i];
        }
        srcPath=srcPath.substr(split+1);
        if(next == ".."){
            src=src->getParent();
        }else{
            Directory *f = findChildrenByName(next, src->getChildren());
            if(f==nullptr){
                cout<<"No such file or directory"<<endl;
                return;
            }else{
                src=f;
            }
        }
    }//src = target file/directory's parent
    while(isPath(destPath)){
        int split = destPath.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next+=destPath[i];
        }
        destPath=destPath.substr(split+1);
        if(next == ".."){
            dest=dest->getParent();
        }else{
            Directory *f = findChildrenByName(next, dest->getChildren());
            if(f==nullptr){
                cout<<"No such file or directory"<<endl;
                return;
            }else{
                dest=f;
            }
        }
    }//dest = target file/directory's parent
    if(destPath.length()>0) {
        Directory *f = findChildrenByName(destPath, dest->getChildren());
        if (f == nullptr) {
            cout << "No such file or directory" << endl;
            return;
        } else {
            dest = f;
        }
    }//dest = target folder to copy in to
    BaseFile *toCopy = findFileByName(srcPath, src->getChildren());
    if(toCopy == nullptr){
        cout<<"No such file or directory"<<endl;
        return;
    }else{
        if(nameExists(toCopy->getName(), dest->getChildren())){
            cout<<"File with given name already exists"<<endl;
            return; }
        if(toCopy->typeCheck()){//toCopy is a directory, deep copy needed
            Directory *dir = new Directory(dynamic_cast<Directory&>(*toCopy));
            dir->setParent(dest);
            dest->addFile(dir);
        } else{
            dest->addFile(new File(dynamic_cast<File&>(*toCopy)));
        }
    }
}


MvCommand::MvCommand(string args):BaseCommand(args) {};//Constructor
string MvCommand::toString() {return "mv";}
void MvCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory *src = &fs.getWorkingDirectory();
    Directory *dest = src;
    int spaceLoc = path.find(" ");
    string srcPath = path.substr(0, spaceLoc);
    string destPath = path.substr(spaceLoc + 1);
    if (srcPath[0] == '/') {
        src = &fs.getRootDirectory();
        srcPath = srcPath.substr(1);
    }
    if (destPath[0] == '/') {
        dest = &fs.getRootDirectory();
        destPath = destPath.substr(1);
    }
    while (isPath(srcPath)) {
        int split = srcPath.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next += srcPath[i];
        }
        srcPath = srcPath.substr(split + 1);
        if (next == "..") {
            src = src->getParent();
        } else {
            Directory *f = findChildrenByName(next, src->getChildren());
            if (f == nullptr) {
                cout << "No such file or directory" << endl;
                return;
            } else {
                src = f;
            }
        }
    }//src = target file/directory's parent
    while (isPath(destPath)) {
        int split = destPath.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next += destPath[i];
        }
        destPath = destPath.substr(split + 1);
        if (next == "..") {
            dest = dest->getParent();
        } else {
            Directory *f = findChildrenByName(next, dest->getChildren());
            if (f == nullptr) {
                cout << "No such file or directory" << endl;
                return;
            } else {
                dest = f;
            }
        }
    }//dest = target file/directory's parent
    if (destPath.length() > 0) {
        Directory *f = findChildrenByName(destPath, dest->getChildren());
        if (f == nullptr) {
            cout << "No such file or directory" << endl;
            return;
        } else {
            dest = f;
        }
    }//dest = target folder to be moved to
    BaseFile *toMove = findFileByName(srcPath, src->getChildren());
    if (toMove == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    } else {
        if (nameExists(toMove->getName(), dest->getChildren())) {
            cout << "File with given name already exists" << endl;
            return;
        }
        if (toMove->typeCheck()) {//toMove is a directory, deep steal needed
            Directory dir = (dynamic_cast<Directory &>(*toMove));
            (&dir)->setParent(dest);
            dest->addFile(&dir);
        } else {
            File *file = new File(toMove->getName(), toMove->getSize());
            src->removeFile(toMove);
            dest->addFile(file);
        }
    }
}

/*
void MvCommand::execute(FileSystem &fs) {
    string argument = getArgs();
    Directory *mvSrc = &fs.getWorkingDirectory();
    Directory *mvDest = mvSrc;
    if (argument[0] == '/') {
        mvSrc = &fs.getRootDirectory();
        argument = argument.substr(1);
    }
    int spaceLoc = argument.find(" ");
    string src = argument.substr(0, spaceLoc);
    string dest = argument.substr(spaceLoc + 1);
    if (dest[0] == '/') {
        mvDest = &fs.getRootDirectory();
        dest = dest.substr(1);
    }

    while (isPath(src)) {
        int split = src.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next += src[i];
        }
        src = src.substr(split + 1);
        if (next == "..") {
            mvSrc = mvSrc->getParent();
        } else {
            Directory *f = findChildrenByName(next, *mvSrc);
            if (f == nullptr) {
                cout << "No such file or directory" << endl;
                return;
            } else {
                mvSrc = f;
            }
        }
    }
    BaseFile *f = findFileByName(src, *mvSrc);
    if (f == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    if (f->typeCheck()) {
        mvSrc = (Directory *) f;
    }
    string mvPath = mvSrc->getAbsolutePath();
    if (!f->typeCheck()) {
        mvPath += "/" + f->getName();
    }
    string pwd = fs.getWorkingDirectory().getAbsolutePath();
    if (pwd.find(mvPath) == 0) {
        cout << "Can’t move directory" << endl;
        return;
    }
    if (mvSrc->typeCheck()) {
        mvSrc = mvSrc->getParent();
    }
    //f holds basefile to be moved, mvSrc holds its parent
    while (isPath(dest)) {
        int split = dest.find("/");
        string next = "";
        for (int i = 0; i < split; ++i) {
            next += dest[i];
        }
        dest = dest.substr(split + 1);
        if (next == "..") {
            mvDest = mvDest->getParent();
        } else {
            Directory *f = findChildrenByName(next, *mvDest);
            if (f == nullptr) {
                cout << "No such file or directory" << endl;
                return;
            } else {
                mvDest = f;
            }
        }
    }//mvDest holds destination's parent
    Directory *destination = findChildrenByName(dest, *mvDest);
    if (destination == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    if (nameExists(f->getName(), *destination)) {
        return;
    }
    if (f->typeCheck()) {//f is a directory
        Directory *moved((Directory *) f);
        moved->setParent(destination);
        destination->addFile(moved);
        mvSrc->removeFile(
                f);//If f was moved correctly: deletes f pointer from its original father. else copied and deleted properly from original father.
    } else {
        File *moved((File *) f);
        destination->addFile(moved);
        mvSrc->removeFile(f);
    }
}*/
RenameCommand::RenameCommand(string args):BaseCommand(args){}; //Constructor
string RenameCommand::toString() {return "rename";}
void RenameCommand::execute(FileSystem &fs) {
    string path = getArgs();
    Directory* target = &fs.getWorkingDirectory();
    int spaceLoc = path.find(" ");
    string newName = path.substr(spaceLoc+1);
    path=path.substr(0,spaceLoc);
    if(path[0] == '/'){
        target=&fs.getRootDirectory();
    }
    if(path.length()>0) {
        while (isPath(path)) {
            int split = path.find("/");
            string next = path.substr(0, split);
            path = path.substr(split + 1);

            if (next == "..") {
                target = target->getParent();
            }else{
                Directory* nextDir = findChildrenByName(next, target->getChildren());
                if (nextDir == nullptr || !nextDir->typeCheck()){
                    cout<<"No such file or directory"<<endl;
                    return;}
                target=nextDir;
            }
        }
    }//target = the target file/folder parent
    BaseFile* file = findFileByName(newName, target->getChildren());
    if (file == nullptr){
        cout<<"No such file or directory"<<endl;
        return;}
    if(!nameCheck(newName)){
        cout<<"Illegal name input"<<endl;
        return;}
    if(nameExists(newName, target->getChildren())){
        cout<<"File with that name already exists"<<endl;
        return;}
    if(file==&fs.getWorkingDirectory()){
        cout<<"Can't rename the working directory"<<endl;
        return;}
    file->setName(newName);
}

RmCommand::RmCommand(string args):BaseCommand(args){}; //Constructor
string RmCommand::toString() {return "rm";}
void RmCommand::execute(FileSystem &fs) {
    Directory* savedPwd = &fs.getWorkingDirectory();
    string command = getArgs();
    int split = command.find_last_of("/");

    string name = command.substr(split+1);
    command=command.substr(0, split);
    CdCommand cd(command);
    cd.execute(fs);

    BaseFile* target=findFileByName(name, fs.getWorkingDirectory().getChildren());
    if (target == nullptr){
        cout<<"No such file or Directory"<<endl;
        return;}
    if(target == savedPwd){
        cout<<"Can't remove directory"<<endl;
        return;}
    fs.getWorkingDirectory().removeFile(target);
    fs.setWorkingDirectory(savedPwd);
}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history):BaseCommand(args), history(history){}; //Constructor
string HistoryCommand::toString() {return "history";}
void HistoryCommand::execute(FileSystem &fs) {
    for (int i = 0; i <history.size() ; ++i) {
        cout<<i<<"    "<<history[i]->toString()<<" "<<history[i]->getArgs()<<endl;
    }
}

VerboseCommand::VerboseCommand(string args):BaseCommand(args){}; //Constructor
string VerboseCommand::toString() {return "verbose";}
void VerboseCommand::execute(FileSystem &fs) {
    string argument = getArgs();
    int input = stoi(argument);
    if(input > 3 || input < 0){
        cout<<"Wrong verbose input"<<endl;
        return;}

}//Implement this!

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history):BaseCommand(args), history(history){}; //Constructor
string ExecCommand::toString() {return "exec";}
void ExecCommand::execute(FileSystem &fs) {
    string argument = getArgs();
    if(!execCmdCheck(argument)){
        cout<<"Illegal input"<<endl;
        return;}
    int cmdNum = stoi(argument);
    if(cmdNum<0 || cmdNum>history.size()){
        cout<<"Command not found"<<endl;
        return;}
    BaseCommand* cmd = history[cmdNum];
    cmd->execute(fs);
}
bool ExecCommand::execCmdCheck(string arg){
    bool valid;
    if(arg.length()==0){ return false;}
    string legal ="0123456789";
    for(int i=0; i<arg.length(); i++){
        valid=false;
        for(int j=0; j<legal.length() && !valid; j++){
            if(arg[i]==legal[j]){
                valid = true;
            }
        }
        if(!valid){return false;};
    }
    return true;
}


ErrorCommand::ErrorCommand(string args):BaseCommand(args){};
string ErrorCommand::toString() {return getArgs();}
void ErrorCommand::execute(FileSystem &fs) {
    string argument = getArgs();

}


