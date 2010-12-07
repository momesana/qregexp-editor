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

#ifndef ESCAPEDPATTERNDIALOG_H
#define ESCAPEDPATTERNDIALOG_H

#include <QDialog>

namespace Ui {
    class EscapedPatternDialog;
}

class EscapedPatternDialog : public QDialog {
    Q_OBJECT
public:
    EscapedPatternDialog(QWidget *parent = 0);
    ~EscapedPatternDialog();
    void setPattern(const QString& pattern);

protected:
    void changeEvent(QEvent *e);

private:
    QString escapePattern(const QString& pattern);
    Ui::EscapedPatternDialog *ui;
};

#endif // ESCAPEDPATTERNDIALOG_H
