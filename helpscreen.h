#ifndef HELPSCREEN_H
#define HELPSCREEN_H

#include <QWidget>

namespace Ui {
class HelpScreen;
}

class HelpScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HelpScreen(QWidget *parent = 0);
    ~HelpScreen();

private slots:
    void on_btnReturntoHome_clicked();

private:
    Ui::HelpScreen *ui;
    QWidget* widgetParent;
};

#endif // HELPSCREEN_H
