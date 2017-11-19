//
// Created by yonatan on 11/19/17.
//

#include "Environment.h"

Environment::Environment(): fs(FileSystem()), commandsHistory(){}; //Constructor
FileSystem& Environment::getFileSystem() {return fs;}
const vector<BaseCommand*>& Environment::getHistory() const {return commandsHistory;}
void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

//Rule of 5 implementation

Environment::~Environment(){
    for (int i = 0; i < commandsHistory.size(); ++i) {
        delete commandsHistory[i];
    }
    commandsHistory.clear();
    //delete &fs;
}//Destructor

Environment::Environment(const Environment &rhs) {
    fs=FileSystem(rhs.fs);
    commandsHistory=rhs.commandsHistory;
}//Copy constructor
Environment& Environment::operator=(const Environment &rhs) {
    if(this!=&rhs){
        fs=rhs.fs;
        commandsHistory=rhs.commandsHistory;
    }
    return *this;
}//Copy assignment operator
Environment::Environment(Environment &&rhs) {
    fs=rhs.fs;
    delete &rhs.fs;
    commandsHistory=rhs.commandsHistory;
    rhs.commandsHistory.clear();
}//Move construtcor
Environment& Environment::operator=(Environment &&rhs) {
    if(this!=&rhs){
        fs=rhs.fs;
        delete &rhs.fs;
        commandsHistory=rhs.commandsHistory;
        rhs.commandsHistory.clear();
    }
    return *this;
}//Move assignment operator

void Environment::start() {
    string input;
    while(input!="exit") {
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        getline(cin, input);
        callToCmd(input);
    }
}
void Environment::callToCmd(string cmd){
    int spaceLoc = cmd.find(" ");
    string args = cmd.substr(spaceLoc+1);
    cmd=cmd.substr(0, spaceLoc);
    if(cmd=="pwd"){
        PwdCommand *pwd= new PwdCommand("");
        pwd->execute(fs);
        addToHistory(pwd);
    } else if(cmd == "exit") {
        return;
    }else if(cmd == "ls"){
        if(args == cmd){
            args = "";}
        LsCommand *ls = new LsCommand(args);
        ls->execute(fs);
        addToHistory(ls);
    } else if(cmd == "cd"){
        CdCommand *cd = new CdCommand(args);
        cd->execute(fs);
        addToHistory(cd);
    } else if(cmd == "mkdir"){
        MkdirCommand *mkdir = new MkdirCommand(args);
        mkdir->execute(fs);
        addToHistory(mkdir);
    } else if(cmd == "mkfile"){
        MkfileCommand *mkfile = new MkfileCommand(args);
        mkfile->execute(fs);
        addToHistory(mkfile);
    } else if(cmd == "rm"){
        RmCommand *rm = new RmCommand(args);
        rm->execute(fs);
        addToHistory(rm);
    } else if(cmd == "rename"){
        RenameCommand *rename = new RenameCommand(args);
        rename->execute(fs);
        addToHistory(rename);
    } else if(cmd == "cp"){
        CpCommand *cp = new CpCommand(args);
        cp->execute(fs);
        addToHistory(cp);
    } else if(cmd == "mv"){
        MvCommand *mv = new MvCommand(args);
        mv->execute(fs);
        addToHistory(mv);
    } else if(cmd == "history"){
        HistoryCommand *hist = new HistoryCommand("", getHistory());
        hist->execute(fs);
        addToHistory(hist);
    } else if(cmd=="verbose"){
        VerboseCommand *verb = new VerboseCommand(args);
        verb->execute(fs);
        addToHistory(verb);
    } else if(cmd=="exec"){
        ExecCommand *exec = new ExecCommand(args, getHistory());
        exec->execute(fs);
        addToHistory(exec);
    } else {
        string fullarg = cmd + " " + args;
        ErrorCommand *err = new ErrorCommand(fullarg);
        err->execute(fs);
        addToHistory(err);
    }
}