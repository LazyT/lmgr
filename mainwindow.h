#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define APPNAME "Logo Manager"
#define APPVERS "1.0.0"
#define APPDATE "17.04.2020"

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QtEndian>
#include <QDesktopServices>
#include <QTimer>
#include <QBuffer>
#include <QProcess>
#include <QThread>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QScreen>
#include <QLCDNumber>

#include "ui_mainwindow.h"
#include "about.h"
#include "flash.h"
#include "help.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);

	QString flash_file;

private:

	QTranslator baseTranslator, helpTranslator, appTranslator;

	QLCDNumber *lcd;
	int screens, startscreen;

	QByteArray logo;

	QString open_text = tr("Import Logo Image");

	void importImage(QLabel*, QLabel*, int, int);
	void exportImage(QLabel*);

	quint32 logo_ofs[9];
	quint32 logo_len[9];

	void loadLogo(quint32, quint32, int);
	void saveLogo(QFile&, int);

	quint32 calcLogo(int);
/*
	struct LOGO_HEADER
	{
		quint8 magic[8] = { 0x4C, 0x4F, 0x47, 0x4F, 0x21, 0x21, 0x21, 0x21 };	// "LOGO!!!!"

		quint32 logo_offset_1;
		quint32 logo_length_1;
		quint32 logo_offset_n;
		quint32 logo_length_n;

		or

		quint32 logo_offset_1;
		quint32 logo_offset_n;
		quint32 logo_length_1;
		quint32 logo_length_n;

	}logo_header;
*/
private slots:

	void centerOnScreen();

	void on_actionExit_triggered();
	bool on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionFlash_triggered();
	void on_actionPrevLogo_triggered();
	void on_actionNextLogo_triggered();
	void on_actionAbout_triggered();
	void on_actionHelp_triggered();

	void on_pushButton_1_imp_clicked();
	void on_pushButton_2_imp_clicked();
	void on_pushButton_3_imp_clicked();
	void on_pushButton_4_imp_clicked();
	void on_pushButton_5_imp_clicked();
	void on_pushButton_6_imp_clicked();
	void on_pushButton_7_imp_clicked();
	void on_pushButton_8_imp_clicked();
	void on_pushButton_9_imp_clicked();
	void on_pushButton_1_exp_clicked();
	void on_pushButton_2_exp_clicked();
	void on_pushButton_3_exp_clicked();
	void on_pushButton_4_exp_clicked();
	void on_pushButton_5_exp_clicked();
	void on_pushButton_6_exp_clicked();
	void on_pushButton_7_exp_clicked();
	void on_pushButton_8_exp_clicked();
	void on_pushButton_9_exp_clicked();

	void keyPressEvent(QKeyEvent*);
	void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
