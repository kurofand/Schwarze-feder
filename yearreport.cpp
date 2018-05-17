#include "yearreport.h"
#include "ui_yearreport.h"

YearReport::YearReport(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::YearReport)
{
	ui->setupUi(this);
}

YearReport::~YearReport()
{
	delete ui;
}
