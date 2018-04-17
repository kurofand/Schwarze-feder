#include "insertdialog.h"
#include "ui_insertdialog.h"
#include "mysqlClient.hpp"
/*koslyli i kopipasta inside*/
InsertDialog::InsertDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InsertDialog)
{
	ui->setupUi(this);
	ui->lName->setVisible(false);
	ui->leName->setVisible(false);
	connect(ui->cbTable, SIGNAL(currentIndexChanged(int)), this, SLOT(selectTable(int)));
}

void InsertDialog::selectTable(int index)
{
	if(!(ui->lName->isVisible()&&ui->leName->isVisible()))
	{
			ui->lName->setVisible(true);
			ui->leName->setVisible(true);
	}
	//current expenses index
	if(index==0)
	{
		lValue=new QLabel("Cost");
		ui->gridLayout->addWidget(lValue);
		dsbValue=new QDoubleSpinBox();
		dsbValue->setMaximum(9999999.99);
		ui->gridLayout->addWidget(dsbValue);
		lDescription=new QLabel("Description");
		ui->gridLayout->addWidget(lDescription);
		teDescription=new QTextEdit();
		ui->gridLayout->addWidget(teDescription);
		lDate=new QLabel("Date");
		ui->gridLayout->addWidget(lDate);
		deDate=new QDateEdit();
		ui->gridLayout->addWidget(deDate);
		lCategory=new QLabel("Category");
		ui->gridLayout->addWidget(lCategory);
		cbCategory=new QComboBox();
		ui->gridLayout->addWidget(cbCategory);
		MySQLClient *client;
		std::ifstream ini;
		std::string params[4];
		ini.open("connect.ini");
		if(ini.is_open())
		{
			for(uint8_t i=0;i<4;i++)
				ini>>params[i];
			client=new MySQLClient(params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str());
			client->connect();
			std::vector<std::string> *vec=new std::vector<std::string>();
			client->executeQuery("SELECT name FROM categories", *vec);
			QStringList list;
			for(uint8_t i=0;i<vec->size();i++)
			{
				QString str=QString::fromStdString(vec->at(i));
				str=str.fromUtf8(str.toAscii());
				list.append(str);
			}
			cbCategory->addItems(list);
			client->closeConnection();
			delete client;
		}
		cbCategory->setCurrentIndex(-1);
		connect(cbCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectCategory(QString)));


	}

}

void InsertDialog::selectCategory(QString text)
{
	MySQLClient *client;
	std::ifstream ini;
	std::string params[4];
	ini.open("connect.ini");
	if(ini.is_open())
	{
		for(uint8_t i=0;i<4;i++)
			ini>>params[i];
		client=new MySQLClient(params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str());
		client->connect();
		std::vector<std::string> *vec=new std::vector<std::string>();
		QString query="SELECT shopAvailable FROM categories WHERE name=\""+text+"\"";
		client->executeQuery(query.toUtf8(), *vec);
		if(vec->at(0)=="1")
		{
			lShop=new QLabel("Shop name");
			ui->gridLayout->addWidget(lShop);
			cbShop=new QComboBox();
			ui->gridLayout->addWidget(cbShop);
		}
		else
		{
			if(lShop&&cbShop)
			{
				lShop->setVisible(false);
				cbShop->setVisible(false);
				cbShop->setCurrentIndex(-1);
			}
		}
		client->closeConnection();
		delete vec;
		delete client;
	}

}

QStringList InsertDialog::returnParams()
{
	QStringList result;
	result.append(QString::number(ui->cbTable->currentIndex()));
	result.append("name|\""+ui->leName->text()+"\"");
	if(dsbValue)
		result.append("val|"+QString::number(dsbValue->value()));
	if(teDescription)
		result.append("descr|\""+teDescription->document()->toPlainText()+"\"");
	if(deDate)
		result.append("date|\""+deDate->text()+"\"");
	if(cbCategory)
		result.append("categoryId|"+QString::number(cbCategory->currentIndex()+1));
	if(cbShop)
		result.append("shopId|"+QString::number(cbShop->currentIndex()+1));
	return result;
}

InsertDialog::~InsertDialog()
{
	if(lValue) delete lValue;
	if(dsbValue) delete dsbValue;
	if(lDescription) delete lDescription;
	if(teDescription) delete teDescription;
	if(lDate) delete lDate;
	if(deDate) delete deDate;
	if(lCategory) delete lCategory;
	if(cbCategory) delete cbCategory;
	if(lShop) delete lShop;
	if(cbShop) delete cbShop;
	delete ui;
}
