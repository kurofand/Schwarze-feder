#include "currencydialog.h"
#include "ui_currencydialog.h"

CurrencyDialog::CurrencyDialog(QWidget *parent, std::vector<std::string> *vec) :
	QDialog(parent),
	ui(new Ui::CurrencyDialog)
{
	ui->setupUi(this);
	QStringList names;
	indexes=new std::vector<double>();
	for(uint16_t i=0;i<vec->size();i++)
	{
		QString buf=vec->at(i).c_str();
		buf=buf.fromUtf8(buf.toAscii());
		names.append(buf.split("|")[0]);
		indexes->push_back(buf.split("|")[1].toDouble());
	}
	ui->cbCurrency->addItems(names);

}

QStringList CurrencyDialog::changeCurrency(std::vector<std::string> *vals)
{
	double currs[vals->size()];
	uint8_t currIds[vals->size()];
	for(uint16_t i=0;i<vals->size();i++)
	{
		QString buf=vals->at(i).c_str();
		buf=buf.split("|")[0];
		currs[i]=buf.toDouble();
		buf=vals->at(i).c_str();
		buf=buf.split("|")[1];
		currIds[i]=buf.toInt();
	}
	uint8_t resCol=ui->cbCurrency->currentIndex();
	double resIndex=indexes->at(resCol);
	for(uint16_t i=0;i<vals->size();i++)
		if(currIds[i]!=resCol+1)
		{
			currs[i]=indexes->at(currIds[i]-1)/resIndex*currs[i];
		}
	QStringList res;
	for(uint16_t i=0;i<vals->size();i++)
		res.append(QString::number(currs[i]));
	return res;

}

uint8_t CurrencyDialog::returnSelectedId()
{
	uint8_t res=ui->cbCurrency->currentIndex();
	return res;
}

CurrencyDialog::~CurrencyDialog()
{
	delete indexes;
	delete ui;
}
