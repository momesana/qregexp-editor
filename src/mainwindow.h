#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class AboutDialog;
class RegExpModel;
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
    QLabel* m_statusLabel;

    bool loadFile(const QString &filename);

    void writeSettings();
    void readSettings();

private slots:
    void open();
    void evaluate();
    void about();
    void enableWidgets();
    void clearInputEdit();
    void clearRegExpEdit();
    void updateStatus(const QString&);
    void toggleWarningWidget(bool);
};

#endif // MAINWINDOW_H
