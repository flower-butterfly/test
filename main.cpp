#include "mainwindow.h"
#include <QApplication>
#include <qdatetime.h>
#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QDir>
#include "videoplayer.h"
#include "receivetest.h"

int main(int argc, char *argv[])
{
    //每个程序都必须有且只有一个QApplication对象、采用
    //argc和argv作为参数、以便对命令行参数进行处理
    QApplication app(argc, argv);
    app.setApplicationName("Ref Video Widget Example");
    app.setOrganizationName("QtProject");
    app.setApplicationDisplayName(QCoreApplication::applicationName());
    app.setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt Video Widget Example");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    MainWindow mainWin;

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWin);
    mainWin.resize(availableGeometry.width() , availableGeometry.height() );
    mainWin.show();
    //程序进入消息循环，等待可能输入进行相应。Qt完成事件处
    //理及显示的工作，并在应用程序退出是返回exec()的值。
    return app.exec();

}
