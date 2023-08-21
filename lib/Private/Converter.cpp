#include "Converter.h"

using namespace DevZA300;

QByteArray Converter::fromString(const QString &string)
{
    QByteArray array;
    array = string.toUtf8();
    return array.leftJustified(string.length(), '\0');
}

QString Converter::toString(const QByteArray &array)
{
    QString result;
    result = QString::fromUtf8(array);
    result.remove(QChar('\0'), Qt::CaseInsensitive);
    return result;
}
