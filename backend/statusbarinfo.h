#ifndef STATUSBARINFO_H
#define STATUSBARINFO_H
#include <QString>
#include <QSize>
struct StatusBarInfoStatic{
    QString file_name;
    QString format;
    QSize dimensions;
};

struct StatusBarInfoDynamic{
    int x;
    int y;
    int r;
    int g;
    int b;
};

#endif // STATUSBARINFO_H
