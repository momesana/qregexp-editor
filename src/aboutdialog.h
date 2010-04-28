#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, public Ui::AboutDialog
{
  Q_OBJECT
public:
  AboutDialog(QWidget* parent = 0);
  ~AboutDialog();

protected:
  virtual void changeEvent(QEvent*);
  virtual void closeEvent(QCloseEvent*);

private:
    void readSettings();
    void writeSettings();

};

#endif // ABOUTDIALOG_H
