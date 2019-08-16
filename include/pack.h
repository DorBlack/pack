#pragma once
#include <abprec.h>
#include <cflprec.h>
#include <io/io.h>
#include <iostream>
#include <lang/string.h>
#define _BUFFERSIZE_ 1024
using namespace cpp::lang;

enum FileType {
    _DIR_  = 'D',
    _FILE_ = 'F',
};
struct FileInfo
{
    FileInfo() : name(String::EMPTY), path(String::EMPTY), type(_DIR_), size(0)
    {
    }
    void serialize(DataOutputStream* out)
    {
        out->writeString(name);
        out->writeString(path);
        out->writeChar(type);
        out->writeInt64(size);
    }
    void deserialize(DataInputStream* in)
    {
        name = in->readString();
        path = in->readString();
        type = in->readChar();
        size = in->readInt64();
    }
    //文件或目录名
    String name;
    //文件的相对路径
    String path;
    //文件的类型
    char type;
    //文件的大小
    int64 size;
};

class pack {
  public:
    pack();
    /**
     * 获取当前的目录下的所有文件及目录的信息
     */
    void getname(const String& SrcPath);
    /**
     * 把获取的文件信息内容及目录信息，写入一个文件
     */
    void getpack(const String& PackPath);
    /**
     * 把文件还原成原来的目录结构
     */
    void depack(const String& PackPath, const String& DepackPath);
    /**
     * 展示文件的路径名
     */
    void showqueue();
    ~pack();

  private:
    vector<FileInfo> fileInfo;
    String           srcPath;
};