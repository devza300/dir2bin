#include "Iterator.h"

using namespace DevZA300;

Iterator::Iterator(const QByteArray &data) :
    mm_data(data),
    mm_index(0)
{

}

QByteArray Iterator::nextValue(const quint64 &size)
{
    auto value = mm_data.mid(mm_index, size);
    mm_index += value.size();
    return value;
}

QByteArray Iterator::unusedValue() const
{
    int length = mm_data.length();
    auto value = mm_data.right(length - mm_index);
    return value;
}
