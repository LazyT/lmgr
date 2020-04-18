#ifndef HELPDLG_H
#define HELPDLG_H

#include "ui_help.h"
#include "mainwindow.h"

class helpDialog : public QDialog, private Ui::Dialog_Help
{
	Q_OBJECT

public:

	helpDialog(QWidget*);

private:

	QString lng;

	QHelpEngine *helpEngine;
	QHelpContentWidget *contentWidget;

private slots:

	void setSourceFromContent(QModelIndex);
	void expand();
};

#endif // HELPDLG_H
