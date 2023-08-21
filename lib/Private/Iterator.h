#ifndef ITERATOR_H
#define ITERATOR_H

#include <QByteArray>

namespace DevZA300 {

/*!
 * \brief The Iterator class - итератор по массиву QByteArray
 */
class Iterator
{
public:
    /*!
     * \brief hrProtocol - конструктор, принимает обрабатываемые данные
     * \param data - массив данных
     */
    Iterator(const QByteArray &data);

    /*!
     * \brief nextValue - возвращает следующий фрагмент данных
     * \param size - длина фрагмента
     * \return - фрагмент данных
     */
    QByteArray nextValue(const quint64 &size);

    /*!
     * \brief unusedValue - возвращает оставшийся фрагмент данных
     * \return - фрагмент данных
     */
    QByteArray unusedValue() const;

private:
    QByteArray mm_data;     //!< массив данных
    quint64 mm_index;       //!< индекс перемещения по массиву данных
};

} // namespace DevZA300

#endif // ITERATOR_H
