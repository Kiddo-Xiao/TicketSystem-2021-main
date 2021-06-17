#ifndef TICKETSYSTEM_2021_MAIN_FILE_OI_HPP
#define TICKETSYSTEM_2021_MAIN_FILE_OI_HPP

#include<iostream>
#include<cstdio>
#include<cstring>
#include<fstream>

template<typename TYPE>
class File {

    static const int maxn = 1e3 + 5;
private:
    char file[55];
    int rec[maxn], top;
    std :: fstream io;

public:
    File(const char *_file = "file.dat") {
        strcpy(file, _file);

        std :: fstream in (file, std :: ios :: in | std :: ios :: binary);

        if(!in.is_open()) {
            std :: fstream out (file, std :: ios :: out | std :: ios :: binary);
            TYPE init;
            out.write(reinterpret_cast<char *>(&init), sizeof(init));
            out.close();
        }
        else
            in.close();
        io.open(file, std :: ios :: in | std :: ios :: out | std :: ios ::binary);
    }
    ~File(){
        io.close();
    }

    template<class T>
    void Read(T &x, int pos) {
        io.seekg(pos, std :: ios :: beg);
        io.read(reinterpret_cast<char *>(&x), sizeof(x));
    }

    template<class T>
    void Write(T &x, int pos) {
        io.seekg(pos, std :: ios :: beg);
        io.write(reinterpret_cast<char *>(&x), sizeof(x));
    }

    int Newpos() {
        if(top)
            return rec[top --];
        io.seekg(0, std :: ios :: end);
        int pos = io.tellp();
        return pos;
    }

    void Delete(int pos) {
        rec[++ top] = pos;
    }
};


#endif //TICKETSYSTEM_2021_MAIN_FILE_OI_HPP
