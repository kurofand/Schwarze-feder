#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	std::string params[4];
	std::ifstream ini;
	ini.open("connect.ini");
	if(ini.is_open())
	{
		for(uint8_t i=0;i<4;i++)
			ini>>params[i];
		ini.close();

	}
	client=new MySQLClient(params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str());
	client->connect();

}
void MainWindow::on_pushButton_clicked()
{
	std::vector<std::string> *res=new std::vector<std::string>();
	QString query=ui->lineEdit->text().toUtf8();
	client->executeQuery(query.toAscii(), *res);

	if(query.split(" ")[0]=="SELECT")
		this->reloadTable(res);

	delete res;
}

void MainWindow::reloadTable(std::vector<std::string> *vec)
{
	ui->tableWidget->setRowCount(vec->size());
	for(uint16_t i=0;i<vec->size();i++)
	{
		QString str=QString::fromStdString(vec->at(i));
		str=str.fromUtf8(str.toAscii());
		QStringList row=str.split("|");
		if(ui->tableWidget->colorCount()!=row.size())
			ui->tableWidget->setColumnCount(row.size());
		for(uint8_t j=0;j<row.size();j++)
			ui->tableWidget->setItem(i,j,new QTableWidgetItem(row.at(j)));
	}
//это растягивает столбец чтобы отображался весь контент
	ui->tableWidget->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
	delete client;
	delete ui;
}
