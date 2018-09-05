#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdio.h"
#include "stdlib.h"

MainWindow::MainWindow(QWidget *parent) :
    QObject(parent)
//    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
//    delete ui;
}

QString MainWindow::generateReport_pdf(QString template_path, QString config_path, QString output_name, QString parameter)
{
    QString fileName = template_path;
    report = new QtRPT(this);
//    report->setBackgroundImage(QPixmap(dir.absolutePath()+"/examples_report/qt_background_portrait.png"));
    QStringList paramError = this->readParam(config_path, parameter);
    if (paramError.length() > 0) {
        QString msg = "{\"ERR\":\"";
        for (int i = 0; i < paramError.length(); i++) {
            msg = msg + paramError.at(i);
        }
        msg = msg + "\"}";
//        printf(msg.toUtf8().data());
//        this->close();
//        QApplication::exit(1);
        return msg;
    } else {
        report->recordCount << max_row_count;
        report->recordCount << max_row_count;
        if (report->loadReport(fileName) == false) {
            QString msg = "{\"ERR\": \"Report "+fileName+" file not found\"}";
//            printf("{\"ERR\": \"Report %s file not found\"}\n\n", fileName.toUtf8().data());
//            this->close();
//            QApplication::exit(1);
            return msg;
        } else {
            QObject::connect(report, SIGNAL(setValue(const int, const QString, QVariant&, const int)),
                             this, SLOT(setValue(const int, const QString, QVariant&, const int)));
            QObject::connect(report, SIGNAL(setValueImage(const int, const QString, QImage&, const int)),
                             this, SLOT(setValueImage(const int, const QString, QImage&, const int)));
//            qDebug() << output_name;
            report->printPDF(output_name, false);
//            report->printExec();
            QString msg = "{\"SUCCESS\": \""+output_name+"\"}";
//            printf("{\"SUCCESS\": \"%s\"}\n\n", output_name.toUtf8().data());
//            this->close();
//            QApplication::exit(0);
            return msg;
        }
    }
}

QString MainWindow::generateReport_excel(QString template_path, QString config_path, QString output_name, QString parameter)
{
    QXlsx::Document xlsx;
    QStringList paramError = this->readParam(config_path, parameter);
    if (paramError.length() > 0) {
        QString msg = "{\"ERR\":\"";
        for (int i = 0; i < paramError.length(); i++) {
            msg = msg + paramError.at(i);
        }
        msg = msg + "\"}";
        return msg;
    } else {
        QFile loadFile(template_path);
        if (!loadFile.exists()) {
            QString msg = "{\"ERR\": \"Template File (.json) not exist !!\"}";
            return msg;
        }
        if (loadFile.open(QIODevice::ReadOnly)) {
            QByteArray readFile = loadFile.readAll();
            QJsonDocument JsonDoc = QJsonDocument::fromJson(readFile);
            QJsonObject object = JsonDoc.object();
            if (object.value("excel_template").isArray()) {
                QStringList row_extend;
                foreach (const QJsonValue & v,  object.value("excel_template").toArray()) {
                    if (v.toObject().value("type").toString() == "text") {
                        QString value = v.toObject().value("value").toString();
                        int x1 = v.toObject().value("x").toString().split(",").at(0).toInt();
                        QString temp1 = "."+value;

                        if (temp1.indexOf("[") > 0) {
                            temp1 = temp1.mid(2, temp1.indexOf("]")-2);
                            for (int i = 0; i < dataName.length(); i++) {
                                QString temp2 = dataName.at(i).split(";").at(0);
                                if (temp1 == temp2) {
                                    int temp3 = dataName.at(i).split(";").at(1).toInt();
                                    if (temp3 > 1) {
                                        if (row_extend.length() > 0) {
                                            bool b = false;
                                            for (int j = 0; j < row_extend.length(); j+=2) {
                                                if (row_extend.at(j) == QString::number(x1)) {
                                                    b = true;
                                                    break;
                                                }
                                            }
                                            if (!b) {
                                                row_extend.append(QString::number(x1));
                                                row_extend.append(QString::number(temp3-1));
                                            }
                                        } else {
                                            row_extend.append(QString::number(x1));
                                            row_extend.append(QString::number(temp3-1));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                foreach (const QJsonValue & v,  object.value("excel_template").toArray()) {
                    QString value = v.toObject().value("value").toString();
                    int x1 = v.toObject().value("x").toString().split(",").at(0).toInt();
                    int x2 = v.toObject().value("x").toString().split(",").at(1).toInt();
                    int y1 = v.toObject().value("y").toString().split(",").at(0).toInt();
                    int y2 = v.toObject().value("y").toString().split(",").at(1).toInt();

                    int extend = 0;
                    for (int i = 0; i < row_extend.length(); i+=2) {
                        if (row_extend.at(i).toInt() < x1) {
                            extend = extend + row_extend.at(i+1).toInt();
                        }
                    }
                    x1 = x1+extend;
                    y1 = y1+extend;

                    QXlsx::CellRange r;
                    r.setFirstRow(x1);
                    r.setFirstColumn(x2);
                    r.setLastRow(y1);
                    r.setLastColumn(y2);

                    QXlsx::CellReference x_ref;
                    x_ref.setRow(x1);
                    x_ref.setColumn(x2);
                    QXlsx::CellReference y_ref;
                    y_ref.setRow(y1);
                    y_ref.setColumn(y2);

                    xlsx.mergeCells(r);
                    xlsx.setColumnWidth(r.firstColumn(),r.lastColumn(), 15);
                    xlsx.setRowHeight(r.firstRow(),r.lastRow(), 20);

                    QString temp1 = "."+value;

                    QXlsx::Format format;
                    if (!v.toObject().value("bold").isUndefined()) {
                        format.setFontBold(v.toObject().value("bold").toBool());
                    }
                    if (!v.toObject().value("size").isUndefined()) {
                        format.setFontSize(v.toObject().value("size").toInt());
                    }
                    if (!v.toObject().value("horizontal_align").isUndefined()) {
                        if (v.toObject().value("horizontal_align").toString() == "AlignLeft") {
                            format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
                        } else if (v.toObject().value("horizontal_align").toString() == "AlignHCenter") {
                            format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                        } else if (v.toObject().value("horizontal_align").toString() == "AlignRight") {
                            format.setHorizontalAlignment(QXlsx::Format::AlignRight);
                        }
                    }
                    if (!v.toObject().value("vertical_align").isUndefined()) {
                        if (v.toObject().value("vertical_align").toString() == "AlignTop") {
                            format.setVerticalAlignment(QXlsx::Format::AlignTop);
                        } else if (v.toObject().value("vertical_align").toString() == "AlignVCenter") {
                            format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
                        } else if (v.toObject().value("vertical_align").toString() == "AlignBottom") {
                            format.setVerticalAlignment(QXlsx::Format::AlignBottom);
                        }
                    }
                    if (temp1.indexOf("[") > 0) {
                        if (!v.toObject().value("formula").isUndefined() && v.toObject().value("formula").toBool()) {
                            temp1 = temp1.mid(temp1.indexOf("[")+1, temp1.indexOf("]")-(temp1.indexOf("[")+1));
                            for (int i = 0; i < dataName.length(); i++) {
                                QString temp2 = dataName.at(i).split(";").at(0);
                                if (temp1 == temp2) {
                                    int temp3 = dataName.at(i).split(";").at(1).toInt();
                                    int x1_formula, x2_formula, y1_formula, y2_formula;
                                    QString key;
                                    foreach (const QJsonValue & val,  object.value("excel_template").toArray()) {
                                        QString tempFormula = "."+val.toObject().value("value").toString();
                                        x1_formula = val.toObject().value("x").toString().split(",").at(0).toInt();
                                        x2_formula = val.toObject().value("x").toString().split(",").at(1).toInt();
                                        y1_formula = val.toObject().value("y").toString().split(",").at(0).toInt();
                                        y2_formula = val.toObject().value("y").toString().split(",").at(1).toInt();
                                        if (tempFormula.indexOf("[") > 0) {
                                            tempFormula = tempFormula.mid(2, tempFormula.indexOf("]")-2);
                                            if (tempFormula == temp1) {
                                                key = tempFormula;
                                                break;
                                            }
                                        }
                                    }

                                    QXlsx::CellReference x_ref;
                                    x_ref.setRow(x1_formula);
                                    x_ref.setColumn(x2_formula);
                                    QXlsx::CellReference y_ref;
                                    y_ref.setRow(y1_formula+temp3-1);
                                    y_ref.setColumn(y2_formula);

                                    value.replace("["+key+"]", x_ref.toString()+":"+y_ref.toString());
//                                    xlsx.write(x1,x2,value,format);
                                    xlsx.currentWorksheet()->writeFormula(x1,x2, QXlsx::CellFormula(value), format);
                                }
                            }
                        } else {
                            temp1 = temp1.mid(2, temp1.indexOf("]")-2);
                            for (int i = 0; i < dataName.length(); i++) {
                                QString temp2 = dataName.at(i).split(";").at(0);
                                if (temp1 == temp2) {
                                    int temp3 = dataName.at(i).split(";").at(1).toInt();
                                    int temp4 = dataName.at(i).split(";").at(2).toInt();
                                    if (temp3 > 1) {
                                        for (int j = temp4; j < temp3; j++) {
                                            if (dataValue.at(i).split(";").length() > 1) {
                                                QXlsx::CellRange r_temp;
                                                r_temp.setFirstRow(x1+j);
                                                r_temp.setFirstColumn(x2);
                                                r_temp.setLastRow(y1+j);
                                                r_temp.setLastColumn(y2);

                                                xlsx.mergeCells(r_temp);
                                                xlsx.setColumnWidth(r_temp.firstColumn(),r_temp.lastColumn(), 15);
                                                xlsx.setRowHeight(r_temp.firstRow(),r_temp.lastRow(), 20);

                                                if (v.toObject().value("type") == "image") {
                                                    xlsx.insertImage(x1-1,x2-1, QImage(dataValue.at(i).split(";").at(j)).scaled(y1*70, y2*50));
                                                } else {
                                                    bool checkInt; double val = dataValue.at(i).split(";").at(j).toDouble(&checkInt);
                                                    if (checkInt) {
                                                        xlsx.write(x1+j,x2,val,format);
                                                    } else {
                                                        xlsx.write(x1+j,x2,dataValue.at(i).split(";").at(j),format);
                                                    }
                                                }
                                            }
                                        }
                                    } else {
                                        if (v.toObject().value("type") == "image") {
                                            xlsx.insertImage(x1-1,x2-1, QImage(dataValue.at(i)).scaled(y1*70, y2*50));
                                        } else {
                                            bool checkInt; double val = dataValue.at(i).toDouble(&checkInt);
                                            if (checkInt) {
                                                xlsx.write(x1,x2,val,format);
                                            } else {
                                                xlsx.write(x1,x2,dataValue.at(i),format);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        if (v.toObject().value("type") == "image") {
//                            qDebug() << value;
                            xlsx.insertImage(x1-1,x2-1, QImage(value).scaled(y1*70, y2*50));
                        } else {
                            bool checkInt; double val = value.toDouble(&checkInt);
                            if (checkInt) {
                                xlsx.write(x1,x2,val,format);
                            } else {
                                xlsx.write(x1,x2,value,format);
                            }
                        }
                    }
                }
            }
        }

        xlsx.saveAs(output_name);
        QString msg = "{\"SUCCESS\": \""+output_name+"\"}";
        return msg;
    }
}

QStringList MainWindow::readParam(QString config_path, QString parameter)
{
    max_row_count = 0;
    QStringList paramError;
    QFile loadFile(config_path);
    if (!loadFile.exists()) {
        paramError.append("Config File (.json) not exist !!");
    }
    if (loadFile.open(QIODevice::ReadWrite)) {
        QByteArray readFile = loadFile.readAll();

        QJsonDocument JsonDoc = QJsonDocument::fromJson(readFile);
        QJsonObject object = JsonDoc.object();
        if (object.value("report_param").isArray()) {
            foreach (const QJsonValue & v,  object.value("report_param").toArray()) {
                if (!v.toObject().value("value").toObject().value("query").isUndefined()) {
                    if (v.toObject().value("name").isArray()) {
                        QStringList result = database.read_m(db, v.toObject().value("value").toObject().value("query").toString(), parameter.split("#"));
                        if (result.at(0).indexOf("Error!::") > 0) {
                            paramError.append(result.at(0).split("::").at(1));
                        } else {
                            int name_cnt = 0;
                            int col_count = v.toObject().value("name").toArray().size();
                            foreach (const QJsonValue & v_name, v.toObject().value("name").toArray()) {
                                int row_count = 0;
                                QString data;
                                if (result.length() > 1) {
                                    for (int i = 0; i < result.length(); i+=col_count) {
                                        if (data.isEmpty()) {
                                            data = result.at(i+name_cnt);
                                        } else {
                                            data = data + ";" + result.at(i+name_cnt);
                                        }
                                        row_count++;
                                    }
                                    dataValue.append(data);
                                    dataName.append(v_name.toString() + ";" +
                                                    QString::number(row_count) + ";0");
                                    m_TableHeader.append(v_name.toString());
                                    max_row_count = max_row_count >= row_count ? max_row_count : row_count;

                                    name_cnt++;
                                    data.clear();
                                } else {
                                    row_count = 1;
                                    dataValue.append(result.at(0));
                                    dataName.append(v_name.toString() + ";1;0");
                                    max_row_count = max_row_count >= row_count ? max_row_count : row_count;
                                }
                            }
//                            qDebug() << "";
                        }
                    } else {
                        QStringList result = database.read(db, v.toObject().value("value").toObject().value("query").toString(), parameter.split("#"));
                        if (result.at(0).indexOf("Error!::") > 0) {
                            paramError.append(result.at(0).split("::").at(1));
                        } else {
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
                        }
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
    return paramError;
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
                            paramValue = "";
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
