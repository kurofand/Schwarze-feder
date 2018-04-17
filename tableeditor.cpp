#include "tableeditor.h"

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
	client->connect();
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
	client->executeQuery(query->toAscii(), *vec);

	if(query->split(" ")[0]=="SELECT")
		reloadTable(vec);

	delete vec;
}

TableEditor::~TableEditor()
{
	delete table;
}
