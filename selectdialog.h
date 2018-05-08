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
	static const uint8_t boxMaxCount=7;
	QCheckBox *boxes[boxMaxCount];
	//похоже что в момент нажатия кнопки "ок" комбобоксы перестают отрисовываться
	//и поэтому я схораняю значения видимости в булевский массив
	bool boxVis[boxMaxCount];
	void showBoxes(uint8_t boxCount);
private slots:
	void tableSelected(int index);
};

#endif // SELECTDIALOG_H
