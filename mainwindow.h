#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDir>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "qtrpt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QtRPT *report;
    QStringList dataName;
    QStringList dataValue;
    int max_row_count;

    void generateReport(QString template_path, QString config_path);
    void readParam(QString config_path);

signals:
    void close();

public slots:
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage);

private:
    Ui::MainWindow *ui;

    QStringList m_TableHeader;
};

#endif // MAINWINDOW_H
