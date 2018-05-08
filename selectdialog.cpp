#include "selectdialog.h"
#include "ui_selectdialog.h"

SelectDialog::SelectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectDialog)
{
	ui->setupUi(this);
	for(uint8_t i=0;i<boxMaxCount;i++)
	{
		boxes[i]=new QCheckBox();
		ui->vlElements->addWidget(boxes[i]);
		boxes[i]->hide();
		boxVis[i]=boxes[i]->isVisible();

	}

	connect(ui->cbTables, SIGNAL(currentIndexChanged(int)), this, SLOT(tableSelected(int)));

}

void SelectDialog::showBoxes(uint8_t boxCount)
{
	for(uint8_t i=0;i<boxCount;i++)
	{
		boxes[i]->show();
		boxVis[i]=boxes[i]->isVisible();
	}
}

void SelectDialog::tableSelected(int index)
{

	for(uint8_t i=0;i<boxMaxCount;i++)
	{
		boxes[i]->hide();
		boxVis[i]=boxes[i]->isVisible();
	}
	switch(index)
	{
	//base viewer
	case 0:
	{
		showBoxes(boxMaxCount);
		boxes[0]->setText("Name");
		boxes[1]->setText("Value");
		boxes[2]->setText("Currency");
		boxes[3]->setText("Description");
		boxes[4]->setText("Date");
		boxes[5]->setText("Category");
		boxes[6]->setText("Shop");
		break;
	}
	//expenses table
	case 1:
	{
		showBoxes(4);
		boxes[0]->setText("Name");
		boxes[1]->setText("Value");
		boxes[2]->setText("Description");
		boxes[3]->setText("Date");
		break;

	}
	//category table
	case 2:
	{
		showBoxes(2);
		boxes[0]->setText("Name");
		boxes[1]->setText("Shop flag");
		break;
	}
	//shop table
	case 3:
	{
		showBoxes(1);
		boxes[0]->setText("Name");
		break;
	}
	//currency view
	case 4:
	{
		showBoxes(3);
		boxes[0]->setText("Name");
		boxes[1]->setText("Convert index");
		boxes[2]->setText("Main currency flag");
	}
	}
}

QStringList SelectDialog::returnParams()
{
	QStringList res;
	res.append(QString::number(ui->cbTables->currentIndex()));
	for(uint8_t i=0;i<boxMaxCount;i++)
	{
		if(boxVis[i])
			res.append(QString::fromAscii(boxes[i]->checkState()==0?"0":"1"));
	}
	return res;
}

SelectDialog::~SelectDialog()
{
	//массив удалить всем скопом нельзя, приходится грохать по одному
	for(uint8_t i=0;i<boxMaxCount;i++)
		delete boxes[i];
	delete ui;
}
