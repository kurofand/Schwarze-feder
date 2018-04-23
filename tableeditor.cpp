#include "tableeditor.h"
#include <QErrorMessage>
#include <QDebug>

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
}

void TableEditor::setTable(QTableWidget &widget)
{
	this->table=new QTableWidget(&widget);
}

void TableEditor::reloadTable(std::vector<std::string> *vec)
{
	table->setRowCount(vec->size());
	for(uint16_t i=0;i<vec->size();i++)
	{
		/*тут должно быть обновление названия столбцов таблицы*/
		qDebug()<<currentQuery;
		QStringList avHeaders={"Name", "Descriptoin", "Price", "Date", "Category", "Shop", "Shop flag", "Convert index", "Main currency flag"};
		QStringList headers;
		QString cols=currentQuery.replace(QString("SELECT "), QString(""));
		QString str=QString::fromStdString(vec->at(i));
		str=str.fromUtf8(str.toAscii());
		QStringList row=str.split("|");
		if(table->colorCount()!=row.size())
			table->setColumnCount(row.size());
		for(uint8_t j=0;j<row.size();j++)
			table->setItem(i,j, new QTableWidgetItem(row.at(j)));
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

TableEditor::~TableEditor()
{
	delete table;
}
