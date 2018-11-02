#include "editdialog.h"
#include "ui_editdialog.h"
#include "sqliteclient.hpp"

EditDialog::EditDialog(QWidget *parent, uint8_t tabIndex, const char* record) :
	QDialog(parent),
	ui(new Ui::EditDialog)
{
	ui->setupUi(this);
	this->tabIndex=tabIndex;
	QString paramStr=QString::fromLatin1(record);
	QStringList params=paramStr.split("|");
	for(uint8_t i=0;i<params.size();i++)
		params[i]=QString::fromUtf8(params[i].toLatin1());
	//необходимость этих преобразований скрыта от меня, но без этого он выводи крокозябры - видимо не умеет работать с юникодом
	//ui->leName->setText(QString::fromUtf8(params.at(0).toAscii()));
	ui->leName->setText(params.at(0));
	SqliteClient *client=new SqliteClient("SchwarzeFeder.db");
	switch(tabIndex)
	{
	case 0:
	case 1:
	{
		dsbVal=new QDoubleSpinBox();
		dsbVal->setMaximum(9999999);
		dsbVal->setValue(params.at(1).toDouble());
		ui->formLayout->addRow("Price", dsbVal);
		teDescr=new QTextEdit();
		teDescr->setText(params.at(2));
		ui->formLayout->addRow("Description", teDescr);
		deDate=new QDateEdit();
		deDate->setDisplayFormat("yyyy-MM-dd");
		deDate->setDate(QDate::fromString(params.at(3),"yyyy-MM-dd"));
		ui->formLayout->addRow("Date", deDate);
		cbCategory=new QComboBox();

		auto *vec=new std::vector<std::string>();
		client->connect();
		client->executeQuery("SELECT name FROM categories", *vec);
		QStringList categories;
		for(uint16_t i=0;i<vec->size();i++)
			categories.append(QString::fromUtf8(QString::fromStdString(vec->at(i)).toLatin1()));
		cbCategory->addItems(categories);
		uint16_t i=0;
		while(categories.at(i)!=params.at(4))
			i++;
		cbCategory->setCurrentIndex(i);
		ui->formLayout->addRow("Category", cbCategory);
		connect(cbCategory,  SIGNAL(currentIndexChanged(int)), this, SLOT(selectCategory(int)));
		client->executeQuery("SELECT shopAvailable FROM categories", *vec);
		if(vec->at(i)=="1")
		{
			createCbShop();
			lShop=new QLabel("Shop name");
			ui->formLayout->addRow(lShop, cbShop);
			i=0;
			while(cbShop->itemText(i)!=params.at(5))
				i++;
			cbShop->setCurrentIndex(i);
		}
		delete client;
		delete vec;
		break;
	}
		//от сих и далее ничего не протестировано. возможно несоответствие элементов массива и интерфейса
	case 2:
	{
		chShopFl=new QCheckBox();
		ui->formLayout->addRow("Shop flag", chShopFl);
		if(params.at(1)=="1")
			chShopFl->setChecked(true);
		break;
	}
	case 4:
	{
		dsbInd=new QDoubleSpinBox();
		dsbInd->setMaximum(9999999);
		dsbInd->setDecimals(5);
		dsbInd->setValue(params.at(1).toDouble());
		ui->formLayout->addRow("Convertion index", dsbInd);
		chMainCurrency=new QCheckBox();
		ui->formLayout->addRow("Main flag", chMainCurrency);
		if(params.at(2)=="1")
			chMainCurrency->setChecked(true);
	}
	}

}

void EditDialog::createCbShop()
{
	cbShop=new QComboBox();
	QStringList list;
	SqliteClient *client=new SqliteClient("SchwarzeFeder.db");
	client->connect();
	auto *vec=new std::vector<std::string>();
	client->executeQuery("SELECT name FROM shops", *vec);
	for(uint16_t i=0;i<vec->size();i++)
		list.append(QString::fromUtf8(QString::fromStdString(vec->at(i)).toLatin1()));
	delete vec;
	client->closeConnection();
	delete client;
	cbShop->addItems(list);
	shopActive=true;
}

void EditDialog::selectCategory(int index)
{
	SqliteClient *client=new SqliteClient("SchwarzeFeder.db");
	client->connect();
	auto *vec=new std::vector<std::string>();
	QString query="SELECT shopAvailable FROM categories WHERE id="+QString::number(index+1);
	client->executeQuery(query.toUtf8(), *vec);
	if(vec->at(0)=="1")
		if(!cbShop)
		{
			createCbShop();
			lShop=new QLabel("Shop name");
			ui->formLayout->addRow(lShop, cbShop);
		}
		else
		{
			cbShop->show();
			shopActive=true;
			lShop->show();
		}
	else
		if(cbShop&&cbShop->isVisible())
		{
			lShop->hide();
			shopActive=false;
			cbShop->hide();
		}
	client->closeConnection();
	delete client;
}

QStringList EditDialog::returnParams()
{
	QStringList res;
	res.append(ui->leName->text());
	switch(tabIndex)
	{
	case 0:
	case 1:
	{
		res.append(QString::number(dsbVal->value()));
		res.append(teDescr->document()->toPlainText());
		res.append(deDate->text());
		//комбобоксы считают с нуля, а в базе записи с единицы
		res.append(QString::number(cbCategory->currentIndex()+1));
		if(cbShop&&shopActive)
			res.append(QString::number(cbShop->currentIndex()+1));
		else
			res.append("1");
		break;
	}
	case 2:
	{
		res.append(chShopFl->isChecked()?"1":"0");
		break;
	}
	case 4:
	{
		res.append(QString::number(dsbInd->value()));
		res.append(chMainCurrency->isChecked()?"1":"0");
		break;
	}
	}
	return res;
}

EditDialog::~EditDialog()
{
	if(lShop) delete lShop;
	if(teDescr) delete teDescr;
	if(deDate) delete deDate;
	if(dsbVal) delete dsbVal;
	if(dsbInd) delete dsbInd;
	if(chShopFl) delete chShopFl;
	if(chMainCurrency) delete chMainCurrency;
	if(cbCategory) delete cbCategory;
	if(cbShop) delete cbShop;
	if(cbCurrency) delete cbCurrency;
	delete ui;
}
