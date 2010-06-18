/*
 * Copyright (C) 2010 M. Mehdi Salem Naraghi <momesana@yahoo.de>
 *
 * This file is part of Regexp-Editor.
 *
 * Regexp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

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
  QCoreApplication::setApplicationVersion("0.2.0.1");
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

