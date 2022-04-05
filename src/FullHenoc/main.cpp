#include <QApplication>
#include "CfrmHenoc.h"

int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(ftp);


    QApplication app(argc, argv);
    CfrmHenoc UnWin;
    UnWin.show();
    return app.exec();
}
