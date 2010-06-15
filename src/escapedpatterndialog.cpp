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

#include "escapedpatterndialog.h"
#include "ui_escapedpatterndialog.h"

EscapedPatternDialog::EscapedPatternDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EscapedPatternDialog)
{
    ui->setupUi(this);
}

EscapedPatternDialog::~EscapedPatternDialog()
{
    delete ui;
}

void EscapedPatternDialog::setPattern(const QString& pattern)
{
    QString str(pattern);
    str.replace('\\', "\\\\");

    QString html = tr(
            "<center><div style='background: lightgray; font-weight: bold;'>Rationale</div></center>"
            "<p>As the QRegexp documentation says:  The C++ compiler transforms "
            "backslashes in strings. To include a \\ in a regexp, enter it "
            "twice, i.e. \\\\.</p>"
            "<center><div style='background: lightgray; font-weight: bold;'>Escaped pattern</div></center>"
            "<p><code style='color: darkGreen'>%1</code></p>").arg(str);

    ui->textBrowser->setHtml(html);
}

void EscapedPatternDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
