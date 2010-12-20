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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class AboutDialog;
class EscapedPatternDialog;
class RegExpModel;
class QAction;
class QLabel;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    RegExpModel *m_model;
    AboutDialog *m_aboutDialog;
    EscapedPatternDialog *m_escapedPatternDialog;
    QLabel *m_statusLabel;
    QRegExp m_rx;
    QStringList m_recentFiles;
    QList<QAction*> m_recentFileActions;
    const int m_maxRecentFiles;

    bool loadFile(const QString &filename);
    void updateRecentFileActions();
    void writeSettings();
    void readSettings();
    void populateComboBoxes();
    void setIcons();
    void makeSignalConnections();
    void createStatusBar();
    void createRegExpModel();
    void createRecentFileActions(); 
    bool isSearchPossible();

private slots:
    void open();
    void openRecentFile();
    void clearAllRecentFiles();
    void search();
    void about();
    void escapedPattern();
    void updateUiStatus();
    void updateRegExp();
    void clearInputEdit();
    void clearRegExpEdit();
    void updateStatus(const QString&);
    void toggleWarningWidget(bool);
};

#endif // MAINWINDOW_H
