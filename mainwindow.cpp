#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateReport(QString template_path, QString config_path, QString output_name, QString parameter)
{
    QString fileName = template_path;
    report = new QtRPT(this);
//    report->setBackgroundImage(QPixmap(dir.absolutePath()+"/examples_report/qt_background_portrait.png"));
    this->readParam(config_path, parameter);
    report->recordCount << max_row_count;
    report->recordCount << max_row_count;
    if (report->loadReport(fileName) == false) {
        printf("{\"ERR\": \"Report file not found\"}\n\n");
        emit close();
    }
    QObject::connect(report, SIGNAL(setValue(const int, const QString, QVariant&, const int)),
                     this, SLOT(setValue(const int, const QString, QVariant&, const int)));
    QObject::connect(report, SIGNAL(setValueImage(const int, const QString, QImage&, const int)),
                     this, SLOT(setValueImage(const int, const QString, QImage&, const int)));
//    qDebug() << output_name;
    report->printPDF(output_name, false);
//    report->printExec();
    printf("{\"SUCCESS\": \"%s\"}\n\n", output_name.toUtf8().data());
}

void MainWindow::readParam(QString config_path, QString parameter)
{
    max_row_count = 0;
    QFile loadFile(config_path);
    if (loadFile.open(QIODevice::ReadWrite)) {
        QByteArray readFile = loadFile.readAll();

        QJsonDocument JsonDoc = QJsonDocument::fromJson(readFile);
        QJsonObject object = JsonDoc.object();
        if (object.value("report_param").isArray()) {
            foreach (const QJsonValue & v,  object.value("report_param").toArray()) {
                if (!v.toObject().value("value").toObject().value("query").isUndefined()) {
                    QStringList result = database.read(db, v.toObject().value("value").toObject().value("query").toString(), parameter.split("#"));
                    int row_count = 0;
                    QString data;
                    if (result.length() > 1) {
                        for (int i = 0; i < result.length(); i++) {
                            if (data.isEmpty()) {
                                data = result.at(i);
                            } else {
                                data = data + ";" + result.at(i);
                            }
                            row_count++;
                        }
                        dataValue.append(data);
                        dataName.append(v.toObject().value("name").toString() + ";" +
                                        QString::number(row_count) + ";0");
                        m_TableHeader.append(v.toObject().value("name").toString());
                        max_row_count = max_row_count >= row_count ? max_row_count : row_count;
                    } else {
                        row_count = 1;
                        dataValue.append(result.at(0));
                        dataName.append(v.toObject().value("name").toString() + ";1;0");
                        max_row_count = max_row_count >= row_count ? max_row_count : row_count;
                    }
                } else if (v.toObject().value("value").isArray()) {
                    int row_count = 0;
                    QString data;
                    foreach (const QJsonValue & v2, v.toObject().value("value").toArray()) {
                        if (data.isEmpty()) {
                            data = v2.toString();
                        } else {
                            data = data + ";" + v2.toString();
                        }
                        row_count++;
                    }
                    dataValue.append(data);
                    dataName.append(v.toObject().value("name").toString() + ";" +
                                    QString::number(row_count) + ";0");
                    m_TableHeader.append(v.toObject().value("name").toString());
                    max_row_count = max_row_count >= row_count ? max_row_count : row_count;
                } else {
                    int row_count = 1;
                    dataValue.append(v.toObject().value("value").toString());
                    dataName.append(v.toObject().value("name").toString() + ";1;0");
                    max_row_count = max_row_count >= row_count ? max_row_count : row_count;
                }
            }
        }
    }
}

void MainWindow::setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage)
{
    Q_UNUSED(reportPage);
//    qDebug() << "From setValue" << paramName;
    bool ok = false;
    for (int i = 0; i < dataName.length(); i++) {
        QStringList nameList = dataName.at(i).split(";");
        if (paramName == nameList.at(0)) {
            if (nameList.at(1) != nameList.at(2)) {
                int row_count = nameList.at(2).toInt();
                for (int j = 0; j < m_TableHeader.length(); j++) {
                    if (paramName == m_TableHeader.at(j)) {
                        QStringList valueList = dataValue.at(i).split(";");
                        if (recNo <= valueList.length()-1) {
                            paramValue = valueList.at(recNo);
                        } else {
                            paramValue = "-";
                        }
//                        dataName[i] = nameList.at(0) + ";" + nameList.at(1) + ";" + QString::number(row_count);
                        ok = true;
                        break;
                    }
                }
                if (!ok) {
                    QStringList valueList = dataValue.at(i).split(";");
                    paramValue = valueList.at(row_count);
//                    dataName[i] = nameList.at(0) + ";" + nameList.at(1) + ";" + QString::number(row_count);
                }
            }
        }
    }
}

void MainWindow::setValueImage(const int recNo, const QString paramName, QImage &paramValue, const int reportPage)
{
    Q_UNUSED(recNo);
    Q_UNUSED(reportPage);
//    qDebug() << "From setValueImage" << paramName;
    for (int i = 0; i < dataName.length(); i++) {
        QStringList nameList = dataName.at(i).split(";");
        if (paramName == nameList.at(0)) {
            if (nameList.at(1) != nameList.at(2)) {
                QStringList valueList = dataValue.at(i).split(";");
                int row_count = nameList.at(2).toInt();
                QImage *Image = new QImage(valueList.at(row_count));
                paramValue = *Image;
                dataName[i] = nameList.at(0) + ";" + nameList.at(1) + ";" + QString::number(row_count++);
            }
        }
    }
}
