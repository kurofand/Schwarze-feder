#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectdialog.h"
#include "filterdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->buttonEnabled(disableAll);
	//настройки отображения для виджета-таблицы
	ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	tableEditor=new TableEditor(*ui->tableWidget);


	connect(this, SIGNAL(executeQuery(QString*)), tableEditor, SLOT(executeQuery(QString*)));
	connect(tableEditor, SIGNAL(buttonEnabled(uint8_t)), this, SLOT(buttonEnabled(uint8_t)));
	connect(this, SIGNAL(setCurrentQuery(QString*,uint8_t)), tableEditor, SLOT(setCurrentQuery(QString*,uint8_t)));
	connect(ui->aSelect, SIGNAL(triggered(bool)), tableEditor, SLOT(selectTable()));
	connect(ui->aRefresh, SIGNAL(triggered(bool)), tableEditor, SLOT(refreshTable()));
	connect(ui->aFilter, SIGNAL(triggered(bool)), tableEditor, SLOT(setFilter()));
	connect(ui->aAdd, SIGNAL(triggered(bool)), tableEditor, SLOT(insertTable()));
	connect(ui->aEdit, SIGNAL(triggered(bool)), tableEditor, SLOT(editTable()));
	connect(ui->aDelete, SIGNAL(triggered(bool)), tableEditor, SLOT(deleteTable()));
	connect(ui->aExit, SIGNAL(triggered(bool)), this, SLOT(close()));

	//tableEditor->clientConnect();
	//интерфейсный костыль
	ui->wSettings->setVisible(false);
	ui->statusBar->showMessage(QString::fromAscii("Connection status: ")+QString::fromAscii(tableEditor->clientConnect()?"connected":"disconnected"));
}
void MainWindow::on_pbRunSQL_clicked()
{
	QString query=ui->lineEdit->text();
	query=query.toUtf8();
	emit(executeQuery(&query));
}

void MainWindow::close()
{
	QApplication::quit();
}

void MainWindow::buttonEnabled(uint8_t ind)
{
	//qDebug()<<"not connected";
	switch(ind)
	{
	case disableAll:
	{
		ui->aSelect->setEnabled(false);
		ui->aRefresh->setEnabled(false);
		ui->aFilter->setEnabled(false);
		ui->aAdd->setEnabled(false);
		ui->aEdit->setEnabled(false);
		ui->aDelete->setEnabled(false);
		break;
	}
	case enableSelect:
	{
		ui->aSelect->setEnabled(true);
		break;
	}
	case enableFilter:
	{
		ui->aRefresh->setEnabled(true);
		ui->aFilter->setEnabled(true);
		break;
	}
	case enableAdd:
	{
		ui->aAdd->setEnabled(true);
		break;
	}
	case enableEditDelete:
	{
		ui->aEdit->setEnabled(true);
		ui->aDelete->setEnabled(true);
		break;
	}
	case disableEditDelete:
	{
		ui->aEdit->setEnabled(false);
		ui->aDelete->setEnabled(false);
		break;
	}
	}
}


MainWindow::~MainWindow()
{
	delete tableEditor;
	delete ui;
}
