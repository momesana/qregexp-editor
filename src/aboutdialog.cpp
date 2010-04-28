#include "aboutdialog.h"
#include <QFile>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
  setupUi(this);

  versionLabel->setText(QString("<div>"
                        "<span style='font-weight: bold; font-size:14pt;'>%1 %2</span>"
                        "<span style='font-weight: bold; font-size: 10pt;'> &copy; 2010</span> <br />"
                        "<span style='font-size: 8pt;'>Mohammad Mehdi Salem Naraghi<span><div>")
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
