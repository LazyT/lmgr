#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include "ui_about.h"
#include "mainwindow.h"

class aboutDialog : public QDialog, private Ui::Dialog_About
{
	Q_OBJECT

public:

	aboutDialog(QWidget*);

private slots:

	void mouseReleaseEvent(QMouseEvent*);
};

#endif // ABOUTDLG_H
