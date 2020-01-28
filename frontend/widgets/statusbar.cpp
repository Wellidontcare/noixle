#include "statusbar.h"

StatusBar::StatusBar()
    : pos_label_(new QLabel("0 0"))
    ,rgb_label_(new QLabel("0 0 0"))
    ,format_label_(new QLabel("None"))
    ,file_path_label_(new QLabel("No image loaded"))
{

}
