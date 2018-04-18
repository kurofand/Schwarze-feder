#ifndef INSERTDIALOG_H
#define INSERTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QCheckBox>

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
	QLabel *lValue=nullptr, *lCategory=nullptr, *lDescription=nullptr, *lShop=nullptr, *lDate=nullptr,
	*lIndex=nullptr, *lMain=nullptr, *lShopAvailable=nullptr;
	QDoubleSpinBox *dsbValue=nullptr, *dsbIndex=nullptr;
	QComboBox *cbCategory=nullptr, *cbShop=nullptr;
	QTextEdit *teDescription=nullptr;
	QDateEdit *deDate=nullptr;
	QCheckBox *chbMain=nullptr, *chbShopAvailable=nullptr;

private:
	int8_t prIndex=-1;
private slots:
	void selectTable(int index);
	void selectCategory(QString text);
};

#endif // INSERTDIALOG_H
