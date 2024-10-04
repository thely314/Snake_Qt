#ifndef CUSTOMBTN_H
#define CUSTOMBTN_H

#include <QPushButton>

class CustomBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomBtn(QWidget *parent = nullptr);
    explicit CustomBtn(const QString& Text, QWidget *parent = nullptr);

    ~CustomBtn();

signals:
};

#endif // CUSTOMBTN_H
