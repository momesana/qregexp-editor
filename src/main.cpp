// Qt
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

#include "mainwindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("momesana software inc.");
  QCoreApplication::setOrganizationDomain("momesana.com");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setApplicationName("QRegExp Editor");

  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  app.installTranslator(&qtTranslator);

  QTranslator appTranslator;
  appTranslator.load("translations/regexpeditor_" + QLocale::system().name());
  app.installTranslator(&appTranslator);

  MainWindow mw;
  mw.show();
  return app.exec();
}

#include "main.moc"
