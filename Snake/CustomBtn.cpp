#include "CustomBtn.h"

CustomBtn::CustomBtn(QWidget *parent)
    : QPushButton{parent}
{

}

CustomBtn::CustomBtn(const QString& Text, QWidget *parent)
    : QPushButton{Text, parent}
{

}

CustomBtn::~CustomBtn()
{

}
