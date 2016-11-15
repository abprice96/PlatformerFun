#ifndef ENDGAME_H
#define ENDGAME_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {
class EndGame;
}

class EndGame : public QWidget
{
    Q_OBJECT

	bool gameOver;

public:
	EndGame(QWidget *parent = 0, bool gameOver_ = true);
    ~EndGame();

	bool checkHighScore();
    QTimer* getBonusTimer() { return bonusTimer; }


private slots:
	void on_PBcontinue_clicked();
    void bonusTimerHit();

private:
    Ui::EndGame *ui;
    QWidget* widgetParent;
    QTimer* bonusTimer;
    int bonusToAdd;
    int bonusAmount;
};

#endif // ENDGAME_H
