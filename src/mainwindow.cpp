/*
 * Copyright (C) 2010 M. Mehdi Salem Naraghi <momesana@yahoo.de>
 *
 * This file is part of QRegexp-Editor.
 *
 * Regexp-Editor is free software: you can redistribute it and/or modify
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "regexpmodel.h"
#include "aboutdialog.h"
#include "escapedpatterndialog.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_aboutDialog(0)
    , m_escapedPatternDialog(0)
    , m_maxRecentFiles(10)
    , m_settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("QRegExp-Editor"), QLatin1String("QRegExp-Editor"))
    , m_searchSettings()
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    setIcons();
    populateComboBoxes();

    // shortcuts
    ui->quitAct->setShortcut(QKeySequence::Quit);

    makeSignalConnections(); // UI widgets (exception: recent files)
    createRegExpModel();
    createRecentFileActions();
    createStatusBar();

    readSettings();
    updateRecentFileActions();
    updateUiStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    m_searchSettings.append(searchSettings());
    writeSettings();
    e->accept();
}

static const char groupM[] = "mainwindow";
static const char geometryKeyC[] = "geometry";
static const char stateKeyC[] = "state";
static const char recentFilesKeyC[] = "recentfiles";
static const char resultViewWidthsKeyC[] = "resultview";

void MainWindow::readSettings()
{
    m_settings.beginGroup(QLatin1String(groupM));
    restoreGeometry(m_settings.value(
                        QLatin1String(geometryKeyC)).toByteArray());
    restoreState(m_settings.value(QLatin1String(stateKeyC)).toByteArray());
    m_recentFiles = m_settings.value(QLatin1String(recentFilesKeyC),
                                     QStringList()).toStringList();
    QStringList widths = m_settings.value(QLatin1String(
            resultViewWidthsKeyC)).toString().split(' ');
    for (int i = 0; i < widths.count(); ++i) {
        ui->resultView->setColumnWidth(i, qMax(widths.at(i).toInt(), 0));
    }

    m_settings.endGroup();

    m_searchSettings.fromSettings(&m_settings);
    m_searchSettings.setHistoryLength(1);
    setSearchSettings(&m_searchSettings);
}

void MainWindow::writeSettings()
{
    m_settings.beginGroup(QLatin1String(groupM));
    m_settings.setValue(QLatin1String(geometryKeyC), saveGeometry());
    m_settings.setValue(QLatin1String(stateKeyC), saveState());
    m_settings.setValue(QLatin1String(recentFilesKeyC), m_recentFiles);

    QString widths;
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i) {
        widths.append(QString::number(ui->resultView->columnWidth(i)));
        widths.append(' ');
    }
    m_settings.setValue(QLatin1String(resultViewWidthsKeyC), widths);
    m_settings.endGroup();
    m_searchSettings.toSettings(&m_settings);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::openRecentFile()
{
    QAction *act = qobject_cast<QAction *>(sender());
    this->loadFile(act->data().toString());
}

void MainWindow::clearAllRecentFiles()
{
    m_recentFiles.clear();
    updateRecentFileActions();
}


void MainWindow::about()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        m_aboutDialog->setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    }

    m_aboutDialog->show();
    m_aboutDialog->raise();
    m_aboutDialog->activateWindow();
}

void MainWindow::escapedPattern()
{
    if (!m_escapedPatternDialog) {
        m_escapedPatternDialog = new EscapedPatternDialog(this);
        m_escapedPatternDialog->setWindowTitle(tr("Escaped Pattern"));
    }

    m_escapedPatternDialog->setPattern(ui->regexpEdit->toPlainText());
    m_escapedPatternDialog->show();
    m_escapedPatternDialog->raise();
    m_escapedPatternDialog->activateWindow();
}

void MainWindow::returnPressed()
{
    if (isSearchPossible()) {
        search();
    }
}

void MainWindow::search()
{
    m_model->evaluate(ui->inputEdit->toPlainText(), m_rx);

    // Resize the columns to it's contents
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i) {
        ui->resultView->resizeColumnToContents(i);
    }
}

void MainWindow::updateUiStatus()
{
    bool b = isSearchPossible();
    ui->escapedPatternAct->setEnabled(m_rx.isValid() && !ui->regexpEdit->toPlainText().isEmpty());

    if (m_rx.isValid()) {
        ui->regexpEdit->setStyleSheet("");
        statusBar()->showMessage(tr("Valid expression"));
    } else {
        ui->regexpEdit->setStyleSheet("QPlainTextEdit { background: #F7E7E7; }");
        statusBar()->showMessage(
            tr("Invalid expression: %1").arg(m_rx.errorString()));
    }

    ui->searchButton->setEnabled(b);
    ui->searchAct->setEnabled(b);
}

void MainWindow::updateRegExp()
{
    int index = ui->syntaxComboBox->currentIndex();
    // we use the data as enum value for the pattern syntax of the regexp
    int data = ui->syntaxComboBox->itemData(index, Qt::UserRole).toInt();
    m_rx.setPattern(ui->regexpEdit->toPlainText());
    m_rx.setPatternSyntax((QRegExp::PatternSyntax)data);
    m_rx.setMinimal(ui->minimalCheckBox->isChecked());
    m_rx.setCaseSensitivity(ui->caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    updateUiStatus();
}

void MainWindow::clearInputEdit()
{
    ui->inputEdit->clear();
}

void MainWindow::clearRegExpEdit()
{
    ui->regexpEdit->clear();
}

void MainWindow::updateStatus(const QString &message)
{
    m_statusLabel->setText(message);
}

void MainWindow::toggleWarningWidget(bool on)
{
    ui->warningWidget->setVisible(on);
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error opening file"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    m_recentFiles.removeAll(fileName);
    m_recentFiles.prepend(fileName);
    updateRecentFileActions();
    QTextStream inFile(&file);
    ui->inputEdit->setPlainText(inFile.readAll());
    return true;
}

void MainWindow::updateRecentFileActions()
{
    ui->recentFilesMenu->setEnabled(m_recentFiles.size());
    QMutableStringListIterator i(m_recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next())) {
            i.remove();
        }
    }

    QFontMetrics fm = this->fontMetrics();
    for (int i = 0; i < m_recentFileActions.size(); ++i) {
        if (i >= m_maxRecentFiles || i >= m_recentFiles.size()) {
            m_recentFileActions[i]->setVisible(false);
        } else {
            const QString path = m_recentFiles.at(i);
            QAction *act = m_recentFileActions[i];
            act->setText(QString("%1 [%2]")
                         .arg(QFileInfo(path).fileName())
                         .arg(fm.elidedText(path, Qt::ElideMiddle, width() / 2))); // TODO find more intelligent way to calculate the avail. width
            act->setData(path);
            act->setVisible(true);
        }
    }
}

void MainWindow::populateComboBoxes()
{
    ui->syntaxComboBox->addItem(tr("RegExp"), QRegExp::RegExp);
    ui->syntaxComboBox->addItem(tr("RegExp2"), QRegExp::RegExp2);
    ui->syntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
    ui->syntaxComboBox->addItem(tr("WildcardUnix"), QRegExp::WildcardUnix);
    ui->syntaxComboBox->addItem(tr("FixedString"), QRegExp::FixedString);
    ui->syntaxComboBox->addItem(tr("W3CXmlSchema11"), QRegExp::W3CXmlSchema11);
}

void MainWindow::setIcons()
{
    ui->openAct->setIcon(QIcon::fromTheme("document-open", QIcon(":/images/document-open.png")));
    ui->quitAct->setIcon(QIcon::fromTheme("application-exit", QIcon(":/images/application-exit.png")));
    ui->clearInputEditAct->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/images/edit-clear.png")));
    ui->aboutAct->setIcon(QIcon::fromTheme("help-about"));
    QIcon warningIcon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
    warningIcon = QIcon::fromTheme("dialog-warning", warningIcon);
    ui->emptyStringMatchedIconLabel->setPixmap(warningIcon.pixmap(32));
}

void MainWindow::makeSignalConnections()
{
    connect(ui->openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->quitAct, SIGNAL(triggered()), SLOT(close()));
    connect(ui->regexpEdit, SIGNAL(textChanged()), SLOT(updateRegExp()));
    connect(ui->clearRegExpEditAct, SIGNAL(triggered()), SLOT(clearRegExpEdit()));
    connect(ui->inputEdit, SIGNAL(textChanged()), SLOT(updateUiStatus()));
    connect(ui->clearInputEditAct, SIGNAL(triggered()), SLOT(clearInputEdit()));
    connect(ui->syntaxComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateRegExp()));
    connect(ui->caseSensitivityCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExp()));
    connect(ui->minimalCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExp()));
    connect(ui->searchAct, SIGNAL(triggered()), SLOT(search()));
    connect(ui->searchButton, SIGNAL(released()), SLOT(search()));
    connect(ui->aboutAct, SIGNAL(triggered()), SLOT(about()));
    connect(ui->escapedPatternAct, SIGNAL(triggered()), SLOT(escapedPattern()));
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel;
    statusBar()->addPermanentWidget(m_statusLabel);
    connect(m_model, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));
    ui->warningWidget->hide();
}

void MainWindow::createRegExpModel()
{
    m_model = new RegExpModel(this);
    ui->resultView->setModel(m_model);
    connect(m_model, SIGNAL(emptyStringMatched(bool)), SLOT(toggleWarningWidget(bool)));
}

void MainWindow::createRecentFileActions()
{
    for (int i = 0; i < m_maxRecentFiles; ++i) {
        QAction *act = new QAction(this);
        connect(act, SIGNAL(triggered()), SLOT(openRecentFile()));
        m_recentFileActions << act;
        act->setVisible(false);
    }
    QAction *separator = ui->recentFilesMenu->addSeparator();
    ui->recentFilesMenu->addAction(tr("C&lear all"), this, SLOT(clearAllRecentFiles()));
    ui->recentFilesMenu->insertActions(separator, m_recentFileActions);
}

bool MainWindow::isSearchPossible()
{
    bool b = ui->inputEdit->toPlainText().isEmpty() ||
             ui->regexpEdit->toPlainText().isEmpty() ||
             !m_rx.isValid() ? false : true;
    return b;
}

SearchData MainWindow::searchSettings() const
{
    SearchData rc;
    rc.pattern = ui->regexpEdit->toPlainText();
    rc.syntax = ui->syntaxComboBox->currentIndex();
    rc.caseSensitivity = ui->caseSensitivityCheckBox->isChecked();
    rc.minimal = ui->minimalCheckBox->isChecked();
    return rc;
}

void MainWindow::setSearchSettings(SearchSettings *s) const
{
    QList<SearchData>searches = s->searchData();
    if (!searches.isEmpty()) {
        SearchData rc = searches.last();
        ui->regexpEdit->setPlainText(rc.pattern);
        ui->syntaxComboBox->setCurrentIndex(rc.syntax);
        ui->caseSensitivityCheckBox->setChecked(rc.caseSensitivity);
        ui->minimalCheckBox->setChecked(rc.minimal);
    }
}
