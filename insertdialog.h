#ifndef INSERTDIALOG_H
#define INSERTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>

namespace Ui {
class InsertDialog;
}

class InsertDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InsertDialog(QWidget *parent = 0);
	QStringList returnParams();
	~InsertDialog();

private:
	Ui::InsertDialog *ui;
	//objects for dynamic interface
	QLabel *lValue=nullptr, *lCategory=nullptr, *lDescription=nullptr, *lShop=nullptr, *lDate=nullptr;
	QDoubleSpinBox *dsbValue=nullptr;
	QComboBox *cbCategory=nullptr, *cbShop=nullptr;
	QTextEdit *teDescription=nullptr;
	QDateEdit *deDate=nullptr;


private slots:
	void selectTable(int index);
	void selectCategory(QString text);
};

#endif // INSERTDIALOG_H
