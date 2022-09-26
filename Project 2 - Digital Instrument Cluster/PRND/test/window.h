#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QPushButton;
class Window : public QWidget
{

    public:
        explicit Window(QWidget *parent = nullptr);
    private:
        QPushButton *m_button;
    signals:
    public slots:
};

#endif // WINDOW_H
