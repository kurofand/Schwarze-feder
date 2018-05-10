#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ReportDialog(QWidget *parent = 0, const QStringList *params=nullptr);
	//QDataStream &returnParams();
	QDataStream returnParams();
	//QStringList returnParams();
	~ReportDialog();

private:
	Ui::ReportDialog *ui;
	QStringList *categories;
	QStringList *currency;
	QCheckBox *cBoxes;
	QComboBox *cbCurrency=nullptr;
	QDataStream *dialogResult=nullptr;
};

#endif // REPORTDIALOG_H
