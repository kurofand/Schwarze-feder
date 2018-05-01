#include "tableeditor.h"
#include <QErrorMessage>
#include <QDebug>
#include "editdialog.h"
#include "selectdialog.h"
#include "insertdialog.h"

TableEditor::TableEditor(QTableWidget &widget, QObject *parent) :
	QObject(parent)
{
	client=new SqliteClient("SchwarzeFeder.db");
	if(!client->connect())
	{
		emit(buttonEnabled(0));

	}
	table=&widget;
	ids=new QList<uint32_t>;
}

void TableEditor::setTable(QTableWidget &widget)
{
	this->table=new QTableWidget(&widget);
}

void TableEditor::reloadTable(std::vector<std::string> *vec)
{
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
		QStringList avHeaders/*={"Name", "Descriptoin", "Price", "Date", "Category", "Shop", "Shop flag", "Convert index", "Main currency flag"}*/;
		QStringList headers;
//		QString cols=currentQuery.replace(QString("SELECT "), QString(""));
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
						if(headers.at(i)=="date")
							avHeaders.append("Date");
						else
							if(headers.at(i)=="categories.name")
								avHeaders.append("Category");
							else
								if(headers.at(i)=="shops.name")
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
			query.append("currency(name, ind, mainFlag) VALUES(");
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
}

void TableEditor::editTable()
{
	uint16_t row;
	for(row=0;row<table->rowCount();row++)
		if(table->item(row, 0)->isSelected())
			break;
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
		query.append("name, shopAvailable, FROM categories WHERE id=");
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
	query.append(QString::number(ids->at(row)));
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
			query.append("expenses SET name=\""+res.at(0)+"\", descr=\""+res.at(2)+"\", val="+res.at(1)+", date=\""+res.at(3)+"\", categoryId="+res.at(4)+", shopId="+res.at(5)+" WHERE id="+QString::number(ids->at(row)));
			break;
		}
		}
		client->executeQuery(query.toUtf8(),*vec);
	}
	delete vec;
	delete dialog;
	currentQuery=oldQuery;
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
			str.replace(QString("##"), QString("expenses INNER JOIN categories, shops WHERE categories.id=categoryId AND shops.id=shopId"));
			QString cols[]={"expenses.name", "val", "descr", "date", "categories.name", "shops.name"};
			colsstr.prepend("expenses.");
			for(uint8_t i=0;i<6;i++)
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
	delete dialog;
}

TableEditor::~TableEditor()
{
	delete ids;
	delete table;
}
