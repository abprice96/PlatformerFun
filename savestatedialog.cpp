#include "savestatedialog.h"
#include "ui_savestatedialog.h"

#include <QMessageBox>
#include <QDirIterator>
#include <QDateTime>
#include "loadsave.h"
#include "world.h"
#include "pausescreen.h"

SaveStateDialog::SaveStateDialog(QWidget *parent, bool save) :
	QDialog(parent),
	saving(save),
	ui(new Ui::SaveStateDialog)
{
	ui->setupUi(this);
	slotLabels.append(ui->lblSlot1);
	slotLabels.append(ui->lblSlot2);
	slotLabels.append(ui->lblSlot3);
	if (!saving)
		ui->lblTitle->setText("-Choose a slot to load from-");
	loadLabels();
}

SaveStateDialog::~SaveStateDialog()
{
	delete ui;
}

void SaveStateDialog::loadLabels()
{
	QDirIterator it("data", QDirIterator::NoIteratorFlags);
	while (it.hasNext())
	{
		it.next();
		for (int i = 1; i <= 3; i++)
		{
			QString levelName = QString::fromStdString(World::instance().getLevelName());
			QString suffix = "scores.txt";
			levelName = levelName.left(levelName.length() - suffix.length());
			if (it.fileName() == (levelName + ".save" + QString::number(i)))
			{
				QDateTime dateCreated = it.fileInfo().created();
				slotLabels.at(i - 1)->setText(dateCreated.toString("M-d-yyyy h:m ap"));
			}
		}
	}
}

void SaveStateDialog::on_btnSlot1_clicked()
{
	doIO(1);
}

void SaveStateDialog::on_btnSlot2_clicked()
{
	doIO(2);
}

void SaveStateDialog::on_btnSlot3_clicked()
{
	doIO(3);
}

void SaveStateDialog::doIO(int slot)
{
	QString levelName = QString::fromStdString(World::instance().getLevelName());
	QString suffix = "scores.txt";
	QString filename = "data/" + levelName.left(levelName.length() - suffix.length()) + ".save" + QString::number(slot);

	if (saving)
	{
		LoadSave::instance().save(filename);
	}
	else
	{
		if (slotLabels.at(slot - 1)->text() == "Empty")
		{
			QMessageBox::critical(NULL, "Empty Slot", "Slot " + QString::number(slot) + " is empty.");
			return;
		}
		emit loadRequested(filename);
	}
	deleteLater();
}
