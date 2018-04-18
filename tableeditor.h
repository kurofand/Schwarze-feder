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
	QString currentQuery="SELECT expenses.name, val, date, descr, categories.name, shops.name "
						 "FROM expenses INNER JOIN categories, shops "
						 "WHERE categoryId=categories.id "
						 "AND shopId=shops.id";
	void reloadTable(std::vector<std::string> *vec);

public slots:
	void executeQuery(QString *query);
	void refreshTable();
};

#endif // TABLEEDITOR_H
