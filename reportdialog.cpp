#include "reportdialog.h"
#include "ui_reportdialog.h"

ReportDialog::ReportDialog(QWidget *parent, const QStringList *params, bool year) :
	QDialog(parent),
	ui(new Ui::ReportDialog)
{
	ui->setupUi(this);
	ui->deMonth->setDate(QDate::currentDate());
	QString buf=params->at(0);
	QStringList list=buf.split("|");
	categories=new QStringList(list);
	cBoxes=new QCheckBox[categories->size()]();
	for(uint16_t i=0;i<categories->size();i++)
	{
		cBoxes[i].setText(categories->at(i));
		ui->formLayout->addWidget(&cBoxes[i]);
	}
	buf=params->at(1);
	list=buf.split("|");
	currency=new QStringList(list);
	cbCurrency=new QComboBox(this);
	cbCurrency->addItems(*currency);
	ui->formLayout->addRow("Currency", cbCurrency);
	if(year)
		ui->deMonth->setDisplayFormat("yyyy");
}


QStringList ReportDialog::returnParams()
{
	QStringList res;
	QDate date;
	date=date.fromString(ui->deMonth->text(), "MMMM yyyy");
	//res.append(ui->deMonth->text());
	res.append(date.toString("MMMM yyyy"));
	//res.append(date.toString("dd MM yyyy"));
	QString buf;
	for(uint16_t i=0;i<categories->size();i++)
		buf=buf+(cBoxes[i].checkState()==0?"0":"1");
	res.append(buf);
	res.append(QString::number(cbCurrency->currentIndex()));
	return res;

}

ReportDialog::~ReportDialog()
{
	delete[] cBoxes;
	delete categories;
	delete cbCurrency;
	delete currency;
	if(dialogResult)  delete dialogResult;
	delete ui;
}
