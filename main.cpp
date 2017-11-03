#include "mainwindow.h"
#include <QApplication>
#include <QTimer>

#define VERSION "0.1 beta"

QStringList processArgs(int argc, char **argv)
{
    bool err = false;
    QStringList result;
    if (argc < 2) {
        printf(" -cnf / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
               " -h   / --help\t\t\t\t: Print this help info\n\n"
               " -tmp / --template TEMPLATE_XML_FILE\t: Template(file.xml) for Report\n\n"
               " -t   / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
               " -v   / --version\t\t\t: Print version of plugins\n\n"
               " ex   : -tmp report_template.xml -cnf config_template.json -t 10000\n\n"
              );
        err = false;
    } else if (argc == 2) {
        QString arg1(argv[1]);
        if (arg1 == QLatin1String("-h") ||
            arg1 == QLatin1String("--help")) {
            printf(" -cnf / --config CONFIG_JSON_FILE\t: Configuration(file.json) for Template\n\n"
                   " -h   / --help\t\t\t\t: Print this help info\n\n"
                   " -tmp / --template TEMPLATE_XML_FILE\t: Template(file.xml) for Report\n\n"
                   " -t   / --timeout TIMEOUT(ms)\t\t: Timeout for end process (default 5000ms)\n\n"
                   " -v   / --version\t\t\t: Print version of plugins\n\n"
                   " ex   : -tmp report_template.xml -cnf config_template.json -t 10000\n\n"
                  );
            err = false;
        } else if (arg1 == QLatin1String("-v") ||
                 arg1 == QLatin1String("--version")) {
            printf(" Report Plugin Version: %s \nPT. DaunBiru Engiinering\nwww.daunbiru.com\n\n", VERSION);
            err = false;
        }
    } else  if (argc > 2) {
        QString temp = "",
                conf = "";
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-cnf") ||
                arg1 == QLatin1String("--config")) {
                conf = argv[i+1];
//                qDebug() << "Config File      : " << conf;
            } else if (arg1 == QLatin1String("-tmp") ||
                       arg1 == QLatin1String("--template")) {
                temp = argv[i+1];
//                qDebug() << "Template File    : " << temp;
            } /*else if (arg1 == QLatin1String("-t") ||
                       arg1 == QLatin1String("--timeout")) {
                timeout = atoi(argv[i+1]);
            }*/
        }
        if (!temp.isEmpty() && !conf.isEmpty()) {
            result.append(temp);
            result.append(conf);
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
    QApplication a(argc, argv);
    MainWindow w;
    a.connect(&w, SIGNAL(close()), &a, SLOT(quit()));
    QStringList parameter = processArgs(argc, argv);
//    w.show();
    w.generateReport("../Report_Gui/sample_report.xml",
                     "../Report_Gui/sample_config.json");

    return a.exec();
}
