#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include <QWidget>
#include "world.h"

namespace Ui {
class PauseScreen;
}

class PauseScreen : public QWidget
{
	Q_OBJECT

public:
	explicit PauseScreen(QWidget *parent = 0);
	~PauseScreen();

signals:
	void resumeClicked();
	void restartClicked();

private slots:
	void on_btnResume_clicked();

	void on_btnHelp_clicked();

	void on_btnCheat_clicked();

	void on_btnRestart_clicked();

	void on_btnSaveState_clicked();

	void on_btnLoadState_clicked();

	void on_btnTitle_clicked();

	void on_btnExit_clicked();

private:
	Ui::PauseScreen *ui;
};

#endif // PAUSESCREEN_H
