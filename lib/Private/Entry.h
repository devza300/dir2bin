#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include "DataList.h"

namespace DevZA300 {

struct pack;
#pragma pack(push, 1)

/*!
 * \brief The Entry struct - структура объекта файловой системы
 */
struct Entry
{
    /*!
     * \brief The Type enum - поддерживаемые типы объектов
     */
    enum Type : quint16
    {
        /// тип не задан
        undefined,

        /// файл
        file,

        /// каталог
        dir,

        /// символическая ссылка для ОС Linux / ярлык для Windows
        symlink
    };

    /*!
     * \brief Entry - конструктор, инициализация из массива данных
     * \param data - по умолчанию инициализируется нулями
     */
    Entry(const QByteArray &data = nullptr);

    /*!
     * \brief Entry - конструктор, инициализация значениями
     * \param path - название объекта (относительно точки входа)
     * \param type - тип объекта
     * \param permissions - маска прав
     * \param data - содержимое объекта
     */
    Entry(QString path,
          Type type,
          quint32 permissions,
          QByteArray data);

    /// название объекта
    QString getPath() const;
    /// название объекта
    void setPath(const QString &path);

    /// тип объекта
    Type getType() const;
    /// тип объекта
    void setType(const Type &type);

    /// маска прав
    quint32 getPermissions() const;
    /// маска прав
    void setPermissions(const quint32 &permissions);

    /// содержимое объекта
    QByteArray getData() const;
    /// содержимое объекта
    void setData(const QByteArray &data);

    /// преобразовать в массив байт
    QByteArray toArray() const;

    /// перегруз оператора тождества
    bool operator==(const Entry &entry) const;

    /// перегруз оператора меньше для сортировки
    bool operator<(const Entry &entry) const;

    /// полный размер структуры
    int getSize() const;

private:
    /*!
     * \brief The EntryHeader struct - внутренняя структура для хранения полей фиксировнного размера
     */
    struct EntryHeader
    {
        ObjectSize objectSize;      //!< размер объекта структуры
        Type type;                  //!< тип объекта

        quint32 pathLenght;         //!< длина названия
        quint32 permissions;        //!< маска прав

        quint8 unused[2];           //!< зарезервированно под обратную совместимость
    };

    EntryHeader mm_header;          //!< загловок

    QByteArray mm_path;             //!< название объекта (относительно точки входа)
    QByteArray mm_data;             //!< содержимое объекта

};

#pragma pack(pop)

} // namespace DevZA300

#endif // ENTRY_H
