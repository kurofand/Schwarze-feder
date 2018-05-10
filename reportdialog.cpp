#include "reportdialog.h"
#include "ui_reportdialog.h"

ReportDialog::ReportDialog(QWidget *parent, const QStringList *params) :
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
}
/*
QDataStream &ReportDialog::returnParams()
{
	QByteArray block;
	//QDataStream res(block, QIODevice::WriteOnly);
	dialogResult=new QDataStream(block, QIODevice::WriteOnly);
	dialogResult<<ui->deMonth->date();
	for(uint16_t i=0;i<categories->size();i++)
		dialogResult<<(cBoxes[i].checkState()==0?0:1);
	dialogResult<<cbCurrency->currentIndex();

	return &dialogResult;
}*/

QDataStream ReportDialog::returnParams()
{
	QByteArray block;
	QDataStream res(&block, QIODevice::WriteOnly);
	return res;
}

/*QStringList ReportDialog::returnParams()
{
	QStringList res;
	return res;

}*/

ReportDialog::~ReportDialog()
{
	delete[] cBoxes;
	delete categories;
	delete cbCurrency;
	delete currency;
	if(dialogResult)  delete dialogResult;
	delete ui;
}
