#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>
#include <QFileInfo>

#include "dir2bin.h"

#define out QTextStream(stdout)
#define err QTextStream(stderr)

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(VERSION " " GIT_VERSION);

    // создание парсера параметров
    QCommandLineParser parser;
    parser.setApplicationDescription(APP_NAME" helper");
    parser.addHelpOption();
    parser.addVersionOption();

    // определение параметров
    parser.addOption(QCommandLineOption(QStringList() << "s" << "src",
                                        "Specify the source file or directory in compression mode or the d2b-file in extraction mode.",
                                        "source"));
    parser.addOption(QCommandLineOption(QStringList() << "t" << "tgt",
                                        "Specify the d2b-file in extraction mode or the source file or directory in compression mode.",
                                        "target"));
    parser.addOption(QCommandLineOption(QStringList() << "c" << "compress", "Set compression mode, specify compression level between [-1 ... 9].", "level" , "-1"));

    // парсинг входных параметров
    parser.parse(QCoreApplication::arguments());

    // сообщение об ошибках, если не заполненны аргументы
    if (!parser.isSet("src"))
    {
        parser.process(app);

        err << "ERROR: src isn't set!" << Qt::endl << Qt::endl;
        parser.showHelp(EXIT_FAILURE);
    }
    if (!parser.isSet("tgt"))
    {
        parser.process(app);

        err << "ERROR: tgt isn't set!" << Qt::endl << Qt::endl;
        parser.showHelp(EXIT_FAILURE);
    }

    // получение агументов
    QString source = parser.value("src");
    QString target = parser.value("tgt");

    // проверка существования исходного файла (каталога)
    QFileInfo sourceInfo(source);
    if (!sourceInfo.exists())
    {
        err << QString("ERROR: src path <%1> isn't exist!").arg(source) << Qt::endl;
        return EXIT_FAILURE;
    }

    bool isCompressMode = parser.isSet("compress");
    if (isCompressMode)
    {
        int compression = parser.value("compress").toInt();
        DevZA300::dir2bin::toBin(source, target, compression);
    }
    else
    {
        if (!sourceInfo.isFile())
        {
            err << QString("ERROR: src file <%1> isn't compressed dir2bin file!").arg(source) << Qt::endl;
            return EXIT_FAILURE;
        }

        DevZA300::dir2bin::toDir(source, target);
    }

    out << "Complete!" << Qt::endl;
    return EXIT_SUCCESS;
}
