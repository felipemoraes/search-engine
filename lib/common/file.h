//
//  file.h
//  search_engine
//
//  Created by Felipe Moraes on 4/01/15.
//
//

#ifndef __search_engine__file__
#define __search_engine__file__


#include <fstream>
#include <cstdio>
#include <string>
using namespace std;

template <class T>

class File {
private:
    int size_;
    string name_;
    FILE* file_;
    
public:
    class FileClosedException {};
    
    File(){
        
    }
    
    File(string file_name, bool trunc = true){
        open(file_name, trunc);
    }
    
    void open(string file_name, bool trunc = true){
        name_ = file_name;
        if(trunc){
            file_ = fopen(name_.c_str(), "w+b");
            size_ = 0;
        } else{
            file_ = fopen(name_.c_str(), "a+b");
            fseek(file_, 0, SEEK_END);
            int file_size = ftell(file_);
            size_ = file_size / sizeof(T);
            rewind();
        }
    }
    
    int write(T& oc){
        ensure_file_is_open();
        fwrite((char *) &oc, sizeof(T), 1, file_);
        size_++;
        return size_;
    }
    
    int write_block(T *oc, int block_size){
        ensure_file_is_open();
        fwrite((char *) oc, sizeof(T), block_size, file_);
        size_ += block_size;
        return size_;
    }
    
    T read(){
        ensure_file_is_open();
        T oc;
        fread((char*) &oc, sizeof(oc), 1, file_); // lê do arquivo
        return oc;
    }
    
    int read_block(T *oc, int block_size){
        ensure_file_is_open();
        size_t elements_read = fread((char*) oc, sizeof(T), block_size, file_); // lê do arquivo
        return elements_read;
    }
    
    void rewind(){
        ensure_file_is_open();
        fseek(file_, 0, SEEK_SET);
    }
    
    void ensure_file_is_open(){
        if( file_ == NULL){
            cout << "File " << name_ << " is not open!" << endl;
            throw FileClosedException();
        }
    }
    
    bool has_next(){
        ensure_file_is_open();
        if(get_position() >= size_){
            return false;
        } else {
            return true;
        }
    }
    
    int get_position(){
        ensure_file_is_open();
        int pointer = ftell(file_);
        int position = pointer/sizeof(T);
        if(feof(file_)){
            return position+1;
        }
        return position;
    }
    
    void set_position(int position){
        ensure_file_is_open();
        fseek(file_, position*sizeof(T), SEEK_SET);
    }
    
    int get_size(){
        ensure_file_is_open();
        return size_;
    }
    
    string get_name(){
        return name_;
    }
    
    void reopen(){
        close();
        open(name_, false);
    }
    
    void close(){
        if(file_ != NULL){
            fclose(file_);
            file_ = NULL;
        }
    }
    
    void delete_file(){
        close();
        remove(name_.c_str());
    }
    
    

};
#endif /* FILE_H_ */