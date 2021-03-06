#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTranslator;
   if (qtTranslator.load(QLocale::system(),
               "qt", "_",
               QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
   {
       a.installTranslator(&qtTranslator);
   }

   QTranslator qtBaseTranslator;
   if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
               QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
   {
       a.installTranslator(&qtBaseTranslator);
   }

    MainWindow w;
    w.show();
    return a.exec();
}
