#ifndef YEARREPORT_H
#define YEARREPORT_H

#include <QDialog>

namespace Ui {
class YearReport;
}

class YearReport : public QDialog
{
	Q_OBJECT

public:
	explicit YearReport(QWidget *parent = 0);
	~YearReport();

private:
	Ui::YearReport *ui;
};

#endif // YEARREPORT_H
