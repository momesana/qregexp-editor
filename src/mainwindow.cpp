#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regexpmodel.h"
#include "aboutdialog.h"

// Qt
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
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
    ui->openAct->setIcon(QIcon::fromTheme("document-open", QIcon(":/images/document-open.png")));
    ui->quitAct->setIcon(QIcon::fromTheme("application-exit"));
    ui->clearInputEditAct->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/images/edit-clear.png")));
    ui->aboutAct->setIcon(QIcon::fromTheme("help-about"));
    QIcon warningIcon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
    warningIcon = QIcon::fromTheme("dialog-warning", warningIcon);
    ui->emptyStringMatchedIconLabel->setPixmap(warningIcon.pixmap(32));

    // shortcuts
    ui->quitAct->setShortcut(QKeySequence::Quit);

    connect(ui->openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->quitAct, SIGNAL(triggered()), SLOT(close()));
    connect(ui->regexpLineEdit, SIGNAL(textChanged(QString)), SLOT(updateRegExp()));
    connect(ui->regexpLineEdit, SIGNAL(returnPressed()), SLOT(evaluate()));
    connect(ui->clearRegExpEditAct, SIGNAL(triggered()), SLOT(clearRegExpEdit()));
    connect(ui->inputEdit, SIGNAL(textChanged()), SLOT(enableEvaluation()));
    connect(ui->clearInputEditAct, SIGNAL(triggered()), SLOT(clearInputEdit()));
    connect(ui->patternSyntaxComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateRegExp()));
    connect(ui->caseSensitivityCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExp()));
    connect(ui->minimalCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExp()));
    connect(ui->evaluateAct, SIGNAL(triggered()), SLOT(evaluate()));
    connect(ui->evalButton, SIGNAL(released()), SLOT(evaluate()));
    connect(ui->aboutAct, SIGNAL(triggered()), SLOT(about()));

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
    connect(m_model, SIGNAL(emptyStringMatched(bool)), SLOT(toggleWarningWidget(bool)));
    readSettings();
    enableEvaluation();

    // statusbar
    m_statusLabel = new QLabel;
    statusBar()->addPermanentWidget(m_statusLabel);
    connect(m_model, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));
    ui->warningWidget->hide();
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
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i) {
        widths.append(QString::number(ui->resultView->columnWidth(i)));
        widths.append(' ');
    }
    s.setValue("mainWindow/resultview", widths);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
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
    m_model->evaluate(ui->inputEdit->toPlainText(), m_rx);

    // Resize the columns to it's contents
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i)
        ui->resultView->resizeColumnToContents(i);
}

void MainWindow::enableEvaluation()
{
    bool b = ui->inputEdit->toPlainText().isEmpty() ||
             ui->regexpLineEdit->text().isEmpty() ||
             !m_rx.isValid() ? false : true;

    if (m_rx.isValid()) {
        ui->regexpLineEdit->setStyleSheet("");
        statusBar()->showMessage(tr("Valid expression"));
    }
    else {
        ui->regexpLineEdit->setStyleSheet("QLineEdit { background: #FFBFBF; }");
        statusBar()->showMessage(tr("Invalid expression"));
    }

    ui->evalButton->setEnabled(b);
    ui->evaluateAct->setEnabled(b);
}

void MainWindow::updateRegExp()
{
    int index = ui->patternSyntaxComboBox->currentIndex();
    // we use the data as enum value for the pattern syntax of the regexp
    int data = ui->patternSyntaxComboBox->itemData(index, Qt::UserRole).toInt();
    m_rx.setPattern(ui->regexpLineEdit->text());
    m_rx.setPatternSyntax((QRegExp::PatternSyntax)data);
    m_rx.setMinimal(ui->minimalCheckBox->isChecked());
    m_rx.setCaseSensitivity(ui->caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    enableEvaluation();
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
    QTextStream inFile(&file);
    ui->inputEdit->setPlainText(inFile.readAll());
    return true;
}
