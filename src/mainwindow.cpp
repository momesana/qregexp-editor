#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regexpmodel.h"
#include "aboutdialog.h"

// Qt
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_aboutDialog(0)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    // icons
    ui->quitAct->setIcon(QIcon::fromTheme("application-exit"));
    ui->clearInputEditAct->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/images/edit-clear.png")));
    ui->aboutAct->setIcon(QIcon::fromTheme("help-about"));

    // shortcuts
    ui->quitAct->setShortcut(QKeySequence::Quit);

    connect(ui->quitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->regexpLineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableWidgets()));
    connect(ui->regexpLineEdit, SIGNAL(returnPressed()), this, SLOT(evaluate()));
    connect(ui->clearRegExpEditAct, SIGNAL(triggered()), this, SLOT(clearRegExpEdit()));
    connect(ui->inputEdit, SIGNAL(textChanged()), this, SLOT(enableWidgets()));
    connect(ui->clearInputEditAct, SIGNAL(triggered()), this, SLOT(clearInputEdit()));

    // combobox
    QComboBox* cb = ui->patternSyntaxComboBox;
    cb->setItemData(0, 0, Qt::UserRole); // QRegExp::RegExp
    cb->setItemData(1, 3, Qt::UserRole); // QRegExp::RegExp2
    cb->setItemData(2, 1, Qt::UserRole); // QRegExp::Wildcard
    cb->setItemData(3, 4, Qt::UserRole); // QRegExp::WildcardUnix
    cb->setItemData(4, 2, Qt::UserRole); // QRegExp::FixedString
    cb->setItemData(5, 5, Qt::UserRole); // QRegExp::W3CXmlSchema11

    m_model = new RegExpModel(this);
    ui->resultView->setModel(m_model);
    connect(ui->evaluateAct, SIGNAL(triggered()), this, SLOT(evaluate()));
    connect(ui->evalButton, SIGNAL(released()), this, SLOT(evaluate()));
    connect(ui->aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    readSettings();
    enableWidgets();

    // statusbar
    m_statusLabel = new QLabel;
    statusBar()->addPermanentWidget(m_statusLabel);
    connect(m_model, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));
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
    writeSettings();
    e->accept();
}

void MainWindow::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("mainwindow/geometry").toByteArray());
    restoreState(s.value("mainwindow/state").toByteArray());
    const int index = s.value("mainwindow/patternsyntaxcombobox", 0).toInt();
    ui->patternSyntaxComboBox->setCurrentIndex(index);
    ui->caseSensitivityCheckBox->setChecked(s.value("mainwindow/casesensitivitycheckbox", true).toBool());
    ui->minimalCheckBox->setChecked(s.value("mainwindow/minimalcheckbox", false).toBool());

    QStringList widths = s.value("mainWindow/resultview").toString().split(' ');
    for (int i = 0; i < widths.count(); ++i)
        ui->resultView->setColumnWidth(i, qMax(widths.at(i).toInt(), 0));
}

void MainWindow::writeSettings()
{
    QSettings s;
    s.setValue("mainwindow/geometry", saveGeometry());
    s.setValue("mainwindow/state", saveState());
    s.setValue("mainwindow/patternsyntaxcombobox", ui->patternSyntaxComboBox->currentIndex());
    s.setValue("mainwindow/casesensitivitycheckbox", ui->caseSensitivityCheckBox->isChecked());
    s.setValue("mainwindow/minimalcheckbox", ui->minimalCheckBox->isChecked());

    QString widths;
    for (int i = 0; i < m_model->columnCount(QModelIndex()); ++i) {
        widths.append(QString::number(ui->resultView->columnWidth(i)));
        widths.append(' ');
    }
    s.setValue("mainWindow/resultview", widths);
}

void MainWindow::about()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        m_aboutDialog->setWindowTitle(tr("About ").arg(qApp->applicationName()));
    }

    m_aboutDialog->show();
    m_aboutDialog->raise();
    m_aboutDialog->activateWindow();
}

void MainWindow::evaluate()
{
    int index = ui->patternSyntaxComboBox->currentIndex();
    // we use the data as enum value for the pattern syntax of the regexp
    int data = ui->patternSyntaxComboBox->itemData(index, Qt::UserRole).toInt();
    QRegExp rx(ui->regexpLineEdit->text());
    rx.setPatternSyntax((QRegExp::PatternSyntax)data);
    rx.setMinimal(ui->minimalCheckBox->isChecked());
    rx.setCaseSensitivity(ui->caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    m_model->evaluate(ui->inputEdit->toPlainText(), rx);
}

void MainWindow::enableWidgets()
{
    bool b = ui->inputEdit->toPlainText().isEmpty() || ui->regexpLineEdit->text().isEmpty();
    ui->evalButton->setDisabled(b);
    ui->evaluateAct->setDisabled(b);
}

void MainWindow::clearInputEdit()
{
    ui->inputEdit->clear();
}

void MainWindow::clearRegExpEdit()
{
    ui->regexpLineEdit->clear();
}

void MainWindow::updateStatus(const QString& message)
{
    m_statusLabel->setText(message);
}

