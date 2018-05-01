#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditDialog(QWidget *parent = 0, uint8_t tabIndex=0, const char* record="");
	QStringList returnParams();
	~EditDialog();

private:
	Ui::EditDialog *ui;
	QLabel *lShop=nullptr;
	QTextEdit *teDescr=nullptr;
	QDateEdit *deDate=nullptr;
	QDoubleSpinBox *dsbVal=nullptr, *dsbInd=nullptr;
	QCheckBox *chShopFl=nullptr, *chMainCurrency=nullptr;
	QComboBox *cbCategory=nullptr, *cbShop=nullptr, *cbCurrency=nullptr;
	uint8_t tabIndex;
	bool shopActive=false;
	void createCbShop();
private slots:
	void selectCategory(int index);
};

#endif // EDITDIALOG_H
