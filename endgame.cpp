#include "endgame.h"
#include "ui_endgame.h"
#include "titlescreen.h"
#include "highscore.h"
#include "highscorepage.h"
#include "world.h"
#include <QRect>

EndGame::EndGame(QWidget *parent, bool gameOver_) :
    QWidget(parent),
	gameOver(gameOver_),
    ui(new Ui::EndGame)
{
    widgetParent = parent;
    ui->setupUi(this);
	if (!gameOver)
	{
        ui->PBcontinue->setEnabled(false);
        ui->lblBonusText->setStyleSheet("QLabel {color : red; }");
        bonusToAdd = World::instance().getSeconds();
        bonusTimer = new QTimer(this);
        bonusTimer->setInterval(30);
        connect(bonusTimer, SIGNAL(timeout()), this, SLOT(bonusTimerHit()));
		ui->lblTitle->setPixmap(QPixmap(":/images/LevelClear.png"));
		ui->lblBackground->setStyleSheet("");
        ui->lblBonusText->show();
        ui->lblBonusText->raise();
        bonusAmount = 0;
        bonusTimer->start();
	}
}

EndGame::~EndGame()
{
    delete ui;
}

bool EndGame::checkHighScore(){
	HighScore::instance().LoadScore("data/" + World::instance().getLevelName());
    World::instance().setScore(World::instance().getScore() + bonusAmount);
    if (World::instance().getScore() > HighScore::instance().getLowestScore()) {

		return true;
	} else {
		return false;
	}
}

void EndGame::on_PBcontinue_clicked()
{
	if (!gameOver && checkHighScore()) {


		int place = HighScore::instance().NewHighScore(World::instance().getScore());
        TitleScreen * title = new TitleScreen(widgetParent);
        title->show();
        title->raise();
        HighScorePage * highScoreScreen = new HighScorePage(title);
		highScoreScreen->setScore(World::instance().getScore());
		highScoreScreen->setPlace(place);
		for( int i = 0; i < highScoreScreen->children().size(); i++) {

			QLabel* textLabel =dynamic_cast<QLabel*>(highScoreScreen->children().at(i));

			if( textLabel != NULL ) {
				textLabel->hide();
			}
		}
		highScoreScreen->show();
		highScoreScreen->raise();
		highScoreScreen->showNameEnter(true);
	}
	else
	{
		TitleScreen* title = new TitleScreen(widgetParent);
		title->show();
		title->raise();
	}
    deleteLater();
}

void EndGame::bonusTimerHit() {
    bonusToAdd--;

    if (bonusToAdd == 0){
        bonusAmount+=2;
        bonusTimer->stop();
        ui->lblBonusText->setText("+" + QString("%1").arg(bonusAmount));
        ui->PBcontinue->setEnabled(true);
    } else {
         bonusAmount+=2;
         ui->lblBonusText->setText("+" + QString("%1").arg(bonusAmount));
    }

}
