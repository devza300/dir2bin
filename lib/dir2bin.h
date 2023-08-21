#ifndef DIR2BIN_H
#define DIR2BIN_H

#include <QtCore/qglobal.h>

#if defined(DIR2BIN_LIBRARY)
#  define DIR2BIN_EXPORT Q_DECL_EXPORT
#else
#  define DIR2BIN_EXPORT Q_DECL_IMPORT
#endif

namespace DevZA300 {

/*!
 * \brief The dir2bin class - класс для преобразования содержимого каталога
 * к списку структур и наоборот
 */
class DIR2BIN_EXPORT dir2bin
{
public:
    dir2bin() = delete;

    /*!
     * \brief toBin - сохранить содержимое каталога в файл
     * \param dir - базовый каталог
     * \param bin - файл для сохранения
     * \param compressionLevel - уровень сжатия данных
     */
    static void toBin(const QString &dir, const QString &bin, int compressionLevel = -1);

    /*!
     * \brief toDir - записать данные в заданный каталог
     * \param bin - бинарный файл
     * \param dir - выходной каталог
     */
    static void toDir(const QString &bin, const QString &dir);

};

} // namespace DevZA300

#endif // DIR2BIN_H
