#include "mainwindow.h"
#include <QApplication>
#include <QTimer>

//#include <qapplication.h>
//#include <stdio.h>
//#include <stdlib.h>

#include "mysql.h"

#define VERSION "0.1 beta"

//void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    QByteArray localMsg = msg.toLocal8Bit();
//    switch (type) {
//    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//        break;
//    case QtInfoMsg:
//        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//        break;
//    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//        break;
//    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//        break;
//    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//        abort();
//    }
//}

QStringList processArgs(int argc, char **argv)
{
    bool err = false;
    QStringList result;
    if (argc < 2) {
        printf(" -cnf  / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
               " -db   / --database DATABASE_NAME\t: Database Name for Connection Database\n\n"
               " -dbp  / --database-port DATABASE_PORT\t: Port Database for Connection (default 3306)\n\n"
               " -f    / --file-name OUTPUT_NAME\t: File Name (default report.pdf / report.xlsx)\n\n"
               " -host / --host-name HOST_NAME\t\t: Host Name for Connection Database\n\n"
               " -h    / --help\t\t\t\t: Print this help info\n\n"
               " -mod  / --mode\t\t\t\t: Report Mode (pdf / excel)\n\n"
               " -par  / --parameter P1#P2#P3#P4#...\t: Parameter for Query on Configuration(file.json)\n\n"
               " -pwd  / --password PASSWORD_DB\t\t: Password for Connection Database\n\n"
               " -tmp  / --template TEMPLATE_FILE\t: Template(file.xml/file.json) for Report\n\n"
               " -t    / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
               " -usr  / --username USERNAME_DB\t\t: Username for Connection Database\n\n"
               " -v    / --version\t\t\t: Print version of plugins\n\n"
               " ex1    : -tmp report_template.xml -cnf config_template.json -mod pdf -host localhost -db database_name -dbp 3360 -usr root -pwd root -t 10000\n\n"
               " ex2    : -tmp report_template.json -cnf config_template.json -mod excel -host localhost -db database_name -dbp 3360 -usr root -pwd root -t 10000\n\n"
              );
        err = false;
    } else if (argc == 2) {
        QString arg1(argv[1]);
        if (arg1 == QLatin1String("-h") ||
            arg1 == QLatin1String("--help")) {
            printf(" -cnf  / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
                   " -db   / --database DATABASE_NAME\t: Database Name for Connection Database\n\n"
                   " -dbp  / --database-port DATABASE_PORT\t: Port Database for Connection (default 3306)\n\n"
                   " -f    / --file-name OUTPUT_NAME\t: File Name (default report.pdf / report.xlsx)\n\n"
                   " -host / --host-name HOST_NAME\t\t: Host Name for Connection Database\n\n"
                   " -h    / --help\t\t\t\t: Print this help info\n\n"
                   " -mod  / --mode\t\t\t\t: Report Mode (pdf / excel)\n\n"
                   " -par  / --parameter P1#P2#P3#P4#...\t: Parameter for Query on Configuration(file.json)\n\n"
                   " -pwd  / --password PASSWORD_DB\t\t: Password for Connection Database\n\n"
                   " -tmp  / --template TEMPLATE_FILE\t: Template(file.xml/file.json) for Report\n\n"
                   " -t    / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
                   " -usr  / --username USERNAME_DB\t\t: Username for Connection Database\n\n"
                   " -v    / --version\t\t\t: Print version of plugins\n\n"
                   " ex1    : -tmp report_template.xml -cnf config_template.json -mod pdf -host localhost -db database_name -dbp 3360 -usr root -pwd root -t 10000\n\n"
                   " ex2    : -tmp report_template.json -cnf config_template.json -mod excel -host localhost -db database_name -dbp 3360 -usr root -pwd root -t 10000\n\n"
                  );
            err = false;
        } else if (arg1 == QLatin1String("-v") ||
                 arg1 == QLatin1String("--version")) {
//            printf(" Report Plugin Version: %s \nPT. DaunBiru Engiinering\nwww.daunbiru.com\n\n", VERSION);
            QString compilationTime = QString("%1 %2").arg(__DATE__).arg(__TIME__);
            QString version = VERSION;
            printf("Report Plugin Version:  %s\nPT. DaunBiru Engineering\nwww.daunbiru.com\n\n"
                   "build on: %s (UTC+7)\n",
                   version.toUtf8().data(),
                   compilationTime.toUtf8().data());
            err = false;
        }
    } else  if (argc > 2) {
        QString temp = "",
                conf = "",
                out  = "",
                par  = "",
                host = "",
                db   = "",
                dbp  = "",
                usr  = "",
                pwd  = "",
                mode = "";
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-cnf") ||
                arg1 == QLatin1String("--config")) {
                conf = argv[i+1];
//                qDebug() << "Config File      : " << conf;
            } else if (arg1 == QLatin1String("-db") ||
                       arg1 == QLatin1String("--database")) {
                db = argv[i+1];
//                qDebug() << "Database Name    : " << db;
            } else if (arg1 == QLatin1String("-dbp") ||
                       arg1 == QLatin1String("--database-port")) {
                dbp = argv[i+1];
//                qDebug() << "Database Port    : " << dbp;
            } else if (arg1 == QLatin1String("-f") ||
                       arg1 == QLatin1String("--file-name")) {
                out = argv[i+1];
//                qDebug() << "Output File Name : " << out;
            } else if (arg1 == QLatin1String("-host") ||
                       arg1 == QLatin1String("--host-name")) {
                host = argv[i+1];
//                qDebug() << "Host Name        : " << host;
            } else if (arg1 == QLatin1String("-mod") ||
                       arg1 == QLatin1String("--mode")) {
                mode = argv[i+1];
//                qDebug() << "Mode             : " << mode;
            } else if (arg1 == QLatin1String("-par") ||
                       arg1 == QLatin1String("--parameter")) {
                par = argv[i+1];
//                qDebug() << "Parameter        : " << par;
            } else if (arg1 == QLatin1String("-pwd") ||
                       arg1 == QLatin1String("--password")) {
                pwd = argv[i+1];
//                qDebug() << "Password         : " << pwd;
            } else if (arg1 == QLatin1String("-tmp") ||
                       arg1 == QLatin1String("--template")) {
                temp = argv[i+1];
//                qDebug() << "Template File    : " << temp;
            } else if (arg1 == QLatin1String("-usr") ||
                       arg1 == QLatin1String("--username")) {
                usr = argv[i+1];
//                qDebug() << "Username         : " << usr;
            } /*else if (arg1 == QLatin1String("-t") ||
                       arg1 == QLatin1String("--timeout")) {
                timeout = atoi(argv[i+1]);
            }*/
        }

        if (!temp.isEmpty() && !conf.isEmpty()) {
            result.append(temp);
            result.append(conf);
            result.append(out);
            result.append(par.replace("__", " "));
            result.append(host);
            result.append(db);
            result.append(usr);
            result.append(pwd);
            result.append(mode);
            if (dbp.isEmpty()) {
                result.append("3306");
            } else {
                result.append(dbp);
            }
        } else {
            err = true;
        }
    }
    if (err) {
        printf("{\"ERR\": \"Wrong Plugin Commands\"}\n\n");
    }

    return result;
}

int timeOut(int argc, char **argv) {
    int tOut = 0;

    if (argc > 2) {
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-t") || arg1 == QLatin1String("--timeout")) {
                tOut = atoi(argv[i+1]);
            }
        }
    }

    return tOut;
}

int main(int argc, char **argv)
{
//    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    MainWindow w;
//    a.connect(&w, SIGNAL(cls()), &a, SLOT(quit()));

    QStringList parameter = processArgs(argc, argv);
    if (parameter.length() > 0) {
        if (!parameter.at(4).isEmpty()) {
            mysql database;
            w.db = database.connect_db("report_plugin", parameter.at(4), parameter.at(9).toInt(), parameter.at(5), parameter.at(6), parameter.at(7));
        }
        QString msg;
        if (parameter.at(8) == "pdf") {
            msg = w.generateReport_pdf(parameter.at(0), parameter.at(1), parameter.at(2), parameter.at(3));
        } else if (parameter.at(8) == "excel") {
            msg = w.generateReport_excel(parameter.at(0), parameter.at(1), parameter.at(2), parameter.at(3));
        } else {
            msg = "{\"ERR\":\"Report Mode Undefined\"}";
        }
        if (msg.indexOf("ERR") > 0) {
            qWarning() << msg;
//            qCritical() << msg;
//            qFatal(msg.toUtf8().data());
//            qDebug() << "Ini Error";
        } else {
            printf(msg.toUtf8().data());
//            qWarning() << msg;
//            qDebug() << "Ini Nggak Error";
        }
        a.quit();
        return 0;
    } else {
        return 0;
    }

//    mysql database;
//    w.db = database.connect_db("report_plugin", "localhost", 3306, "marine_1", "test", "test");
//    w.generateReport_excel("../Report_Gui/Test_Report/dsv_twinsister_504_template.json",
//                     "../Report_Gui/Test_Report/dsv_twinsister_504_config.json",
//                     "../Report_Gui/Test_Report/dsv_twinsister_504.xlsx",
//                     "");
////    w.generateReport("../Report_Gui/Test_Report/reportharian2.xml",
////                     "../Report_Gui/Test_Report/sample_config.json",
////                     "Testing_tlnaga.pdf", "");
//    a.quit();
//    return 0;
}
