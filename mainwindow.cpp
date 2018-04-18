#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertdialog.h"
#include "selectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	tableEditor=new TableEditor(*ui->tableWidget);

	connect(this, SIGNAL(executeQuery(QString*)), tableEditor, SLOT(executeQuery(QString*)));
	connect(ui->pbRefresh, SIGNAL(clicked(bool)), tableEditor, SLOT(refreshTable()));
}
void MainWindow::on_pbRunSQL_clicked()
{
	QString query=ui->lineEdit->text();
	emit(executeQuery(&query));
}

void MainWindow::on_pbSelect_clicked()
{
	SelectDialog *dialog=new SelectDialog();
	QStringList res;
	if(dialog->exec())
	{
		res=dialog->returnParams();
	}
	delete dialog;
}

void MainWindow::on_pbAdd_clicked()
{
	InsertDialog *dialog=new InsertDialog();
	QStringList res;
	if(dialog->exec())
	{
		res=dialog->returnParams();
		QStringList colNames;
		//сверху лежит название таблицы, поэтому начинаем тут с одного
		for(uint8_t i=1;i<res.size();i++)
			colNames.append(res.at(i).split("|")[0]);
		QString query;
		switch(res.at(0).toInt())
		{
		case 0:
			query="INSERT INTO expenses(";
			for(uint8_t i=0;i<colNames.size();i++)
				if(i!=colNames.size()-1)
					query.append(colNames.at(i)+", ");
				else
					query.append(colNames.at(i)+") VALUES(");
			for(uint8_t i=1;i<res.size();i++)
				if(i!=res.size()-1)
					query.append(res.at(i).split("|")[1]+", ");
				else
					query.append(res.at(i).split("|")[1]+")");
			break;
		case 1:
			query="INSERT INTO categories(name, shopAvailable) VALUES(";
			for(uint8_t i=1;i<res.size();i++)
				if(i!=res.size()-1)
					query.append(res.at(i).split("|")[1]+", ");
				else
					query.append(res.at(i).split("|")[1]+")");
			break;
		case 2:
			query="INSERT INTO shops(name) VALUES("+res.at(1).split("|")[1]+")";
			break;
		case 3:
			query="INSERT INTO currency(name, ind, mainFlag) VALUES(";
			for(uint8_t i=1;i<res.size();i++)
				if(i!=res.size()-1)
					query.append(res.at(i).split("|")[1]+", ");
				else
					query.append(res.at(i).split("|")[1]+")");
			break;
		}
		std::vector<std::string> *vec=new std::vector<std::string>();
		MySQLClient *client;
		std::string params[4];
		std::ifstream ini;
		ini.open("connect.ini");
		if(ini.is_open())
		{
			for(uint8_t i=0;i<4;i++)
				ini>>params[i];
			client=new MySQLClient(params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str());
			client->connect();
			client->executeQuery(query.toUtf8(), *vec);
			client->closeConnection();

		}
		delete vec;

	}
	delete dialog;
}

MainWindow::~MainWindow()
{
	delete tableEditor;
	delete ui;
}
