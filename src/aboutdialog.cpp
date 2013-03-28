/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
 * Copyright (C) 2010-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegExp-Editor.
 *
 * QRegExp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aboutdialog.h"

#include <QFile>
#include <QSettings>
#include <QDebug>

#include <QtGui/QCloseEvent>

static const char geometryKey[] = "geometry";
static const char group[] = "aboutdialog";

AboutDialog::AboutDialog(QSettings *settings, QWidget *parent)
    : QDialog(parent),
      m_settings(settings)
{
    setupUi(this);

    versionLabel->setText(QString::fromUtf8("<div>"
                                  "<span style='font-weight: bold; font-size:14pt;'>%1 %2</span> <br />"
                                  "<span style='font-size: 8pt;'> Copyright &copy; 2010-2012 M. Mehdi Salem Naraghi<span><div>"
                                  "<span style='font-size: 8pt;'> Copyright &copy; 2010-2012 Luís Pereira<span><div>")
                          .arg(qApp->applicationName())
                          .arg(qApp->applicationVersion()));

    QHash<QTextBrowser *, QString> hash;
    hash[aboutBrowser] = ":ABOUT.html";
    hash[authorsBrowser] = ":AUTHORS.html";
    hash[licenseBrowser] = ":COPYING.html";

    QHash<QTextBrowser *, QString>::iterator iter;
    for (iter = hash.begin(); iter != hash.end(); ++iter) {
        QFile f(iter.value());
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&f);
            in.setCodec("UTF-8");
            iter.key()->setHtml(in.readAll());
        }
    }

    readSettings();
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::changeEvent(QEvent *e)
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

void AboutDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

void AboutDialog::readSettings()
{
    QSettings *s = m_settings.data();
    if (s) {
        s->beginGroup(QLatin1String(group));
        QRect geometry = s->value((QLatin1String(geometryKey)), QRect()).toRect();
        if (!geometry.isNull()) {
            resize(geometry.width(), geometry.height());
        }
        s->endGroup();
    } else {
        qWarning() << Q_FUNC_INFO << "null settings pointer";
    }
}

void AboutDialog::writeSettings()
{
    QSettings *s = m_settings.data();
    if (s) {
        s->beginGroup(QLatin1String(group));
        s->setValue((QLatin1String(geometryKey)), geometry());
        s->endGroup();
    } else {
        qWarning() << Q_FUNC_INFO << "null settings pointer";
    }
}
