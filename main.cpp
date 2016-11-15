#include "mainwidget.h"
#include "world.h"
#include "highscore.h"
#include <QApplication>
#include <object.h>

void UnitTest() {
    Object::unitTest();
	PlayingScreen::unitTest();
    //HighScore::UnitTests();
    Enemy::unitTest();
    qDebug() << "Passed all unit tests.";
}

int main(int argc, char *argv[])
{
    vector<string> args(argv, argv + argc);

    if (argc == 2){
        if (args.at(1) == "-test"){
			UnitTest();
        }
    }

    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}
