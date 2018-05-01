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
	//настройки отображения для виджета-таблицы
	ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableEditor=new TableEditor(*ui->tableWidget);

	connect(this, SIGNAL(executeQuery(QString*)), tableEditor, SLOT(executeQuery(QString*)));
	connect(ui->pbRefresh, SIGNAL(clicked(bool)), tableEditor, SLOT(refreshTable()));
	connect(tableEditor, SIGNAL(buttonEnabled(uint8_t)), this, SLOT(buttonEnabled(uint8_t)));
	connect(this, SIGNAL(setCurrentQuery(QString*,uint8_t)), tableEditor, SLOT(setCurrentQuery(QString*,uint8_t)));
	connect(ui->pbEdit, SIGNAL(clicked(bool)), tableEditor, SLOT(editTable()));
	connect(ui->pbAdd, SIGNAL(clicked(bool)), tableEditor, SLOT(insertTable()));
	connect(ui->pbSelect, SIGNAL(clicked(bool)), tableEditor, SLOT(selectTable()));
}
void MainWindow::on_pbRunSQL_clicked()
{
	QString query=ui->lineEdit->text();
	query=query.toUtf8();
	emit(executeQuery(&query));
}

void MainWindow::on_pbFilter_clicked()
{
	//покаместь индекс ручками
	FilterDialog *dialog=new FilterDialog(0,currentTable);
	if(dialog->exec())
	{
		QString filterQuery=dialog->returnFilterString();
		/*QString query="SELECT expenses.name, descr, val, date, "
					  "categories.name, shops.name "
					  "FROM expenses INNER JOIN categories, shops "
					  "WHERE shopId=shops.id AND categories.id=categoryId AND ("+filterQuery+ ")";*/
		//QString query=" AND (" + filterQuery + ")";
		QString query=filterQuery;
		query=query.toUtf8();
		qDebug()<<query;
		emit(setCurrentQuery(&query, 1));
		//emit(executeQuery(&query));
	}
	delete dialog;
}

void MainWindow::buttonEnabled(uint8_t ind)
{
	//qDebug()<<Q_FUNC_INFO;
	qDebug()<<"not connected";
	switch(ind)
	{
	//all disabled
	case 0:
	{
		ui->pbAdd->setEnabled(false);
		ui->pbFilter->setEnabled(false);
		ui->pbRefresh->setEnabled(false);
		ui->pbRunSQL->setEnabled(false);
		ui->pbSelect->setEnabled(false);
	}
	}
}


MainWindow::~MainWindow()
{
	delete tableEditor;
	delete ui;
}
