#include "Entry.h"

#include "Converter.h"
#include "Iterator.h"

using namespace DevZA300;

#define SIZE sizeof (EntryHeader) + mm_path.size() + mm_data.size()

Entry::Entry(const QByteArray &data)
{
    if (data == nullptr)
    {
        mm_header = EntryHeader();

        // очистка зарезервированного массива
        Converter::toObject(nullptr, mm_header.unused);

        // запись массивов
        mm_path = nullptr;
        mm_data = nullptr;
    }
    else
    {
        Iterator iterator(data);
        QByteArray internalData = iterator.nextValue(sizeof (EntryHeader));

        // запись внутренней структуры
        Converter::toObject(internalData, mm_header);

        // запись массивов
        mm_path = iterator.nextValue(mm_header.pathLenght);
        mm_data = iterator.unusedValue();
    }

    // сохранение размера структуры
    mm_header.objectSize = SIZE;
}

Entry::Entry(QString path,
             Type type,
             quint32 permissions,
             QByteArray data)
{
    // очистка зарезервированного массива
    Converter::toObject(nullptr, mm_header.unused);

    setPath(path);
    setType(type);
    setPermissions(permissions);
    setData(data);

    // сохранение размера структуры
    mm_header.objectSize = SIZE;
}

QString Entry::getPath() const
{
    return Converter::toString(mm_path);
}

void Entry::setPath(const QString &path)
{
    mm_path = Converter::fromString(path);

    // сохранение размера имени объекта
    mm_header.pathLenght = mm_path.size();

    // сохранение размера структуры
    mm_header.objectSize = SIZE;
}

Entry::Type Entry::getType() const
{
    return mm_header.type;
}

void Entry::setType(const Type &type)
{
    mm_header.type = type;
}

quint32 Entry::getPermissions() const
{
    return mm_header.permissions;
}

void Entry::setPermissions(const quint32 &permissions)
{
    mm_header.permissions = permissions;
}

QByteArray Entry::getData() const
{
    return qUncompress(mm_data);
}

void Entry::setData(const QByteArray &data)
{
    mm_data = qCompress(data);

    // сохранение размера структуры
    mm_header.objectSize = SIZE;
}

QByteArray Entry::toArray() const
{
    QByteArray data;
    data.append(Converter::toArray(mm_header));
    data.append(mm_path);
    data.append(mm_data);
    return data;
}

bool Entry::operator==(const Entry &entry) const
{
    // NOTE windows and linux permissions are differents
#ifdef _WIN32
    if (this->getPath() != entry.getPath())
        return false;

    return this->getData() == entry.getData();
#else
    return this->toArray() == entry.toArray();
#endif
}

bool Entry::operator<(const Entry &entry) const
{
    return this->getPath() < entry.getPath();
}

int Entry::getSize() const
{
    return mm_header.objectSize;
}
