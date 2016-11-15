#include <QTimer>
#include "titlescreen.h"
#include "ui_titlescreen.h"
#include "mainwidget.h"
#include "highscorepage.h"
#include "highscore.h"
#include "mapmaker.h"
#include <QMessageBox>
#include <QDirIterator>
#include <exception>
#include <stdexcept>
#include "world.h"


TitleScreen::TitleScreen(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::TitleScreen)
{
    widgetParent = parent;
	ui->setupUi(this);
    playing = false;
	ui->btnEasy->setFocus();
    World::instance().setIsPlaying(false);
    ui->lblHighScorePrompt->hide();
    ui->leInitialHighScore->hide();
    ui->btnGo->hide();
}

TitleScreen::~TitleScreen()
{
	delete ui;
}

void TitleScreen::on_btnEasy_clicked()
{
	startLevel(":/levels/easy.lv");
}

void TitleScreen::on_btnMedium_clicked()
{
	startLevel(":/levels/medium.lv");

}

void TitleScreen::on_btnHard_clicked()
{
	startLevel(":/levels/hard.lv");
}

void TitleScreen::on_btnOtherMap_clicked()
{
	QString line = "";
	if (ui->leOtherMap->text() == ""){
		QMessageBox::warning(this,"No File Specified", "Please enter a map to load.");
		return;
	}
	line = "data/" + ui->leOtherMap->text() + ".lv";
	startLevel(line);
}

void TitleScreen::on_lblHighScores_clicked()
{ 
	if(World::instance().getLevelName() == ""){

        ui->lblHighScorePrompt->show();
        ui->leInitialHighScore->show();
        ui->btnGo->show();
   } else {
    HighScorePage * scorePage = new HighScorePage(this->parentWidget());
    scorePage->show();
    scorePage->raise();
    }
}

void TitleScreen::on_btnMapMaker_clicked()
{
    MapMaker * maker = new MapMaker();
    maker->show();
    maker->raise();
}

void TitleScreen::on_btnHelp_clicked()
{
    HelpScreen * help = new HelpScreen(this->parentWidget());
    help->show();
    help->raise();

    deleteLater();
}


void TitleScreen::on_btnGo_clicked()
{

    ui->lblHighScorePrompt->hide();
    ui->leInitialHighScore->hide();
    ui->btnGo->hide();
    QString text= ui->leInitialHighScore->text().trimmed();
    string  highScoreFile = text.toStdString();
    text += "scores.txt";
    QDirIterator it(QDir::currentPath(), QDirIterator::NoIteratorFlags);
        while (it.hasNext())
        {
            it.next();
            for (int i = 1; i <= 3; i++)
            {

                if (it.fileName() == text)
                {
                    World::instance().setLevelName(highScoreFile);
                    HighScorePage * scorePage = new HighScorePage(this->parentWidget());
                    scorePage->show();
                    scorePage->raise();
                    return;
                }
            }
        }
    QMessageBox * q = new QMessageBox();
    q->setText("That file name couldn't be found!");
    q->raise();
    q->show();
}

void TitleScreen::startLevel(QString level)
{
	MainWidget * prnt = dynamic_cast<MainWidget *>(widgetParent);
	if (prnt != NULL)
	{
		try
		{
			prnt->loadLevel(level);
		}
		catch (exception& ex)
		{
			QMessageBox::warning(this, "Error!", "An error occurred.\n" + QString::fromStdString(ex.what()));
			return;
		}
		this->hide();
		this->widgetParent->setFocus();
		World::instance().setIsPlaying(true);
		prnt->getTimer()->start();
		prnt->getClock()->start();
        prnt->getRotateTimer()->start();
	}
	deleteLater();

}
