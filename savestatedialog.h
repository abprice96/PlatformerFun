#ifndef SAVESTATEDIALOG_H
#define SAVESTATEDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class SaveStateDialog;
}

class SaveStateDialog : public QDialog
{
	Q_OBJECT

	bool saving;
	QList<QLabel*> slotLabels;

public:
	explicit SaveStateDialog(QWidget *parent = 0, bool save = true);
	~SaveStateDialog();

	void loadLabels();

	void doIO(int slot);

signals:
	void loadRequested(QString filename);

private slots:
	void on_btnSlot1_clicked();

	void on_btnSlot2_clicked();

	void on_btnSlot3_clicked();

private:
	Ui::SaveStateDialog *ui;
};

#endif // SAVESTATEDIALOG_H
