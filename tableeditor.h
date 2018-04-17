#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H
#include <QTableWidget>
#include <QObject>
#include "mysqlClient.hpp"

class TableEditor: public QObject
{
	Q_OBJECT
public:
	TableEditor(QTableWidget &widget,QObject *parent=0);
	void setTable(QTableWidget &widget);

	virtual ~TableEditor();

private:
	QTableWidget *table;
	MySQLClient *client;
	QString currentQuery;
	void reloadTable(std::vector<std::string> *vec);

public slots:
	void executeQuery(QString *query);
};

#endif // TABLEEDITOR_H
