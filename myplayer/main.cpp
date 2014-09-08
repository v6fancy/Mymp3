#include <QtGui/QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    //设置无主窗口无边框

//qInstallMsgHandler(customMessageHandler);

    Widget w;
    //w.setWindowOpacity(1);
    //w.setWindowFlags(Qt::FramelessWindowHint);
    //w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();

    return a.exec();
}
