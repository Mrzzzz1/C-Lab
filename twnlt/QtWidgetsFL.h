#pragma once

#include <Qwidget>
#include "ui_QtWidgetsFL.h"

class QtWidgetsFL : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsFL(QWidget* parent = nullptr);
	~QtWidgetsFL();
	QGraphicsView* parnt;
	void myShow(QGraphicsView* p);

private:
	Ui::QtWidgetsFLClass ui;
private slots:
	void inputStop();
	void inputLine();
	void checkFile();
};
