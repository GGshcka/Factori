//
// Created by silly on 24.03.2025.
//

#ifndef FACTORI_DEBUGINFOHANDLER_H
#define FACTORI_DEBUGINFOHANDLER_H

#include <QDebug>
#include <QDateTime>

class DropLog : public QMessageLogger {
public:
    static DropLog &Logger() {
        static DropLog logger;
        return logger;
    }

    void Error(const char * tag, const char * info) {
        writeLog(0, tag, info);
    }

    void Warning(const char * tag, const char * info) {
        writeLog(1, tag, info);
    }

    void Info(const char * tag, const char * info) {
        writeLog(2, tag, info);
    }

private:
    DropLog() {}

    void writeLog(int level, const char *tag, const char *info) {
        QMap<int, QPair<QString, QString>>  levelDat{
                {0, qMakePair("ERROR", "\033[31m")},
                {1, qMakePair("WARNING", "\033[33m")},
                {2, qMakePair("INFO", "\033[32m")}
        };

        QString line = QString("%1%2 | [%3] | %4: %5")
                .arg(levelDat.value(level).second,
                     QDateTime::currentDateTime().toString("hh:mm:ss"),
                     levelDat.value(level).first,
                     tag,
                     info);


        switch (level) {
            case 0:
                QMessageLogger::critical().noquote() << line;
                break;
            case 1:
                QMessageLogger::warning().noquote() << line;
                break;
            case 2:
                QMessageLogger::info().noquote() << line;
                break;
        }
    }
};


#endif //FACTORI_DEBUGINFOHANDLER_H
