#include "tableeditor.h"
#include <QErrorMessage>
#include <QDebug>
#include "editdialog.h"

TableEditor::TableEditor(QTableWidget &widget, QObject *parent) :
	QObject(parent)
{
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
	if(!client->connect())
	{
		//QErrorMessage *errorMessage=new QErrorMessage();

		//if(errorMessage->exec())
		//errorMessage->showMessage("Connection to DB was not established!", "Connection error");
		//delete errorMessage;
		//QErrorMessage err();
		//err.showMessage("Connection to DB was not established!");
		//connect(this, SIGNAL(buttonEnabled(uint8_t)), )
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
		//qDebug()<<currentQuery;
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
		qDebug()<<currentQuery;
		//if(parts.size()>1)

	}
	else
		currentQuery.append(part);

}

void TableEditor::editTable()
{
	uint16_t row;
	for(row=0;row<table->rowCount();row++)
		if(table->item(row, 0)->isSelected())
			break;
	//сюды надо перенести как-то активную таблицу
	std::vector<std::string> *vec=new std::vector<std::string>();
	QString query;
	uint8_t tableIndex=0;

	//bool res=client->executeQuery()
	EditDialog *dialog=new EditDialog(0,0, ids->at(row));
	if(dialog->exec())
	{

	}
	delete dialog;
}

TableEditor::~TableEditor()
{
	delete ids;
	delete table;
}
