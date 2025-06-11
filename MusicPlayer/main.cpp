#include "MusicPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicPlayer w;
    w.show();
    //调用resizeEvent
    w.resize(w.size()+QSize(1,1) );
    return a.exec();
}
