#include "highscorepage.h"
#include "ui_highscorepage.h"
#include "titlescreen.h"
#include "mainwidget.h"
#include "highscore.h"


HighScorePage::HighScorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HighScorePage)
{
     widgetParent = parent;
     ui->setupUi(this);

	 HighScore::instance().LoadScore("data/" + World::instance().getLevelName());

     ui->lblNewHighScoreInfo->hide();
     ui->ledNewName->hide();
     ui->btnEnterName->hide();
     setNewScores();
}

HighScorePage::~HighScorePage()
{
    delete ui;
}

void HighScorePage::on_btnReturnHome_clicked()
{
    HighScore::instance().SaveScores(World::instance().getLevelName());
    deleteLater();
}

void HighScorePage::showNameEnter(bool show){

	ui->scoreListWidget->setVisible(!show);
	ui->btnReturnHome->setVisible(!show);
	ui->lblNewHighScoreInfo->setVisible(show);
	ui->ledNewName->setVisible(show);
	ui->btnEnterName->setVisible(show);
}


void HighScorePage::on_btnEnterName_clicked()
{
    name = ui->ledNewName->text();
    string nameInString = name.toStdString();
    HighScore::instance().NewHighScoreName(nameInString, place);
    setNewScores();
    for( int i = 0; i < this->children().size(); i++) {

            QLabel* textLabel =dynamic_cast<QLabel*>(this->children().at(i));

            if( textLabel != NULL ) {
                     textLabel->show();
            }
    }
	showNameEnter(false);
    HighScore::instance().SaveScores(World::instance().getLevelName());
}

void HighScorePage::setNewScores(){


    ui->lblHighscore1->setText(QString("%1").arg(HighScore::instance().getScore(0)));
    ui->lblHighscore2->setText(QString("%1").arg(HighScore::instance().getScore(1)));
    ui->lblHighscore3->setText(QString("%1").arg(HighScore::instance().getScore(2)));
    ui->lblHighscore4->setText(QString("%1").arg(HighScore::instance().getScore(3)));
    ui->lblHighscore5->setText(QString("%1").arg(HighScore::instance().getScore(4)));
    ui->lblHighscore6->setText(QString("%1").arg(HighScore::instance().getScore(5)));
    ui->lblHighscore7->setText(QString("%1").arg(HighScore::instance().getScore(6)));
    ui->lblHighscore8->setText(QString("%1").arg(HighScore::instance().getScore(7)));
    ui->lblHighscore9->setText(QString("%1").arg(HighScore::instance().getScore(8)));
    ui->lblHighscore10->setText(QString("%1").arg(HighScore::instance().getScore(9)));

    ui->lblName1->setText(QString::fromStdString(HighScore::instance().getName(0)));
    ui->lblName2->setText(QString::fromStdString(HighScore::instance().getName(1)));
    ui->lblName3->setText(QString::fromStdString(HighScore::instance().getName(2)));
    ui->lblName4->setText(QString::fromStdString(HighScore::instance().getName(3)));
    ui->lblName5->setText(QString::fromStdString(HighScore::instance().getName(4)));
    ui->lblName6->setText(QString::fromStdString(HighScore::instance().getName(5)));
    ui->lblName7->setText(QString::fromStdString(HighScore::instance().getName(6)));
    ui->lblName8->setText(QString::fromStdString(HighScore::instance().getName(7)));
    ui->lblName9->setText(QString::fromStdString(HighScore::instance().getName(8)));
    ui->lblName10->setText(QString::fromStdString(HighScore::instance().getName(9)));


}
