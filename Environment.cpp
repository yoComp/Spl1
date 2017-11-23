//
// Created by yonatan on 11/19/17.
//

#include "../include/Environment.h"
#include "../include/GlobalVariables.h"

Environment::Environment(): commandsHistory(), fs(FileSystem()){}; //Constructor
FileSystem& Environment::getFileSystem() {return fs;}
const vector<BaseCommand*>& Environment::getHistory() const {return commandsHistory;}
void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

//Rule of 5 implementation

Environment::~Environment(){
    for (size_t i = 0; i < commandsHistory.size(); ++i) {
        delete commandsHistory[i];
    }
    commandsHistory.clear();
    if(verbose==1 || verbose==3) {
        cout << "Environment::~Environment()" << endl;
    }
}//Destructor

Environment::Environment(const Environment &rhs): commandsHistory(),fs(FileSystem(rhs.fs)){
    for(size_t i=0; i<rhs.commandsHistory.size(); ++i){
        commandsHistory.push_back(rhs.commandsHistory[i]->clone());
    }
    if(verbose==1 || verbose==3) {
        cout << "Environment::Environment(const Environment &rhs)" << endl;
    }
}//Copy constructor
Environment& Environment::operator=(const Environment &rhs) {
    if(this!=&rhs){
        fs=rhs.fs;
        for(size_t i=0; i<rhs.commandsHistory.size(); ++i){
            commandsHistory.push_back(rhs.commandsHistory[i]->clone());
        }
    }
    if(verbose==1 || verbose==3) {
        cout << "Environment& Environment::operator=(const Environment &rhs)" << endl;
    }
    return *this;
}//Copy assignment operator
Environment::Environment(Environment &&rhs):commandsHistory(move(rhs.commandsHistory)),fs(move(rhs.fs)) {
    rhs.commandsHistory=vector<BaseCommand*>();
    if(verbose==1 || verbose==3) {
        cout << "Environment::Environment(Environment &&rhs)" << endl;
    }
}//Move construtcor
Environment& Environment::operator=(Environment &&rhs) {
    if(this!=&rhs){
        fs=move(rhs.fs);
        //delete &rhs.fs;
        commandsHistory=move(rhs.commandsHistory);
        //rhs.commandsHistory.clear();
    }
    if(verbose==1 || verbose==3) {
        cout << "Environment& Environment::operator=(Environment &&rhs)" << endl;
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
    if(verbose==2||verbose==3){
        if(cmd=="exit"){return;}
        cout<<cmd<<endl;
    }
    string args = cmd.substr(spaceLoc+1);
    cmd=cmd.substr(0, spaceLoc);
    if(cmd=="pwd"){
        PwdCommand *pwd= new PwdCommand("");
        pwd->execute(fs);
        addToHistory(pwd);
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
    } else if(cmd=="exec") {
        ExecCommand *exec = new ExecCommand(args, getHistory());
        exec->execute(fs);
        addToHistory(exec);
    }else if(cmd=="exit"){
        return;
    } else {
        string fullArg = cmd + " " + args;
        ErrorCommand *err = new ErrorCommand(fullArg);
        err->execute(fs);
        addToHistory(err);
    }
}