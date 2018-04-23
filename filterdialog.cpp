#include "filterdialog.h"
#include "ui_filterdialog.h"
//bydlocode prodaction presents
FilterDialog::FilterDialog(QWidget *parent, uint8_t tableIndex) :
	QDialog(parent),
	ui(new Ui::FilterDialog)
{
	ui->setupUi(this);
	tIndex=tableIndex;
}



void FilterDialog::on_pbAdd_clicked()
{
	filterLayouts.append(new QVBoxLayout());
	ui->vlElements->addLayout(filterLayouts.back());
	if(filterLayouts.size()>1)
	{
		/*cbCols.append(new QComboBox());
		cbCols.back()->addItems({"AND", "OR"});
		cbCols.back()->setCurrentIndex(0);
		cbCols.back()->setProperty("tag", "logic");
		//ui->vlElements->addWidget(cbCols.back());
		filterLayouts.back()->addWidget(cbCols.back());*/
		cbCombo.append(new QComboBox());
		cbCombo.back()->addItems({"AND", "OR"});
		cbCombo.back()->setCurrentIndex(0);
		cbCombo.back()->setProperty("tag", "logic");
		filterLayouts.back()->addWidget(cbCombo.back());
	}
	cbCols.append(new QComboBox());
	filterLayouts.back()->addWidget(cbCols.back());
	innerLayouts.append(new QFormLayout());
	filterLayouts.back()->addLayout(innerLayouts.back());
	pbDeletes.append(new QPushButton("Delete filter"));
	connect(pbDeletes.back(), SIGNAL(clicked(bool)), this, SLOT(deleteFilter()));
	filterLayouts.back()->addWidget(pbDeletes.back());
	switch(tIndex)
	{
	case 0:
	{
		QStringList sl={"Name", "Value", "Date", "Category", "Shop"};
		cbCols.back()->addItems(sl);
		cbCols.back()->setCurrentIndex(-1);
		cbCols.back()->setProperty("tag","rowName");
		connect(cbCols.back(), SIGNAL(currentIndexChanged(int)), this, SLOT(selectCol(int)));

		break;
	}
	}
}
//пока так, потом править
void FilterDialog::deleteFilter()
{
	for(uint8_t i=0;i<filterLayouts.size();i++)
		if(sender()==pbDeletes.at(i))
		{
			QLayoutItem *child;
			while ((child = filterLayouts.at(i)->takeAt(0)) != 0)
			{
				for(uint8_t j=0;j<cbCols.size();j++)
					if(child->widget()==cbCols.at(j))
						cbCols.removeAt(j);
				for(uint8_t j=0;j<pbDeletes.size();j++)
					if(child->widget()==pbDeletes.at(j))
						pbDeletes.removeAt(j);
				for(uint8_t j=0;j<innerLayouts.size();j++)
					if(child->layout()==innerLayouts.at(j))
					{
						QLayoutItem *innerChild;
						while((innerChild=innerLayouts.at(j)->takeAt(0))!=0)
						{
							for(uint8_t k=0;k<leLines.size();k++)
								if(innerChild->widget()==leLines.at(k))
									leLines.removeAt(k);
							for(uint8_t k=0;k<dsbVals.size();k++)
								if(innerChild->widget()==dsbVals.at(k))
									dsbVals.removeAt(k);
							for(uint8_t k=0;k<deDates.size();k++)
								if(innerChild->widget()==deDates.at(k))
									deDates.removeAt(k);
							for(uint8_t k=0;k<cbCombo.size();k++)
								if(innerChild->widget()==cbCombo.at(k))
									cbCombo.removeAt(k);
							delete innerChild->widget();
							delete innerChild;
							innerChild=nullptr;
						}
						innerLayouts.removeAt(j);
					}
				/*for(uint8_t j=0;j<leLines.size();j++)
					if(child->widget()==leLines.at(j))
						leLines.removeAt(j);
				for(uint8_t j=0;j<dsbVals.size();j++)
					if(child->widget()==dsbVals.at(j))
						dsbVals.removeAt(j);
				for(uint8_t j=0;j<deDates.size();j++)
					if(child->widget()==deDates.at(j))
						deDates.removeAt(j);*/
				//кроме внутреннего есть ещё и внешний, тот где "И"/"ИЛИ"
				for(uint8_t j=0;j<cbCombo.size();j++)
					if(child->widget()==cbCombo.at(j))
						cbCombo.removeAt(j);
				delete child->widget();
				delete child;
				child=nullptr;
			}
			delete filterLayouts.at(i);
			filterLayouts.removeAt(i);
		}
}

void FilterDialog::selectCol(int index)
{
	switch(index)
	{
	case 0:
	case 3:
	case 4:
	{
		leLines.append(new QLineEdit());
		innerLayouts.back()->addWidget(leLines.back());
		break;
	}
	case 1:
	{
		//cbCols.append(new QComboBox());
		//cbCols.back()->setProperty("tag", "compare");
		dsbVals.append(new QDoubleSpinBox());
		//cbCols.back()->addItems({"<", "<=", "=", ">=", ">"});
		cbCombo.append(new QComboBox());
		cbCombo.back()->setProperty("tag", "compare");
		cbCombo.back()->addItems({"<", "<=", "=", ">=", ">"});
		innerLayouts.back()->addRow(cbCombo.back(), dsbVals.back());
		//innerLayouts.back()->addRow(cbCols.back(), dsbVals.back());
		dsbVals.back()->setMaximum(9999999.99);
		break;
	}
	case 2:
	{
		deDates.append(new QDateEdit());
		deDates.back()->setDisplayFormat("yyyy-MM-dd");
		deDates.back()->setDate(QDate::currentDate());
		/*cbCols.append(new QComboBox());
		cbCols.back()->setProperty("tag", "compare");
		cbCols.back()->addItems({"<", "<=", "=", ">=", ">"});
		innerLayouts.back()->addRow(cbCols.back(), deDates.back());*/
		cbCombo.append(new QComboBox());
		cbCombo.back()->setProperty("tag", "compare");
		cbCombo.back()->addItems({"<", "<=", "=", ">=", ">"});
		innerLayouts.back()->addRow(cbCombo.back(), deDates.back());
		break;
	}
	}
}

QString FilterDialog::returnFilterString()
{
	QString res="";
	QStringList resArr;
	QStringList logicArr;
	for(uint8_t i=0;i<filterLayouts.size();i++)
	{
		//res=res+cbCols.at(i)->currentText();
		QLayoutItem *child;
		switch(cbCols.at(i)->currentIndex())
		{
		case 0:
		case 3:
		case 4:
		{
			if(cbCols.at(i)->currentIndex()==3)
				//res=res+"categories.";
				resArr.append("categories.name");
			else
				if(cbCols.at(i)->currentIndex()==4)
					//res=res+"shops.";
					resArr.append("shops.name");
			//res=res+"name";
				else
					resArr.append("expenses.name");
			for(uint8_t j=0;j<innerLayouts.at(i)->count();j++)
			{
				child=innerLayouts.at(i)->takeAt(j);
				for(uint8_t k=0;k<leLines.size();k++)
					if(child->widget()==leLines.at(k))
					{
						//res=res+"=\""+leLines.at(k)->text()+"\"";
						resArr.back().append("=\""+leLines.at(k)->text()+"\"");
						break;
					}
			}
			break;
		}
		case 1:
		{
			//res=res+"val";
			resArr.append("val");
			while((child=innerLayouts.at(i)->takeAt(0))!=0)
			{
				for(uint8_t j=0;j<cbCombo.size();j++)
					if((child->widget()==cbCombo.at(j))&&(cbCombo.at(j)->property("tag")=="compare"))
					{
						//res=res+cbCombo.at(j)->currentText();
						resArr.back().append(cbCombo.at(j)->currentText());
						break;
					}
				for(uint8_t j=0;j<dsbVals.size();j++)
					if(child->widget()==dsbVals.at(j))
					{
						//res=res+QString::number(dsbVals.at(j)->value());
						resArr.back().append(QString::number(dsbVals.at(j)->value()));
						break;
					}
			}
			break;
		}
		case 2:
		{
			//res=res+"date";
			resArr.append("date");
			while((child=innerLayouts.at(i)->takeAt(0))!=0)
			{

				for(uint8_t j=0;j<cbCombo.size();j++)
					if((child->widget()==cbCombo.at(j))&&(cbCombo.at(j)->property("tag")=="compare"))
					{
						//res=res+cbCombo.at(j)->currentText();
						resArr.back().append(cbCombo.at(j)->currentText());
						break;
					}
				for(uint8_t j=0;j<deDates.size();j++)
					if(child->widget()==deDates.at(j))
					{
						//res=res+"\""+deDates.at(j)->text()+"\"";
						resArr.back().append("\""+deDates.at(j)->text()+"\"");
						break;
					}
			}
			break;
		}
		}
		for(uint8_t j=0;j<filterLayouts.at(i)->count();j++)
		{
			child=filterLayouts.at(i)->itemAt(j);
			for(uint8_t k=0;k<cbCombo.size();k++)
			if((child->widget()==cbCombo.at(k))&&(cbCombo.at(k)->property("tag")=="logic"))
				//res=res+" "+cbCombo.at(k)->currentText()+" ";
				logicArr.append(" "+cbCombo.at(k)->currentText()+" ");
		}
	}
	res=resArr.at(0);
	for(uint8_t i=0;i<logicArr.size();i++)
		res=res+logicArr.at(i)+resArr.at(i+1);
	return res;
}

void FilterDialog::deleteElements(QList<QWidget *> *list)
{
	for(uint8_t i=0;i<list->size();i++)
		delete list->at(i);
}

FilterDialog::~FilterDialog()
{
	for(uint8_t i=0;i<dsbVals.size();i++)
		if(dsbVals.at(i))
			delete dsbVals.at(i);
	for(uint8_t i=0;i<leLines.size();i++)
		if(leLines.at(i))
			delete leLines.at(i);
	for(uint8_t i=0;i<lFilterCaptions.size();i++)
		if(lFilterCaptions.at(i))
			delete lFilterCaptions.at(i);
	for(uint8_t i=0;i<innerLayouts.size();i++)
		if(innerLayouts.at(i))
			delete innerLayouts.at(i);
	for(uint8_t i=0;i<pbDeletes.size();i++)
		if(pbDeletes.at(i))
			delete pbDeletes.at(i);
	for(uint8_t i=0;i<cbCols.size();i++)
		if(cbCols.at(i))
			delete cbCols.at(i);
	for(uint8_t i=0;i<filterLayouts.size();i++)
		if(filterLayouts.at(i))
			delete filterLayouts.at(i);
	for(uint8_t i=0;i<deDates.size();i++)
		if(deDates.at(i))
			delete deDates.at(i);
	for(uint8_t i=0;i<cbCombo.size();i++)
		if(cbCombo.at(i))
			delete cbCombo.at(i);
	delete ui;
}
