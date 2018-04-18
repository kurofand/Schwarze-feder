#include "selectdialog.h"
#include "ui_selectdialog.h"

SelectDialog::SelectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectDialog)
{
	ui->setupUi(this);
	for(uint8_t i=0;i<6;i++)
	{
		boxes[i]=new QCheckBox();
		ui->vlElements->addWidget(boxes[i]);
		//boxes[i]->hide();
		boxes[i]->setVisible(false);
	}

	connect(ui->cbTables, SIGNAL(currentIndexChanged(int)), this, SLOT(tableSelected(int)));

}

void SelectDialog::showBoxes(uint8_t boxCount)
{
	for(uint8_t i=0;i<boxCount;i++)
		//boxes[i]->show();
		boxes[i]->setVisible(true);
}

void SelectDialog::tableSelected(int index)
{

	for(uint8_t i=0;i<6;i++)
		//boxes[i]->hide();
		boxes[i]->setVisible(false);
	switch(index)
	{
	case 0:
	{
		showBoxes(6);
		boxes[0]->setText("Name");
		boxes[1]->setText("Value");
		boxes[2]->setText("Description");
		boxes[3]->setText("Date");
		boxes[4]->setText("Category");
		boxes[5]->setText("Shop");
		break;
	}
	case 1:
	{
		showBoxes(4);
		boxes[0]->setText("Name");
		boxes[1]->setText("Value");
		boxes[2]->setText("Description");
		boxes[3]->setText("Date");
		break;

	}
	case 2:
	{
		showBoxes(2);
		boxes[0]->setText("Name");
		boxes[1]->setText("Shop flag");
		break;
	}
	case 3:
	{
		showBoxes(1);
		boxes[0]->setText("Name");
	}
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
	for(uint8_t i=0;i<6;i++)
		if(boxes[i]->isVisible())
			res.append(QString::fromAscii(boxes[i]->checkState()==0?"0":"1"));
	return res;
}

SelectDialog::~SelectDialog()
{
	//массив удалить всем скопом нельзя, приходится грохать по одному
	for(uint8_t i=0;i<6;i++)
		delete boxes[i];
	delete ui;
}
