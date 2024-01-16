#include <cstdio>
#include "ic_manager.h"
#include <string.h>


int main()
{
//    Logger::Instance().add(std::make_shared<ConsoleChannel>());
//    Logger::Instance().add(std::make_shared<FileChannel>());
//    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

    //InfoL << "Version : " << CURRENTVERSION;
    //InfoL << "v : Version check";

    ICManager icMgr;
    char pStr[100];
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}

//local file mode
/*
int main(int argc, char* argv[]) {
    if (argc < 2 ) {
        printf("Please input VIDEO JSON FILE");
        return 0;
    }

    string jsonfile(argv[1]);
    cout << jsonfile;
    ExpUtil in;
    VIDEO_INFO info;
    int result  = in.ImportVideoInfo(jsonfile, &info, true);
    if(result == dove::ERR_NONE) {
        Dove stblz(&info);
        result = stblz.Process();
    }

    return 1;
}
*/
