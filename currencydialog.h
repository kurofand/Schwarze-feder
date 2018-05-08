#ifndef CURRENCYDIALOG_H
#define CURRENCYDIALOG_H

#include <QDialog>

namespace Ui {
class CurrencyDialog;
}

class CurrencyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CurrencyDialog(QWidget *parent = 0, std::vector<std::string> *vec=nullptr);
	QStringList changeCurrency(std::vector<std::string> *vals);
	~CurrencyDialog();

private:
	Ui::CurrencyDialog *ui;
	std::vector<double> *indexes;
};

#endif // CURRENCYDIALOG_H
