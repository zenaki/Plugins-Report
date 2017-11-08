#include "mainwindow.h"
#include <QApplication>
#include <QTimer>

#include "mysql.h"

#define VERSION "0.1 beta"

QStringList processArgs(int argc, char **argv)
{
    bool err = false;
    QStringList result;
    if (argc < 2) {
        printf(" -cnf  / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
               " -db   / --database DATABASE_NAME\t: Database Name for Connection Database\n\n"
               " -f    / --file-name OUTPUT_NAME\t: PDF File Name (default report.pdf)\n\n"
               " -host / --host-name HOST_NAME\t\t: Host Name for Connection Database\n\n"
               " -h    / --help\t\t\t\t: Print this help info\n\n"
               " -par  / --parameter P1#P2#P3#P4#...\t\t: Parameter for Query on Configuration(file.json)\n\n"
               " -pwd  / --password PASSWORD_DB\t\t: Password for Connection Database\n\n"
               " -tmp  / --template TEMPLATE_XML_FILE\t: Template(file.xml) for Report\n\n"
               " -t    / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
               " -usr  / --username USERNAME_DB\t\t: Username for Connection Database\n\n"
               " -v    / --version\t\t\t: Print version of plugins\n\n"
               " ex    : -tmp report_template.xml -cnf config_template.json -host localhost -db database_name -usr root -pwd root -t 10000\n\n"
              );
        err = false;
    } else if (argc == 2) {
        QString arg1(argv[1]);
        if (arg1 == QLatin1String("-h") ||
            arg1 == QLatin1String("--help")) {
            printf(" -cnf  / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
                   " -db   / --database DATABASE_NAME\t: Database Name for Connection Database\n\n"
                   " -f    / --file-name OUTPUT_NAME\t: PDF File Name (default report.pdf)\n\n"
                   " -host / --host-name HOST_NAME\t\t: Host Name for Connection Database\n\n"
                   " -h    / --help\t\t\t\t: Print this help info\n\n"
                   " -par  / --parameter P1#P2#P3#P4#...\t\t: Parameter for Query on Configuration(file.json)\n\n"
                   " -pwd  / --password PASSWORD_DB\t\t: Password for Connection Database\n\n"
                   " -tmp  / --template TEMPLATE_XML_FILE\t: Template(file.xml) for Report\n\n"
                   " -t    / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
                   " -usr  / --username USERNAME_DB\t\t: Username for Connection Database\n\n"
                   " -v    / --version\t\t\t: Print version of plugins\n\n"
                   " ex    : -tmp report_template.xml -cnf config_template.json -host localhost -db database_name -usr root -pwd root -t 10000\n\n"
                  );
            err = false;
        } else if (arg1 == QLatin1String("-v") ||
                 arg1 == QLatin1String("--version")) {
            printf(" Report Plugin Version: %s \nPT. DaunBiru Engiinering\nwww.daunbiru.com\n\n", VERSION);
            err = false;
        }
    } else  if (argc > 2) {
        QString temp = "",
                conf = "",
                out  = "",
                par  = "",
                host = "",
                db   = "",
                usr  = "",
                pwd  = "";
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
            } else if (arg1 == QLatin1String("-f") ||
                       arg1 == QLatin1String("--file-name")) {
                out = argv[i+1];
//                qDebug() << "Output File Name : " << out;
            } else if (arg1 == QLatin1String("-host") ||
                       arg1 == QLatin1String("--host-name")) {
                host = argv[i+1];
//                qDebug() << "Host Name        : " << host;
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
            result.append(par);
            result.append(host);
            result.append(db);
            result.append(usr);
            result.append(pwd);
        } else {
            err = true;
        }
    }
    if (err) {
        printf("{\"ERR\": \"Wrong Plugin Commands\"}\n\n");
    }

    result.append("../Report_Gui/sample_report.xml");
    result.append("../Report_Gui/sample_config.json");
    result.append("Testing.pdf");
    result.append("");
    result.append("localhost");
    result.append("monita_alarm");
    result.append("root");
    result.append("root");

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
    QApplication a(argc, argv);
    MainWindow w;
//    a.connect(&w, SIGNAL(close()), &a, SLOT(quit()));
    QStringList parameter = processArgs(argc, argv);
    if (parameter.length() > 0) {
        if (!parameter.at(4).isEmpty()) {
            mysql database;
            w.db = database.connect_db("report_plugin", parameter.at(4), parameter.at(5), parameter.at(6), parameter.at(7));
        }
        w.generateReport(parameter.at(0), parameter.at(1), parameter.at(2), parameter.at(3));
        a.quit();
//        return a.exec();
    } else {
        return 0;
    }
//    mysql database;
//    w.db = database.connect_db("report_plugin", "localhost", "monita_alarm", "root", "root");
//    w.generateReport("../Report_Gui/sample_report.xml",
//                     "../Report_Gui/sample_config.json",
//                     "Testing.pdf", "");
//    return a.exec();
}
