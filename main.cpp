#include "Files.h"
#include "FileSystem.h"
#include "Commands.h"
#include "Environment.h"


using namespace std;
void test1(){
    FileSystem fs= FileSystem();
    PwdCommand pwd = PwdCommand("");

    LsCommand ls ("");
    LsCommand lsSize("-s");
    Directory *dir1 = new Directory("dir1", &fs.getRootDirectory());
    fs.getRootDirectory().addFile(dir1);
    Directory *dir2 = new Directory("dir2", &fs.getRootDirectory());
    fs.getRootDirectory().addFile(dir2);
    Directory *dir1_1 = new Directory("dir1_1", dir1);
    dir1->addFile(dir1_1);
    Directory *dir1_2 = new Directory("dir1_2", dir2);
    dir1->addFile(dir1_2);


    File *f2 = new File("aslnk", 51);
    File *f3 = new File("pozl", 13);
    File *f4 = new File("maskd", 42);
    File *f5 = new File("AZAZ", 2);
    File *f1 = new File("asdasz", 1);
    dir1->addFile(f1);
    dir1_1->addFile(f3);
    dir1_1->addFile(f5);
    dir1_2->addFile(f4);
    dir2->addFile(f2);
    cout<<"print sort for root by size:"<<endl;
    pwd.execute(fs);
    lsSize.execute(fs);
    CdCommand cd1("dir1");
    cd1.execute(fs);
    cout<<"\n"<<"print sort for dir1 by name:"<<endl;
    pwd.execute(fs);
    ls.execute(fs);
    cout<<"\n"<<"print sort for /dir1/dir1_1 by size: (without CDing there!)"<<endl;
    cout<<"first line is pwd before ls, last is pwd after last, should be equal"<<endl;
    pwd.execute(fs);
    LsCommand lsPath("-s /dir1/dir1_1");
    lsPath.execute(fs);
    pwd.execute(fs);
    LsCommand lsPath2("/dir2");
    cout<<"\n"<<"print sort for /dir2 by name: (without CDing there!)"<<endl;
    lsPath2.execute(fs);


} //cd pwd ls [-s]
void test2(){
    FileSystem fs=FileSystem();
    PwdCommand pwd("");
    LsCommand ls("");

    MkdirCommand mkdir("dir1/dir11/dir111");
    mkdir.execute(fs);
    MkdirCommand mkdir2("dir2");
    mkdir2.execute(fs);
    MkfileCommand mkfile("dir1/file1 100");
    mkfile.execute(fs);
    MkfileCommand mkfile2("dir1/dir11/file1 100");
    mkfile2.execute(fs);
    MkfileCommand mkfile3("dir1/dir11/dir111/file1 100");
    mkfile3.execute(fs);
    CpCommand cp("dir1 dir2");
    cp.execute(fs);
    ls.execute(fs);
    cout<<endl;

    cout<<"now we delete a file from dir1!"<<endl;
    CdCommand cd("dir2/dir1");

    cd.execute(fs);
    ls.execute(fs);
    //fs.getWorkingDirectory().removeFile("file1");
    fs.getWorkingDirectory();
    cout<<"after deletion:"<<endl;

    CdCommand cdRoot("/");
    cdRoot.execute(fs);
    ls.execute(fs);


} //mkdir mkfile cp
void test3(){
    FileSystem fs = FileSystem();
    LsCommand ls ("");
    MkdirCommand mkdir("dir1");
    mkdir.execute(fs);
    MkdirCommand mkdir2("dir2");
    mkdir2.execute(fs);
    MkfileCommand mkfile("dir1/file1 500");
    mkfile.execute(fs);

    MvCommand mv("dir1 dir2");
    ls.execute(fs);
    //mv.execute(fs);
    cout<<endl;
    ls.execute(fs);

}
void test4(){
    FileSystem fs=FileSystem();
    LsCommand ls("");
    PwdCommand pwd("");

    MkdirCommand mk1 ("dir1/dir11");
    MkdirCommand mk2 ("dir2");
    mk1.execute(fs); mk2.execute(fs);
    MkfileCommand mkfile1 ("dir1/file1 500");
    MkfileCommand mkfile2 ("dir1/dir11/file2 1000");
    mkfile1.execute(fs); mkfile2.execute(fs);
    RenameCommand rename("dir1/dir11 dir2");
    rename.execute(fs);
    MvCommand mv("/dir1/dir2 /");
    mv.execute(fs);
    ls.execute(fs);


}

    //test1();
    //test2();
    //test3();
    //test4();
    unsigned int verbose = 0;

int main(int, char **) {
    Environment env;
    env.start();

    return 0;
}

