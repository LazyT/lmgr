#ifndef FLASHDLG_H
#define FLASHDLG_H

#include "ui_flash.h"
#include "mainwindow.h"

class flashDialog : public QDialog, private Ui::Dialog_Flash
{
	Q_OBJECT

public:

	flashDialog(QWidget*, bool);

private:

	QProcess *process;
	QString process_output;
	QStringList devices = { "beryllium", "cepheus", "davinci", "dipper", "grus", "pyxis", "raphael", "raphaelin", "tucana" };
	QColor textcolor;
	int fontweight;

	bool usbmode, running, failed, abort, supported;

private slots:

	void initADB();
	void sendCommand(QString);
	void startFlashing();
	void startDumping();

	void prg_errorOccurred(QProcess::ProcessError);
	void prg_readyReadStandardOutput();
	void prg_finished(int, QProcess::ExitStatus);

	void accept();
	void reject();
};

#endif // FLASHDLG_H
