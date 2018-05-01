#include "insertdialog.h"
#include "ui_insertdialog.h"
//#include "mysqlClient.hpp"
#include "sqliteclient.hpp"
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
	ui->leName->setText("");

	switch(prIndex)
	{
	case 0:
	{
		lValue->hide();
		dsbValue->hide();
		cbCurrency->hide();
		cbCurrency->setCurrentIndex(-1);
		lCurrency->hide();
		lDescription->hide();
		teDescription->hide();
		lDate->hide();
		deDate->hide();
		lCategory->hide();
		cbCategory->hide();
		cbCategory->setCurrentIndex(-1);
		if(lShop)
			lShop->hide();
		if(cbShop)
			cbShop->hide();
		break;
	}
	case 1:
	{
		lShopAvailable->hide();
		chbShopAvailable->hide();
		break;
	}
	case 3:
	{
		lIndex->hide();
		dsbIndex->hide();
		lMain->hide();
		chbMain->hide();
		break;
	}
	}

	//current expenses index
	switch(index)
	{
	case 0:
	{
		if(!lValue)
		{
			lValue=new QLabel("Cost");
			ui->gridLayout->addWidget(lValue);
		}
		else
			lValue->show();
		if(!dsbValue)
		{
			dsbValue=new QDoubleSpinBox();
			dsbValue->setMaximum(9999999.99);
			ui->gridLayout->addWidget(dsbValue);
		}
		else
			dsbValue->show();
		if(!lCurrency)
		{
			lCurrency=new QLabel("Currency type");
			ui->gridLayout->addWidget(lCurrency);
		}
		else
			lCurrency->show();
		if(!cbCurrency)
		{
			cbCurrency=new QComboBox();
			ui->gridLayout->addWidget(cbCurrency);
			SqliteClient *client=new SqliteClient("SchwarzeFeder.db");
			client->connect();
			std::vector<std::string> *vec=new std::vector<std::string>();
			client->executeQuery("SELECT name FROM currency", *vec);
			QStringList list;
			for(uint16_t i=0;i<vec->size();i++)
			{
				QString str=QString::fromStdString(vec->at(i));
				str=str.fromUtf8(str.toAscii());
				list.append(str);
			}
			cbCurrency->addItems(list);
			client->closeConnection();
			delete vec;
			delete client;
			cbCurrency->setCurrentIndex(-1);
		}
		else
			cbCurrency->show();
		if(!lDescription)
		{
			lDescription=new QLabel("Description");
			ui->gridLayout->addWidget(lDescription);
		}
		else
			lDescription->show();
		if(!teDescription)
		{
			teDescription=new QTextEdit();
			ui->gridLayout->addWidget(teDescription);
		}
		else
			teDescription->show();
		if(!lDate)
		{
			lDate=new QLabel("Date");
			ui->gridLayout->addWidget(lDate);
		}
		else
			lDate->show();
		if(!deDate)
		{
			deDate=new QDateEdit();
			ui->gridLayout->addWidget(deDate);
			deDate->setDisplayFormat("yyyy-MM-dd");
			deDate->setDate(QDate::currentDate());
		}
		else
			deDate->show();
		if(!lCategory)
		{
			lCategory=new QLabel("Category");
			ui->gridLayout->addWidget(lCategory);
		}
		else
			lCategory->show();
		if(!cbCategory)
		{
			cbCategory=new QComboBox();
			ui->gridLayout->addWidget(cbCategory);
			SqliteClient *client;
			client=new SqliteClient("SchwarzeFeder.db");
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
				delete vec;
				delete client;
			//}
			cbCategory->setCurrentIndex(-1);
			connect(cbCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectCategory(QString)));
		}
		else
			cbCategory->show();
		break;
	}
	case 1:
	{
		if(!lShopAvailable)
		{
			lShopAvailable=new QLabel("Connect with shop");
			ui->gridLayout->addWidget(lShopAvailable);
		}
		else
			lShopAvailable->show();
		if(!chbShopAvailable)
		{
			chbShopAvailable=new QCheckBox();
			ui->gridLayout->addWidget(chbShopAvailable);
		}
		else
			chbShopAvailable->show();
		break;
	}
	case 3:
	{
		if(!lIndex)
		{
			lIndex=new QLabel("Convert index");
			ui->gridLayout->addWidget(lIndex);
		}
		else
			lIndex->show();
		if(!dsbIndex)
		{
			dsbIndex=new QDoubleSpinBox();
			dsbIndex->setMaximum(9999999.99);
			ui->gridLayout->addWidget(dsbIndex);
		}
		else
			dsbIndex->show();
		if(!lMain)
		{
			lMain=new QLabel("Main currency");
			ui->gridLayout->addWidget(lMain);
		}
		else
			lMain->show();
		if(!chbMain)
		{
			chbMain=new QCheckBox();
			ui->gridLayout->addWidget(chbMain);
		}
		else
			chbMain->show();
		break;
	}

	}
	prIndex=index;
}

void InsertDialog::selectCategory(QString text)
{
	if(text!="")
	{
		//MySQLClient *client;
		SqliteClient *client;
		/*std::ifstream ini;
		std::string params[4];
		ini.open("connect.ini");
		if(ini.is_open())
		{
			for(uint8_t i=0;i<4;i++)
				ini>>params[i];
			client=new MySQLClient(params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str());*/
		client=new SqliteClient("SchwarzeFeder.db");
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
				//обнулим вектор для запроса магазинов
				vec->clear();
				query="SELECT name FROM shops";
				client->executeQuery(query.toUtf8(), *vec);
				QStringList shops;
				for(uint16_t i=0;i<vec->size();i++)
				{
					QString str=QString::fromStdString(vec->at(i));
					str=str.fromUtf8(str.toAscii());
					shops.append(str);
				}
				cbShop->addItems(shops);
				cbShop->setCurrentIndex(-1);
			}
			else
			{
				if(lShop&&cbShop)
				{
					lShop->setVisible(false);
					cbShop->setVisible(false);
					cbShop->setCurrentIndex(-1);
					for(uint8_t i=0;i<cbShop->count();i++)
						cbShop->removeItem(i);
				}
			}
			client->closeConnection();
			delete vec;
			delete client;
		//}
	}
}

QStringList InsertDialog::returnParams()
{
	QStringList result;
	result.append(QString::number(ui->cbTable->currentIndex()));
	result.append("name|\""+ui->leName->text()+"\"");
	switch(ui->cbTable->currentIndex())
	{
	case 0:
	{
		if(dsbValue)
			result.append("val|"+QString::number(dsbValue->value()));
		if(cbCurrency)
			result.append("currencyId|"+QString::number(cbCurrency->currentIndex()+1));
		if(teDescription)
			result.append("descr|\""+teDescription->document()->toPlainText()+"\"");
		if(deDate)
			result.append("date|\""+deDate->text()+"\"");
		if(cbCategory)
			result.append("categoryId|"+QString::number(cbCategory->currentIndex()+1));
		if(cbShop)
			result.append("shopId|"+QString::number(cbShop->currentIndex()+1));
		break;
	}
	case 1:
	{
		result.append("shopAvailable|"+QString::fromAscii(chbShopAvailable->checkState()==0?"0":"1"));
		break;
	}
	case 3:
	{
		result.append("ind|"+QString::number(dsbIndex->value()));
		result.append("mainFlag|"+QString::fromAscii(chbMain->checkState()==0?"0":"1"));
		break;
	}
	}

	return result;
}

InsertDialog::~InsertDialog()
{
	//this->deleteWidgets();
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
	if(lIndex) delete lIndex;
	if(dsbIndex) delete dsbIndex;
	if(lMain) delete lMain;
	if(chbMain) delete chbMain;
	if(cbCurrency) delete cbCurrency;
	delete ui;
}
