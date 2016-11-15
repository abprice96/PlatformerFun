#include <QLabel>
#include "mapmaker.h"
#include "ui_mapmaker.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QRect>
#include <string>
#include <fstream>
#include <iostream>
#include <QScrollBar>
#include <QIcon>

using namespace std;


MapMaker::MapMaker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapMaker)
{
    ui->setupUi(this);

    startX = 0;
    startY = 0;
    time = 300;

    scrollArea = new QScrollArea;
    ui->QWworld->resize(1022,766);
    scrollArea->setWidget(ui->QWworld);
    scrollArea->resize(1024,768);
    scrollArea->setWidgetResizable(false);
    scrollArea->setWindowTitle("World");

    scrollArea->show();

    for(int i = 0; i < this->children().size(); ++i){
        QPushButton* button = dynamic_cast<QPushButton*>(children().at(i));
        if (button != NULL) {
            connect(button, SIGNAL(pressed()), this, SLOT(on_any_click()));
        }
    }
}

MapMaker::~MapMaker()
{
    delete ui;
}

void MapMaker::makePlatform(MovableLabel *label, QString file, bool& successful, QString& errorMSG)
{
	int x = scrollArea->horizontalScrollBar()->value();
	int y = scrollArea->verticalScrollBar()->value();

    label->xRange = 0;
    label->yRange = 0;
    label->xSpeed = 0;
    label->ySpeed = 0;
    if (ui->LEwidth->text() == "" || ui->LEheight->text() =="") {
        successful = false;
        errorMSG = "Please enter width and height.";
        return;
    } else {
        label->setGeometry(x,y,ui->LEwidth->text().toInt(&successful),ui->LEheight->text().toInt(&successful));
    }
    if (ui->LExRange->text() != ""){
        label->xRange = ui->LExRange->text().toInt(&successful);
    } else {
        label->xRange = 0;
    }
    if (ui->LEyRange->text() != ""){
        label->yRange = ui->LEyRange->text().toInt(&successful);
    } else {
        label->yRange = 0;
    }
    if (ui->LExSpeed->text() != ""){
        label->xSpeed = ui->LExSpeed->text().toInt(&successful);
    } else {
        label->xSpeed = 0;
    }
    if (ui->LEySpeed->text() != ""){
        label->ySpeed = ui->LEySpeed->text().toInt(&successful);
    } else {
        label->ySpeed = 0;
    }
    if (!successful) {
        errorMSG = "Please enter intiger values.";
        ui->LEwidth->setText("");
        ui->LEheight->setText("");
        ui->LExRange->setText("");
        ui->LEyRange->setText("");
        ui->LExSpeed->setText("");
        ui->LEySpeed->setText("");
        return;
    }

    label->setPixmap(QPixmap(file));
    label->type = "platform";
    label->file = file.toStdString();
    label->setScaledContents(true);
}

void MapMaker::makeDecor(MovableLabel *label, QString file, int w, int h)
{
	int x = scrollArea->horizontalScrollBar()->value();
	int y = scrollArea->verticalScrollBar()->value();
	label->setGeometry(x,y,w,h);
    label->setPixmap(QPixmap(file));
    label->type = "object";
    label->file = file.toStdString();
    label->setScaledContents(true);
}

void MapMaker::makeItem(MovableLabel *label, QString file, string colType)
{
	int x = scrollArea->horizontalScrollBar()->value();
	int y = scrollArea->verticalScrollBar()->value();
	label->setGeometry(x,y,32,32);
    label->setPixmap(QPixmap(file));
    label->type = "collectible";
    label->file = file.toStdString();
    label->collectableType = colType;
    label->setScaledContents(true);
}

void MapMaker::labelClicked()
{
    MovableLabel *lbl = dynamic_cast<MovableLabel *>(sender());
    if (lbl != nullptr) {
        if(ui->PBdelete->isChecked()){
            lbl->deleteLater();
        }
    }
}

void MapMaker::on_PBmakeObject_clicked()
{
    int checkedPB = 0;
    for (int i = 0; i < this->children().size(); ++i){
        QPushButton * current = dynamic_cast<QPushButton *>(this->children().at(i));
        if (current != NULL){
            if (current->isChecked()){
                if (current->text() != "Delete") {
                    ++checkedPB;
                }
            }
        }
    }

    if (checkedPB != 1) {
        QMessageBox::warning(this,"Error","Choose one and only one Object.");
        for (int i = 0; i < this->children().size(); ++i){
            QPushButton * current = dynamic_cast<QPushButton *>(this->children().at(i));
            if (current != NULL){
                if (current->isChecked()){
                    current->setChecked(false);
                }
            }
        }
        return;
    }
    MovableLabel *label = new MovableLabel(this->ui->QWworld);
    connect(label, SIGNAL(clicked()), this, SLOT(labelClicked()));
    bool successful = true;
    QString errorMSG = "";

	int x = scrollArea->horizontalScrollBar()->value();
	int y = scrollArea->verticalScrollBar()->value();

    if (ui->PBplayer->isChecked()) {
        for (int i = 0; i < ui->QWworld->children().size(); ++i) {
            MovableLabel* current = dynamic_cast<MovableLabel*>(ui->QWworld->children().at(i));
            if (current!= NULL) {
                if (current->type == "player") {
                    successful = false;
                    errorMSG = "There can be only one player.";
                }
            }
        }
		label->setGeometry(x,y,25,48);
        label->setPixmap(QPixmap(":/images/maincharacter/stand.png"));
        label->type = "player";
        label->file = "images/maincharacter/stand.png";
    } else if (ui->PBcoin->isChecked()){
		label->setGeometry(x,y,32,32);
		label->setPixmap(QPixmap(":/images/goldCoin/goldCoin1.png"));
        label->type = "coin";
		label->file = ":/images/goldCoin/goldCoin1.png";
    } else if (ui->PBwin->isChecked()){
		label->setGeometry(x,y,50,50);
        label->setPixmap(QPixmap(":/images/flag.png"));
        label->setScaledContents(true);
        label->type = "endGame";
        label->file = ":/images/flag.png";
    } else if (ui->PBplatform1->isChecked()){
        makePlatform(label,":/images/bridgelogs.png",successful,errorMSG);
    } else if (ui->PBplatform2->isChecked()){
        makePlatform(label,":/images/bridgeplanks.png",successful,errorMSG);
    } else if (ui->PBplatform3->isChecked()){
        makePlatform(label,":/images/platformbox.png",successful,errorMSG);
    } else if (ui->PBplatform4->isChecked()){
        makePlatform(label,":/images/platformcastle.png",successful,errorMSG);
    } else if (ui->PBplatform5->isChecked()){
        makePlatform(label,":/images/platformdirt.png",successful,errorMSG);
    } else if (ui->PBplatform6->isChecked()){
        makePlatform(label,":/images/platformearth.png",successful,errorMSG);
    } else if (ui->PBplatform7->isChecked()){
        makePlatform(label,":/images/platformsand.png",successful,errorMSG);
    } else if (ui->PBplatform8->isChecked()){
        makePlatform(label,":/images/platformsnow.png",successful,errorMSG);
    } else if (ui->PBplatform9->isChecked()){
        makePlatform(label,":/images/platformstone.png",successful,errorMSG);
    } else if (ui->PBenemyFly->isChecked()) {
		label->setGeometry(x,y,42,42);
        label->setPixmap(QPixmap(":/images/flyingrobot.png").scaled(42,42));
        label->type = "flyingenemy";
        label->file = ":/images/flyingrobot.png";
    } else if (ui->PBenemyGround->isChecked()) {
		label->setGeometry(x,y,42,48);
        label->setPixmap(QPixmap(":/images/groundrobot.png").scaled(42,48));
        label->type = "enemy";
        label->file = ":/images/groundrobot.png";
    } else if (ui->PBobject1->isChecked()) {
        makeDecor(label,":/images/objectarrowsign.png",50,50);
    } else if (ui->PBobject2->isChecked()) {
        makeDecor(label,":/images/objectexitsign.png",50,50);
    } else if (ui->PBobject3->isChecked()) {
        makeDecor(label,":/images/objecthill.png",30,75);
    } else if (ui->PBobject4->isChecked()) {
        makeDecor(label,":/images/objecttorch.png",25,25);
    } else if (ui->PBobject5->isChecked()) {
        makeDecor(label,":/images/objectwindow.png",70,70);
    } else if (ui->PBitem1->isChecked()) {
        makeItem(label,":/images/powerjump.png", "jump");
    } else if (ui->PBitem2->isChecked()) {
        makeItem(label,":/images/powerscore.png", "score");
    } else if (ui->PBitem3->isChecked()) {
        makeItem(label,":/images/powershield.png", "shield");
    } else if (ui->PBitem4->isChecked()) {
        makeItem(label,":/images/powerspeed.png", "speed");
    }


    if (successful == false) {
        QMessageBox::warning(this,"Error", errorMSG);
        label->deleteLater();
        return;
    }
    label->show();

    for (int i = 0; i < this->children().size(); ++i){
        QPushButton * current = dynamic_cast<QPushButton *>(this->children().at(i));
        if (current != NULL){
            current->setChecked(false);
        }
    }
    ui->LExRange->setText("");
    ui->LEyRange->setText("");
    ui->LExSpeed->setText("");
    ui->LEySpeed->setText("");
    ui->LEwidth->setText("");
    ui->LEheight->setText("");
}

void MapMaker::on_PBresize_clicked()
{
    int newX = 0;
    int newY = 0;
    bool sucessful = true;
    if (ui->LEresizeX->text().trimmed() != "" && ui->LEresizeY->text().trimmed() != ""){
        newX = ui->LEresizeX->text().toInt(&sucessful);
        newY = ui->LEresizeY->text().toInt(&sucessful);
    } else if (ui->LEresizeX->text().trimmed() != "") {
        newX = ui->LEresizeX->text().toInt(&sucessful);
        newY = ui->QWworld->size().height();
    } else if (ui->LEresizeY->text().trimmed() != "") {
        newX = ui->QWworld->size().width();
        newY = ui->LEresizeY->text().toInt(&sucessful);
    } else {
        QMessageBox::warning(this,"ERROR", "Please enter an X and/or Y value.");
        ui->LEresizeX->setText("");
        ui->LEresizeY->setText("");
        return;
    }
    if(!sucessful){
        QMessageBox::warning(this,"ERROR", "Please enter an intiger for the X and/or Y value.");
        ui->LEresizeX->setText("");
        ui->LEresizeY->setText("");
        return;
    }
    if(newX < 944 || newY < 688) {
        QMessageBox::warning(this,"ERROR", "The game size must be at least 944x688.");
        ui->LEresizeX->setText("");
        ui->LEresizeY->setText("");
        return;
    }
    ui->QWworld->resize(newX,newY);
    ui->LBsize->setText(QString::fromStdString(to_string(ui->QWworld->size().width())) + "x" + QString::fromStdString(to_string(ui->QWworld->size().height())));
    ui->LEresizeX->setText("");
    ui->LEresizeY->setText("");
}

void MapMaker::on_PBsetTime_clicked()
{
    bool sucess = true;
    if (ui->LEtime->text().trimmed() != "") {
        int newTime = 0;
        newTime = ui->LEtime->text().toInt(&sucess);
        if(!sucess) {
            QMessageBox::warning(this,"ERROR", "Please enter an integer.");
            ui->LEtime->setText("");
            return;
        }
        if(newTime < 30){
            QMessageBox::warning(this,"ERROR", "The level must have at least a 30 second time limit.");
            ui->LEtime->setText("");
            return;
        }
        time = newTime;
        ui->LBtime->setText(QString::fromStdString(std::to_string(time)));
        ui->LEtime->setText("");
    }
}

void MapMaker::on_Save_clicked()
{
	if (ui->LEfilename->text().trimmed() == "") {
        QMessageBox::warning(this,"Error","supply a file name please.");
        return;
    }
	string filename = "data/" + (ui->LEfilename->text().toStdString()) + ".lv";
    fstream stream;
    stream.open(filename,ios::out);
    stream << ui->LEfilename->text().toStdString() << endl;
    stream << time << "," << time << endl;
    stream << ui->QWworld->geometry().width() << "," << ui->QWworld->geometry().height() << endl;
    stream << startX << "," << startY << endl;
    stream << 0 << endl;
    stream << ":/images/easybackground.png" << endl;
    bool containPlayer = false;
    bool containEnd = false;
    for (int i = 0; i < ui->QWworld->children().size(); ++i) {
        MovableLabel * current = dynamic_cast<MovableLabel*>(ui->QWworld->children().at(i));
        if (current != NULL) {
            if (current->type == "player") {
                containPlayer = true;
                QRect thisone = current->geometry();
                stream << current->type << "," << thisone.x() << "," << thisone.y() << "," << thisone.width() << ","
                       << thisone.height() << "," << current->file << "," << thisone.x() << "," << thisone.y()
					   << ",0,0,3,false,0,false,0,false,0,false,0" << endl;
            } else if (current->type == "endGame") {
                containEnd = true;
            }
        }
    }
    if (!containPlayer) {
        QMessageBox::warning(this,"ERROR", "You must add a player to the world.");
        return;
    }
    if (!containEnd) {
        QMessageBox::warning(this, "ERROR", "You must have at least one end game object.");
        return;
    }
    for (int i = 0; i < ui->QWworld->children().size(); ++i) {
        MovableLabel * current = dynamic_cast<MovableLabel*>(ui->QWworld->children().at(i));
        if (current != NULL) {
            if (current->type == "player"){
                continue;
            }
            QRect thisone = current->geometry();
            stream << current->type << "," << thisone.x() << "," << thisone.y() << "," << thisone.width() << ","
                      << thisone.height() << "," << current->file;
            if (current->type == "platform") {
                stream << "," << current->xSpeed << "," << current->ySpeed << "," << current->xRange << ","
                       << current->yRange << "," << thisone.x() << "," << thisone.y();
            } else if (current->type == "endGame") {
                stream << ",0,0";
            } else if (current->type == "collectible") {
                stream << "," << current->collectableType << ",true";
            } else if (current->type == "enemy" || current->type == "flyingenemy") {
                stream << ",true," << thisone.x() << "," << thisone.y();
            } else if (current->type == "coin") {
                stream << ",50,true";
            }
            stream << endl;
        }
    }
    stream.close();
    QMessageBox::information(this, "Success", "Your level has been saved! Go to the Title Screen, type in your level name, and click \"Other Map\" to play your new level.");
}

void MapMaker::on_any_click()
{
    QPushButton* send = dynamic_cast<QPushButton*>(sender());
    if(!send->isCheckable()) {
        return;
    }
    for(int i = 0; i < this->children().size(); ++i){
        QPushButton* button = dynamic_cast<QPushButton*>(children().at(i));
        if (button != NULL) {
            if (button->isCheckable()) {
                if (button->geometry().x() != send->geometry().x()) {
                    button->setChecked(false);
                }
            }
        }
    }
}
