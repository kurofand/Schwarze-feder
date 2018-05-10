#include "tableeditor.h"
#include <QErrorMessage>
#include <QDebug>
#include "editdialog.h"
#include "selectdialog.h"
#include "insertdialog.h"
#include "filterdialog.h"
#include "currencydialog.h"
#include <fstream>
#include "reportdialog.h"

TableEditor::TableEditor(QTableWidget &widget, QObject *parent) :
	QObject(parent)
{
	client=new SqliteClient("SchwarzeFeder.db");
	table=&widget;
	ids=new QList<uint32_t>;
	connect(table, SIGNAL(cellActivated(int,int)), this, SLOT(enableEditDelete()));
}

void TableEditor::enableEditDelete()
{
	emit buttonEnabled(4);
}

bool TableEditor::clientConnect()
{
	if(!client->connect())
	{
		emit buttonEnabled(0);
		return false;
	}
	else
	{
		emit buttonEnabled(1);
		return true;
	}
}

void TableEditor::setTable(QTableWidget &widget)
{
	this->table=new QTableWidget(&widget);
}

void TableEditor::reloadTable(std::vector<std::string> *vec)
{
	if(vec->size()==0)
		emit buttonEnabled(5);
	for(uint16_t i=0;i<vec->size();i++)
	{
		QString id=QString::fromStdString(vec->at(i));
		ids->append(id.split("|")[0].toInt());
		uint16_t pos=0;
		while(id.at(pos)!=*"|")
			pos++;
		//без +1 он не захватывает саму черту
		vec->at(i).erase(0, pos+1);
	}
	table->setRowCount(vec->size());
	for(uint16_t i=0;i<vec->size();i++)
	{
		/*тут должно быть обновление названия столбцов таблицы*/
		QStringList avHeaders;
		QStringList headers;
		QString cols=currentQuery;
		cols.replace(QString("SELECT "), QString(""));
		cols=cols.split("FROM")[0];
		cols.remove(cols.size()-1, 1);
		headers=cols.split(", ");
		for(uint8_t i=0;i<headers.size();i++)
			if(headers.at(i)=="expenses.name"||headers.at(i)=="name")
				avHeaders.append("Name");
			else
				if(headers.at(i)=="descr")
					avHeaders.append("Description");
				else
					if(headers.at(i)=="val")
						avHeaders.append("Price");
					else
							if(headers.at(i)=="currency")
								avHeaders.append("Currency");
							else
								if(headers.at(i)=="date")
									avHeaders.append("Date");
								else
									if(headers.at(i)=="categories.name"||headers.at(i)=="category")
										avHeaders.append("Category");
									else
										if(headers.at(i)=="shops.name"||headers.at(i)=="shop")
											avHeaders.append("Shop");
										else
											if(headers.at(i)=="shopAvailable")
												avHeaders.append("Shop flag");
											else
												if(headers.at(i)=="ind")
													avHeaders.append("Convesrion index");
												else
													if(headers.at(i)=="mainFlag")
														avHeaders.append("Main currency flag");


		QString str=QString::fromStdString(vec->at(i));
		str=str.fromUtf8(str.toAscii());
		QStringList row=str.split("|");
		if(table->colorCount()!=row.size())
			table->setColumnCount(row.size());
		for(uint8_t j=0;j<row.size();j++)
			table->setItem(i,j, new QTableWidgetItem(row.at(j)));
		table->setHorizontalHeaderLabels(avHeaders);
	}
	table->resizeColumnsToContents();
}

void TableEditor::executeQuery(QString *query)
{
	std::vector<std::string> *vec=new std::vector<std::string>();
	bool res=client->executeQuery(query->toAscii(), *vec);
	if(res)
	{
		currentQuery=*query;
		if(query->split(" ")[0]=="SELECT")
			reloadTable(vec);
	}
	delete vec;
}

void TableEditor::refreshTable()
{
	this->executeQuery(&currentQuery);
}

void TableEditor::setCurrentQuery(QString *part, uint8_t pos)
{
	if(currentQuery!="")
	{
		QStringList parts=currentQuery.split("WHERE");
		switch(pos)
		{
		case 0:
		{
			//при новом селекте удаляем старую фильтрацию
			parts.clear();
			parts.append(*part);
			break;
		}
		case 1:
		{
			if(parts.size()>1)
				parts[1]=*part;
			else
				parts.append(*part);
			break;
		}
		}
		uint8_t last=parts.at(0).size()-1;
		if(parts.at(0).at(last)==*" ")
			if(parts.at(1).at(0)!=*" ")
				currentQuery=parts.join("WHERE ");
			else
				currentQuery=parts.join("WHERE");
		else
			currentQuery=parts.join(" WHERE ");

	}
	else
		currentQuery.append(part);

}

void TableEditor::insertTable()
{
	InsertDialog *dialog=new InsertDialog();
	QStringList res;
	QString oldQuery=currentQuery;
	if(dialog->exec())
	{
		res=dialog->returnParams();
		QStringList colNames;
		for(uint8_t i=1;i<res.size();i++)
			colNames.append(res.at(i).split("|")[0]);
		QString query="INSERT INTO ";
		switch(res.at(0).toInt())
		{
		case 0:
		{
			query.append("expenses(");
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
		}
		case 1:
		{
			query.append("categories(name, shopAvailable) VALUES(");
			for(uint8_t i=1;i<res.size();i++)
				if(i!=res.size()-1)
					query.append(res.at(i).split("|")[1]+", ");
				else
					query.append(res.at(i).split("|")[1]+")");
			break;
		}
		case 2:
		{
			query.append("shops(name) VALUES("+res.at(1).split("|")[1]+")");
			break;
		}
		case 3:
		{
			query.append("vCurrency(name, ind, mainFlag) VALUES(");
			for(uint8_t i=1;i<res.size();i++)
				if(i!=res.size()-1)
					query.append(res.at(i).split("|")[1]+", ");
				else
					query.append(res.at(i).split("|")[1]+")");
			break;
		}
		}
		std::vector<std::string> *vec=new std::vector<std::string>();
		client->executeQuery(query.toUtf8(), *vec);
		delete vec;
	}
	delete dialog;
	currentQuery=oldQuery;
	emit(refreshTable());
}

uint16_t TableEditor::getSelectedId()
{
	uint16_t res;
	for(res=0;res<table->rowCount();res++)
		if(table->item(res, 0)->isSelected())
			break;
	return ids->at(res);
}

void TableEditor::editTable()
{
	uint16_t id=this->getSelectedId();
	//сюды надо перенести как-то активную таблицу
	//активная таблица в классовом таблеиндексе
	std::vector<std::string> *vec=new std::vector<std::string>();
	QString query="SELECT ", oldQuery=currentQuery;
	switch(tableIndex)
	{
	case 0:
	case 1:
	{
		query.append("expenses.name, val, descr, date, categories.name, shops.name"
			  " FROM expenses INNER JOIN categories, shops"
			  " WHERE expenses.categoryId=categories.id AND expenses.shopId=shops.id"
			  " AND expenses.id=");
		break;
	}
	case 2:
	{
		query.append("name, shopAvailable FROM categories WHERE id=");
		break;
	}
	case 3:
	{
		query.append("name FROM shops WHERE id=");
		break;
	}
	case 4:
	{
		query.append("name, ind, mainFlag FROM currency WHERE id=");
		break;
	}
	}
	query.append(QString::number(id));
	client->executeQuery(query.toAscii(), *vec);
	//поскольку фильтруем по уникальному ид можно смело брать нулевой элемент
	EditDialog *dialog=new EditDialog(0, tableIndex, vec->at(0).c_str());
	if(dialog->exec())
	{
		QStringList res=dialog->returnParams();
		QString query="UPDATE ";
		switch(tableIndex)
		{
		case 0:
		case 1:
		{
			query.append("expenses SET name=\""+res.at(0)+"\", descr=\""+res.at(2)+"\", val="+res.at(1)+", date=\""+res.at(3)+"\", categoryId="+res.at(4)+", shopId="+res.at(5));
			break;
		}
		case 2:
		{
			query.append("categories SET name=\""+res.at(0)+"\", shopAvailable="+res.at(1));
			break;
		}
		case 3:
		{
			query.append("shops SET name=\""+res.at(0)+"\"");
			break;
		}
		case 4:
		{
			query.append("vCurrency SET name=\""+res.at(0)+"\", ind="+res.at(1)+", mainFlag="+res.at(2));
			break;
		}
		}
		query.append(" WHERE id="+QString::number(id));
		client->executeQuery(query.toUtf8(),*vec);
	}
	delete vec;
	delete dialog;
	currentQuery=oldQuery;
	emit(refreshTable());
}

void TableEditor::deleteTable()
{
	uint16_t id=this->getSelectedId();
	QString oldQuery=currentQuery;
	QString query="DELETE FROM # WHERE id="+QString::number(id);
	QString table;
	switch(tableIndex)
	{
	case 0:
	case 1:
	{
		table="expenses";
		break;
	}
	case 2:
	{
		table="categories";
		break;
	}
	case 3:
	{
		table="shops";
		break;
	}
	case 4:
	{
		table="vCurrency";
		break;
	}
	}
	query.replace(QString("#"), table);
	std::vector<std::string> *vec=new std::vector<std::string>();
	client->executeQuery(query.toAscii(), *vec);
	delete vec;
	currentQuery=oldQuery;
	emit(refreshTable());
}

void TableEditor::selectTable()
{
	SelectDialog *dialog=new SelectDialog();
	QStringList res;
	QString str="SELECT # FROM ##";
	if(dialog->exec())
	{
		res=dialog->returnParams();
		tableIndex=res.at(0).toInt();
		QString colsstr="id, ";
		switch(tableIndex)
		{
		case 0:
		{
			//str.replace(QString("##"), QString("expenses INNER JOIN categories, shops WHERE categories.id=categoryId AND shops.id=shopId"));
			str.replace(QString("##"), QString("base"));
			//QString cols[]={"expenses.name", "val", "descr", "date", "categories.name", "shops.name"};
			QString cols[]={"name", "val", "currency", "descr", "date", "category", "shop"};
			//colsstr.prepend("expenses.");
			for(uint8_t i=0;i<7;i++)
				if(res.at(i+1)=="1")
					colsstr.append(cols[i]+", ");
			uint8_t pos=colsstr.lastIndexOf(",");
			colsstr=colsstr.left(pos);
			str.replace(QString("#"), colsstr);
			break;
		}
		case 1:
		{
			str.replace(QString("##"), QString("expenses"));
			QString cols[]={"name", "val", "descr", "date"};
			for(uint8_t i=0;i<4;i++)
				if(res.at(i+1)=="1")
					colsstr.append(cols[i]+", ");
			uint8_t pos=colsstr.lastIndexOf(",");
			colsstr=colsstr.left(pos);
			str.replace(QString("#"), colsstr);
			break;
		}
		case 2:
		{
			str.replace(QString("##"), QString("categories"));
			QString cols[]={"name", "shopAvailable"};
			for(uint8_t i=0;i<2;i++)
				if(res.at(i+1)=="1")
					colsstr.append(cols[i]+", ");
			uint8_t pos=colsstr.lastIndexOf(",");
			colsstr=colsstr.left(pos);
			str.replace(QString("#"), colsstr);
			break;
		}
		case 3:
		{
			str.replace(QString("##"), QString("shops"));
			if(res.at(1)=="1")
				str.replace(QString("#"), QString("id, name"));
			break;
		}
		case 4:
		{
			str.replace(QString("##"), QString("currency"));
			QString cols[]={"name", "ind", "mainFlag"};
			for(uint8_t i=0;i<3;i++)
				if(res.at(i+1)=="1")
					colsstr.append(cols[i]+", ");
			uint8_t pos=colsstr.lastIndexOf(",");
			colsstr=colsstr.left(pos);
			str.replace(QString("#"), colsstr);
			break;
		}
		}
	}
	str=str.toUtf8();
	emit(setCurrentQuery(&str, 0));
	emit buttonEnabled(2);
	emit buttonEnabled(3);
	emit buttonEnabled(6);
	delete dialog;
}

void TableEditor::setFilter()
{
	FilterDialog *dialog=new FilterDialog(0, tableIndex);
	if(dialog->exec())
	{
		QString filterQuery=dialog->returnFilterString();
		QString query=filterQuery;
		query=query.toUtf8();
		this->setCurrentQuery(&query, 1);
	}
	delete dialog;
	std::vector<std::string> *vec=new std::vector<std::string>();
	client->executeQuery(currentQuery.toAscii(), *vec);
	emit reloadTable(vec);
	delete vec;
}

void TableEditor::setCurrency()
{
	std::vector<std::string> *vec=new std::vector<std::string>();
//	QStringList *currency=new QStringList();
	client->executeQuery("SELECT name, ind, mainFlag FROM vCurrency", *vec);
/*	for(uint16_t i=0;i<vec->size();i++)
	{
		currency->append(vec->at(i).c_str());
		currency->back()=currency->back().toAscii();
	}*/
	CurrencyDialog *dialog=new CurrencyDialog(0, vec);
	if(dialog->exec())
	{
		std::vector<std::string> *vals=new std::vector<std::string>();
		QString query="SELECT val, currencyId FROM expenses WHERE id IN (#)";
		QString idStr;
		for(uint16_t i=0;i<ids->size();i++)
			idStr.append(QString::number(ids->at(i))+", ");
		//удаляем последнюю запятую и пробел
		idStr.chop(2);
		query.replace(QString("#"), idStr);
		client->executeQuery(query.toAscii(), *vals);
		QStringList newVals=dialog->changeCurrency(vals);
		uint8_t colInd;
		for(colInd=0;colInd<table->columnCount();colInd++)
			if(table->horizontalHeaderItem(colInd)->text()=="Price")
				break;
		for(uint16_t i=0;i<newVals.size();i++)
			table->setItem(i,colInd,new QTableWidgetItem(newVals.at(i)));
		delete vals;

	}
	delete vec;
	delete dialog;
}

void TableEditor::createMonthReport()
{
	QStringList *params=new QStringList();
	std::vector<std::string> *cat=new std::vector<std::string>();
	client->executeQuery("SELECT name FROM categories", *cat);
	for(uint8_t i=0;i<cat->size();i++)
	{
		QString buf=cat->at(i).c_str();
		buf=buf.fromUtf8(buf.toAscii());
		params->append(buf);
	}
	QString buf=params->join("|");
	params->clear();
	params->append(buf);
	cat->clear();
	client->executeQuery("SELECT name FROM currency", *cat);
	buf.clear();
	uint16_t catSize=cat->size();
	for(uint8_t i=0;i<catSize;i++)
		buf=buf+cat->at(i).c_str()+"|";
	buf=buf.fromUtf8(buf.toAscii());
	buf.chop(1);
	params->append(buf);
	delete cat;
	ReportDialog *dialog=new ReportDialog(0, params);
	delete params;
	if(dialog->exec())
	{
		//QDataStream *returnedParams;
		QDataStream returnedParams;
		//*returnedParams<<dialog->returnParams();
		returnedParams<<dialog->returnParams();
		QDateTime date;
		//date=new QDateTime();
		returnedParams>>date;
		uint8_t catStates[catSize];
		for(uint16_t i=0;i<catSize;i++)
			returnedParams>>catStates[i];
		uint16_t currId;
		returnedParams>>currId;
		//delete date;
	}


	QStringList reportText;
	reportText.append("<!DOCTYPE html>");
	reportText.append("<meta charset=\"UTF-8\"/>");
	double totalSum=0;
	QDate date=QDate::currentDate();
	QString mY=date.longMonthName(date.month())+" "+QString::number(date.year());
	reportText.append("<h2>"+mY+"</h2>");
	std::vector<std::string> *vec=new std::vector<std::string>();
	client->executeQuery("SELECT name, shopAvailable FROM categories", *vec);
	for(uint16_t i=0;i<vec->size();i++)
	{
		QString catName=vec->at(i).c_str();
		uint8_t shop=catName.split("|")[1].toInt();
		catName=catName.split("|")[0];
		catName=catName.fromUtf8(catName.toAscii());
		double sum=0;
		reportText.append("<h4>"+catName+"</h4>");
		std::vector<std::string> *t=new std::vector<std::string>();
		QString query="SELECT name, val, date, descr# FROM base WHERE category=\""+catName+"\"";
		if (shop==1)
			query.replace(QString("#"), QString(", shop"));
		else
			query.replace(QString("#"), QString(""));
		client->executeQuery(query.toAscii(), *t);
		reportText.append("<table border=1>");
		reportText.append("<tr>");
		reportText.append("<th>Name</th>");
		reportText.append("<th>Value</th>");
		reportText.append("<th>Date</th>");
		reportText.append("<th>Description</th>");
		if(shop==1)
			reportText.append("<th>Shop</th>");
		reportText.append("</tr>");
		for(uint16_t j=0;j<t->size();j++)
		{
			reportText.append("<tr>");
			QString row=t->at(j).c_str();
			row=row.fromUtf8(row.toAscii());
			QStringList cells=row.split("|");
			for(uint8_t k=0;k<cells.size();k++)
			{
				if(k==1)
					sum=sum+cells.at(k).toDouble();
				reportText.append("<td>"+cells.at(k)+"</td>");
			}
			reportText.append("</tr>");
		}
		reportText.append("</table>");
		reportText.append("<h5>Sum: "+QString::number(sum)+"</h5><hr>");
		totalSum=totalSum+sum;
		delete t;
	}
	delete vec;
	reportText.append("<h4>Total: "+QString::number(totalSum)+"</h4>");
	std::ofstream file;
	file.open(mY.toUtf8()+".html");
	for(uint16_t i=0;i<reportText.size();i++)
		file<<reportText.at(i).toUtf8().constData();
	file.close();
	delete dialog;
}

TableEditor::~TableEditor()
{
	delete ids;
	delete table;
}
