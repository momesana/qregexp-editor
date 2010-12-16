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

#include "aboutdialog.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include <QtGui/QCloseEvent>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
  setupUi(this);

  versionLabel->setText(QString("<div>"
                        "<span style='font-weight: bold; font-size:14pt;'>%1 %2</span> <br />"
                        "<span style='font-size: 8pt;'> Copyright &copy; 2010 M. Mehdi Salem Naraghi<span><div>")
                        .arg(qApp->applicationName())
                        .arg(qApp->applicationVersion()));

  QHash<QTextBrowser*, QString> hash;
  hash[aboutBrowser] = ":ABOUT.html";
  hash[authorsBrowser] = ":AUTHORS.html";
  hash[licenseBrowser] = ":COPYING.html";

  QHash<QTextBrowser*, QString>::iterator iter;
  for (iter = hash.begin(); iter != hash.end(); ++iter) {
      QFile f(iter.value());
      if (f.open(QIODevice::ReadOnly | QIODevice::Text))
          iter.key()->setHtml(f.readAll());
  }

  readSettings();
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::changeEvent(QEvent* e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
  }
}

void AboutDialog::closeEvent(QCloseEvent* e)
{
    writeSettings();
    e->accept();
}

void AboutDialog::readSettings()
{
    QSettings settings;
    QRect geometry = settings.value("aboutdialog/geometry", QRect()).toRect();
    if (!geometry.isNull())
      resize(geometry.width(), geometry.height());
}

void AboutDialog::writeSettings()
{
    QSettings settings;
    settings.setValue("aboutdialog/geometry", geometry());
}
