#ifndef DATALIST_H
#define DATALIST_H

#include <QByteArray>

#include "Converter.h"
#include "Iterator.h"

namespace DevZA300 {

/*!
 * \brief ObjectSize - размер поля, используемого для хранения полного размера структуры
 */
typedef quint32 ObjectSize;

/*!
 * \brief The DataList struct - список структур
 */
template<typename Type>
struct DataList
{
    /*!
     * \brief DataList - конструктор, инициализация из массива данных
     * \param data - по умолчанию инициализируется нулями
     */
    DataList(const QByteArray &data = nullptr)
    {
        mm_listArray = data;
    }

    /*!
     * \brief TaskList - конструктор, инициализация заданными значениями
     * \param list - список структур
     */
    DataList(const QList<Type> &list)
    {
        setList(list);
    }

    /*!
     * \brief getList - возврат структурированных данных
     * \return - список заполенных структур
     */
    QList<Type> getList() const
    {
        QList<Type> list;

        QByteArray tmp = mm_listArray;
        while (tmp.size() != 0)
        {
            // опеределение размера порции данных
            ObjectSize size = 0;
            Converter::toObject(tmp.left(sizeof (ObjectSize)), size);

            // защита от зацикливания (если в поле FLAG_SIZE по каким-то причинам отсутствет размер)
            if (size == 0)
                break;

            // откустывание порции данных
            Iterator iterator(tmp);
            QByteArray data = iterator.nextValue(size);

            // заполнение полей структуры
            Type type(data);

            // сохранение в списке
            list.append(type);

            // оставшиеся данные отправяются на обработку
            tmp = iterator.unusedValue();
        }

        return list;
    }

    /*!
     * \brief setList - заполнение списка значениями
     * \param list - список заполненных структур
     */
    void setList(const QList<Type> &list)
    {
        mm_listArray.clear();
        foreach (auto tmp, list)
        {
            mm_listArray.append(tmp.toArray());
        }
    }

    /// преобразовать в массив байт
    QByteArray toArray() const
    {
        return mm_listArray;
    }

private:
    QByteArray mm_listArray;  //!< массив данных

};

} // namespace DevZA300

#endif // DATALIST_H
