#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QWidget>
#include "mainwidget.h"
#include "helpscreen.h"

namespace Ui {
class TitleScreen;
}

class TitleScreen : public QWidget
{
	Q_OBJECT

public:
    explicit TitleScreen(QWidget *parent = 0);
    ~TitleScreen();

    bool isPlaying() { return playing; }

	void startLevel(QString level);
private slots:
	void on_btnEasy_clicked();

    void on_lblHighScores_clicked();

	void on_btnMapMaker_clicked();

    void on_btnMedium_clicked();

    void on_btnOtherMap_clicked();

    void on_btnHard_clicked();

	void on_btnHelp_clicked();

    void on_btnGo_clicked();

private:
	Ui::TitleScreen *ui;
    QWidget* widgetParent;

    bool playing;
};

#endif // TITLESCREEN_H
