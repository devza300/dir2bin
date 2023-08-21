#ifndef CONVERTER_H
#define CONVERTER_H

#include <QByteArray>
#include <QString>

namespace DevZA300 {

/*!
 * \brief The Converter class - преобразователь данных Object-типа
 * к массиву байт и наоборот, приведение данных SourceObject-типа
 */
class Converter
{
public:

    // запрет вызова конструктора
    Converter() = delete;

    /*!
     * \brief toArray - преобразование строки QString в массиа байт
     * \param object - строка QString
     * \return - массив байт
     */
    static QByteArray fromString(const QString &string);

    /*!
     * \brief toString - приведение к строке QString
     * \param array - массив байт
     * \return - строка
     */
    static QString toString(const QByteArray &array);

    /*!
     * \brief toArray - преобразование объекта Object-типа в массиа байт
     * \param object - объект Object-типа
     * \return - массив байт
     */
    template <typename Object>
    static QByteArray toArray(const Object &object)
    {
        char buffer[sizeof (Object)];
        memcpy(&buffer, &object, sizeof (Object));

        QByteArray array;
        array.append(buffer, sizeof (Object));

        return array;
    }

    /*!
     * \brief toObject - сохранение массива байт в объект Object-типа
     * \param array - массив байт
     * \param container - контейнер Object-типа
     */
    template <typename Object>
    static void toObject(const QByteArray &array, Object &container)
    {
        auto zeroContainer = [](Object &container, int size)
        {
            // создание нулевых данных для заполнения контейнера
            char *buffer = (char*) malloc(size);
            memset(buffer, 0, size);

            memcpy(&container, buffer, size);
        };

        int size = sizeof (Object);

        if (array.size() < size)
            zeroContainer(container, size);

        if (array == nullptr)
            return;

        memcpy(&container, array.data(), array.size() < size ? array.size() : size);
    }

    /*!
     * \brief copyObject - копирование данных SourceObject-объекта в объект Object-типа
     * \param source - SourceObject-объект
     * \param container - контейнер Object-типа
     */
    template <typename SourceObject, typename Object = SourceObject>
    static void copyObject(const SourceObject &source, Object &container)
    {
        QByteArray array = Converter::toArray(source);
        Converter::toObject(array, container);
    }

    /*!
     * \brief dataToString - преобразование строки string в QString
     * \param data - срока string
     * \return - строка
     */
    template <typename string>
    static QString dataToString(const string &data)
    {
        return Converter::toString(Converter::toArray(data));
    }

    /*!
     * \brief stringToData - сохранение строки QString в string
     * \param value - строка QString
     * \param container - контейнер string
     */
    template <typename string>
    static void stringToData(const QString &value, string &container)
    {
        Converter::toObject(Converter::fromString(value), container);
    }

};

} // namespace DevZA300

#endif // CONVERTER_H
