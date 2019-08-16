#include "pack.h"

pack::pack()
{
    static AppContext appCtx(_T("pack"));
    AppContext::setInstance(&appCtx);
    LibManager::getInstance()->initLibs(AppSettings::getCFLAppSettings(),
                                        &appCtx, 0);
}

void pack::getname(const String& SrcPath)
{
    srcPath = SrcPath;
    vector<SystemFile*> sysFileSub;
    queue<String>       pathQueue;
    String              pattern = _T("*");
    pathQueue.push(SrcPath);
    while (true != pathQueue.empty()) {
        Directory           dir(pathQueue.front());
        vector<SystemFile*> sysFileSub;
        if (dir.exists()) {
            dir.list(sysFileSub, pattern);
            for (auto it = sysFileSub.begin(); it != sysFileSub.end(); ++it) {
                FileInfo _fileInfo;
                _fileInfo.name = (*it)->getName();
                _fileInfo.path = (*it)->getFullName();
                if (true == (*it)->isDirectory((*it)->getFullName())) {
                    pathQueue.push((*it)->getFullName());
                }
                else {
                    File _file((*it)->getFullName());
                    _fileInfo.type = _FILE_;
                    _fileInfo.size = _file.getLength();
                }
                fileInfo.push_back(_fileInfo);
            }
        }
        pathQueue.pop();
    }
}

void pack::getpack(const String& PackPath)
{
    Directory        dir(srcPath);
    String           packFilePath = PackPath + dir.getName() + _T(".pack");
    FileOutputStream pack(packFilePath, OPEN_OR_CREATE_MODE);
    DataOutputStream out(&pack);
    for (auto it = fileInfo.begin(); it != fileInfo.end(); ++it) {
        if (_FILE_ == (*it).type) {
            FileInputStream inputFile((*it).path);
            (*it).path.remove(0, srcPath.getLength());
            (*it).serialize(&out);
            unsigned char buf[_BUFFERSIZE_] = {0};
            while (1) {
                int ret = inputFile.read(buf, _BUFFERSIZE_);
                if (0 == ret) {
                    break;
                }
                // printf_s("size = %zd | ret = %d\n", (*it).size, ret);
                out.write(buf, ret);
            }
            inputFile.close();
        }
        else if (_DIR_ == (*it).type) {
            (*it).path.remove(0, srcPath.getLength());
            (*it).serialize(&out);
        }
    }
    out.close();
}
void pack::depack(const String& PackPath, const String& DepackPath)
{
    FileInputStream pack(PackPath);
    DataInputStream in(&pack);
    while (1) {
        if (0 == in.available()) {
            break;
        }
        FileInfo fInfo;
        fInfo.deserialize(&in);
        if (_FILE_ == fInfo.type) {
            String           _path = DepackPath + fInfo.path;
            FileOutputStream out(_path, OPEN_OR_CREATE_MODE);
            unsigned char    buf[_BUFFERSIZE_] = {0};
            int              wsize             = (int)fInfo.size;
            while (wsize > 0) {
                int i = (wsize > _BUFFERSIZE_) ? _BUFFERSIZE_ : wsize;
                in.read(buf, i);
                out.write(buf, i);
                wsize = wsize - _BUFFERSIZE_;
            }
            out.close();
        }
        else if (_DIR_ == fInfo.type) {
            String    _path = DepackPath + fInfo.path;
            Directory dir(_path);
            dir.create();
        }
    }
    in.close();
}
void pack::showqueue()
{
    for (auto it = fileInfo.begin(); it != fileInfo.end(); ++it) {
        printMessage((*it).path);
    }
}
pack::~pack()
{
    LibManager::getInstance()->closeLibs(0);
}