#include "dir2bin.h"

#include <QDirIterator>
#include "Private/Converter.h"
#include "Private/Entry.h"
#include "Private/DataList.h"

using namespace DevZA300;

/*!
 * \brief exploreDir - чтение объектов каталога
 * \param root - каталог, относительного которого изучается содержимое
 * \param path - полный путь к каталогу
 * \return - список структур объектов фаловой системы
 */
inline QList<Entry> exploreDir(const QDir root, const QString &path)
{
    QList<Entry> result;

    QDirIterator iterator(path, QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    while (iterator.hasNext())
    {
        auto info = QFileInfo(iterator.next());

        Entry::Type type = Entry::undefined;
        QByteArray data = nullptr;

        // определение типа объекта
        if (info.isSymLink())
        {
            type = Entry::symlink;
            data = Converter::fromString(info.symLinkTarget());
        }
        else if (info.isDir())
        {
            type = Entry::dir;
        }
        else
        {
            type = Entry::file;

            auto file = QFile(info.absoluteFilePath());
            file.open(QFile::ReadOnly);
            data = file.readAll();
            file.close();
        }

        // абсолютный путь объекта
        QString entry_path = root.relativeFilePath(info.absoluteFilePath());
        Entry entry(entry_path, type, info.permissions(), data);
        result.append(entry);

        if (entry.getType() != Entry::dir)
            continue;

        // рекурсия если объект каталог
        result += exploreDir(root, info.absoluteFilePath());
    }

    return result;
}

/*!
 * \brief writeEntry - записать объект
 * \param root - каталог, относительного которого осуществляется запись
 * \param entry - объект файловой системы
 */
inline void writeEntry(const QDir &root, const Entry &entry)
{
    QString path = root.absoluteFilePath(entry.getPath());

    switch (entry.getType())
    {
    case Entry::undefined:
        break;

    case Entry::dir:
        root.mkdir(path);
        QFile::setPermissions(path, (QFile::Permission) entry.getPermissions());
        break;

    case Entry::file:
    {
        QFile file(path);
        file.open(QFile::WriteOnly);
        file.setPermissions((QFile::Permission) entry.getPermissions());
        file.write(entry.getData());
        file.close();
        break;
    }

    case Entry::symlink:
        QFile::link(Converter::toString(entry.getData()), path);
        break;
    }
}

/*!
 * \brief changeEntry - перезаписать объект файловой системы
 * \param root - каталог, относительного которого осуществляется запись
 * \param old_entry - старый объект
 * \param new_entry - новый объект
 */
inline void changeEntry(const QDir &root, const Entry &old_entry, const Entry &new_entry)
{
    QString path = root.absoluteFilePath(old_entry.getPath());

    if (old_entry.getType() == Entry::dir)
    {
        QDir dir(path);
        dir.removeRecursively();
    }
    else
    {
        QFile file(path);
        file.remove();
    }

    writeEntry(root, new_entry);
}

void dir2bin::toBin(const QString &dir, const QString &bin)
{
    QDir root(dir);
    auto entries = exploreDir(root, dir);
    DataList<Entry> dataList(entries);

    QFile file(bin);
    file.open(QFile::Truncate | QFile::WriteOnly);
    file.write(dataList.toArray());
}

void dir2bin::toDir(const QString &bin, const QString &dir)
{
    QFile file(bin);
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();

    DataList<Entry> dataList(data);
    QList<Entry> entries = dataList.getList();

    QDir root(dir);
    if (!root.exists())
        root.mkpath(root.absolutePath());

    // существующая структура каталога
    auto old_entries = exploreDir(root, dir);

    // от самого длинного к самому короткому - для первоначального удаления файлов
    std::reverse(old_entries.begin(), old_entries.end());

    // создание копии для модификации данных
    auto new_entries = entries;

    /**************************** поиск объектов, которые нужно удалить ****************************/
    foreach (auto old_entry, old_entries)
    {
        bool exist = false;
        foreach (auto new_entry, new_entries)
        {
            if (old_entry.getPath() != new_entry.getPath())
                continue;

            exist = true;

            // удаление объекта из дальнейшей обработки
            new_entries.removeOne(new_entry);

            if (old_entry == new_entry)
                break;

            // если новый объект отличется от старого - перезаписать
            changeEntry(root, old_entry, new_entry);
            break;
        }

        if (exist)
            continue;

        // если объект отсутствует в новом списке - удалить
        QString path = root.absoluteFilePath(old_entry.getPath());
        if (old_entry.getType() == Entry::dir)
        {
            QDir dir(path);
            dir.removeRecursively();
        }
        else
        {
            QFile file(path);
            file.remove();
        }
    }

    // от самого короткого к самому длинному - для первоначального создания каталогов
    std::sort(new_entries.begin(), new_entries.end());

    /**************************** в списке остались только объекты, которые нужно создать ****************************/
    foreach (auto entry, new_entries)
    {
        writeEntry(root, entry);
    }
}
