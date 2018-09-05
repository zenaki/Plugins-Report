#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QTableWidget>
#include <QDir>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "mysql.h"

#include "qtrpt.h"

#include "xlsxdocument.h"
#include "xlsxcellformula.h"
//#include "xlsxformat.h"
//#include "xlsxcellrange.h"
//#include "xlsxworksheet.h"

//#include "3rdparty/QGumboParser/QGumboParser/qgumbodocument.h"
//#include "3rdparty/QGumboParser/QGumboParser/qgumbonode.h"

//#include <poppler/qt5/poppler-qt5.h>
//#include <QRectF>
//#include <QDebug>
//#include <QFile>
//#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QtRPT *report;
    QXlsx::Document *xlsx;
    QStringList dataName;
    QStringList dataValue;
    int max_row_count;

    QString generateReport_pdf(QString template_path, QString config_path, QString output_name, QString parameter);
    QString generateReport_excel(QString template_path, QString config_path, QString output_name, QString parameter);
    QStringList readParam(QString config_path, QString parameter);

    QSqlDatabase db;
    mysql database;

//signals:
//    void close();

public slots:
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage);

private:
//    Ui::MainWindow *ui;

    QStringList m_TableHeader;
};

#endif // MAINWINDOW_H
