#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class SelectDialog;
}

class SelectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectDialog(QWidget *parent = 0);
	QStringList returnParams();
	~SelectDialog();

private:
	Ui::SelectDialog *ui;
	QCheckBox *boxes[6];
	//похоже что в момент нажатия кнопки "ок" комбобоксы перестают отрисовываться
	//и поэтому я схораняю значения видимости в булевский массив
	bool boxVis[6];
	void showBoxes(uint8_t boxCount);
private slots:
	void tableSelected(int index);
};

#endif // SELECTDIALOG_H
