#ifndef HIGHSCOREPAGE_H
#define HIGHSCOREPAGE_H

#include <QWidget>
#include <QString>

namespace Ui {
class HighScorePage;
}

class HighScorePage : public QWidget
{
    Q_OBJECT

public:
    explicit HighScorePage(QWidget *parent = 0);
    ~HighScorePage();
	 void showNameEnter(bool show);
     QString getName() {return name; }
     void showHighScores();
     void setNewScores();
     void setPlace(int newPlace) { place = newPlace; }
     void setScore(int newScore) { score = newScore; }

private slots:
    void on_btnReturnHome_clicked();

    void on_btnEnterName_clicked();

private:
    Ui::HighScorePage *ui;
    QWidget * widgetParent;
    QString name;
    int score;
    int place;
};

#endif // HIGHSCOREPAGE_H
