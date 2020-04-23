#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow win;

	win.show();

	return app.exec();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
//	QApplication::setStyle("Fusion");
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

	if(!QLocale::system().name().startsWith("en_"))
	{
		if(appTranslator.load("lmgr_" + QLocale::system().name(), QApplication::applicationDirPath() + "/lng"))
		{
			QApplication::installTranslator(&appTranslator);

			if(baseTranslator.load("qtbase_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			{
				QApplication::installTranslator(&baseTranslator);
			}
			else if(baseTranslator.load("qtbase_" + QLocale::system().name(), QApplication::applicationDirPath() + "/lng"))
			{
				QApplication::installTranslator(&baseTranslator);
			}

			if(helpTranslator.load("qt_help_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			{
				QApplication::installTranslator(&helpTranslator);
			}
			else if(helpTranslator.load("qt_help_" + QLocale::system().name(), QApplication::applicationDirPath() + "/lng"))
			{
				QApplication::installTranslator(&helpTranslator);
			}
		}
	}

	setupUi(this);

	lcd = new QLCDNumber(1);
	lcd->setToolTip(tr("Logo Screens"));
	lcd->setStatusTip(tr("Logo Screens"));

	toolBar->insertWidget(actionNextLogo, lcd);

	logo_5->hide();
	logo_6->hide();
	logo_7->hide();
	logo_8->hide();
	logo_9->hide();

	resize(0, 0);

	QTimer::singleShot(1, this, SLOT(centerOnScreen()));
}

void MainWindow::centerOnScreen()
{
	lcd->setMinimumWidth(lcd->width() + 20);
	lcd->setMaximumHeight(lcd->height() - 20);

	move(screen()->geometry().center() - rect().center());
}

quint32 MainWindow::calcLogo(int pos)
{
	QByteArray data = logo.mid(pos, 4);

	return (static_cast<quint8>(data.at(1)) << 24 | static_cast<quint8>(data.at(0)) << 16 | static_cast<quint8>(data.at(3)) << 8 | static_cast<quint8>(data.at(2))) >> 4;
}

bool MainWindow::loadLogo(quint32 ofs, quint32 len, int index)
{
	QPixmap pm;
	QLabel *img[9] = { label_1_img, label_2_img, label_3_img, label_4_img, label_5_img, label_6_img, label_7_img, label_8_img, label_9_img };
	QLabel *txt[9] = { label_1_txt, label_2_txt, label_3_txt, label_4_txt, label_5_txt, label_6_txt, label_7_txt, label_8_txt, label_9_txt };

	bool result = pm.loadFromData(logo.mid(ofs, len), "BMP");

	if(result)
	{
		img[index]->setPixmap(pm);
		txt[index]->setText(QString("%1 x %2\n").arg(pm.width()).arg(pm.height()) + "0x" + QString("%1\n").arg(ofs, 8, 16, QChar('0')).toUpper() + "0x" + QString("%1").arg(len, 8, 16, QChar('0')).toUpper());
	}
	else
	{
		img[index]->setPixmap(QPixmap(":/png/png/warning.png").scaled(128, 128, Qt::KeepAspectRatio));
		txt[index]->setText("\n?\n");
	}

	return result;
}

void MainWindow::saveLogo(QFile &file, int index)
{
	QByteArray arr;
	QBuffer buf(&arr);
	QLabel *img[9] = { label_1_img, label_2_img, label_3_img, label_4_img, label_5_img, label_6_img, label_7_img, label_8_img, label_9_img };

	buf.open(QIODevice::WriteOnly);

	img[index]->pixmap()->save(&buf, "BMP");

	file.write(arr);
	file.write(QByteArray(logo_len[index] - arr.size(), 0x00));

	buf.close();
}

void MainWindow::importImage(QLabel *pic, QLabel *txt, int ofs, int len)
{
	QFile file(QFileDialog::getOpenFileName(this, tr("Import Logo Screen"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)+ QString("/logo%1.png").arg(pic->objectName().mid(6, 1)), "Images (*.png *.jpg *.bmp)", nullptr, QFileDialog::DontUseNativeDialog));

	if(!file.fileName().isEmpty())
	{
		QPixmap img(file.fileName());

		if(img.width() != pic->pixmap()->width() || img.height() != pic->pixmap()->height())
		{
			QMessageBox::warning(this, APPNAME, tr("Selected logo screen will be scaled from %1x%2 to %3x%4!").arg(img.width()).arg(img.height()).arg(pic->pixmap()->width()).arg(pic->pixmap()->height()));
		}

		pic->setPixmap(img.scaled(pic->pixmap()->width(), pic->pixmap()->height()));
		txt->setText(QString("%1 x %2\n").arg(pic->pixmap()->width()).arg(pic->pixmap()->height()) + "0x" + QString("%1\n").arg(ofs, 8, 16, QChar('0')).toUpper() + "0x" + QString("%1").arg(len, 8, 16, QChar('0')).toUpper());
	}
}

void MainWindow::exportImage(QLabel *label)
{
	QImage img = label->pixmap()->toImage().convertToFormat(QImage::Format_RGB888);

	QFile save(QFileDialog::getSaveFileName(this, tr("Export Logo Screen"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + QString("/logo%1.png").arg(label->objectName().mid(6, 1)), "Images (*.png *.jpg *.bmp);;Raw (*.raw)", nullptr, QFileDialog::DontUseNativeDialog));

	if(!save.fileName().isEmpty())
	{
		if(save.fileName().endsWith(".raw"))
		{
			bool rc = false;
			qint64 bytes = -1;

			if((rc = save.open(QIODevice::WriteOnly)))
			{
				bytes = save.write(reinterpret_cast<char*>(img.bits()), img.sizeInBytes());

				save.close();
			}

			if(rc == false || bytes == -1)
			{
				QMessageBox::warning(this, APPNAME, tr("Logo screen export failed!\n\n%1").arg(save.errorString()));
			}
			else
			{
				QMessageBox::information(this, APPNAME, tr("Logo screen export successfull."));
			}
		}
		else
		{
			if(img.save(save.fileName()))
			{
				QMessageBox::information(this, APPNAME, tr("Logo screen export successfull."));
			}
			else
			{
				QMessageBox::warning(this, APPNAME, tr("Logo screen export failed!\n\n%1").arg(save.errorString()));
			}
		}
	}
}

void MainWindow::on_actionExit_triggered()
{
	close();
}

bool MainWindow::on_actionOpen_triggered()
{
	QFile file(QFileDialog::getOpenFileName(this, open_text, QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/logo.img", "logo*.img", nullptr, QFileDialog::DontUseNativeDialog));

	flash_file = file.fileName();
	open_text = tr("Import Logo Image");

	if(!file.fileName().isEmpty())
	{
		if(file.open(QIODevice::ReadOnly))
		{
			logo = file.readAll();
			file.close();

			if(logo.mid(0x4000, 8) != "LOGO!!!!")
			{
				QMessageBox::warning(this, APPNAME, tr("Doesn't look like a supported logo image!\n\nNo \"LOGO!!!!\" header @ 0x4000..."));
			}
			else
			{
				int index = 0;
				int logos = 0;
				bool mixed = true;
				bool failed = false;
				quint32 val1, val2;

				startscreen = 0;

				logo_9->hide();
				logo_8->hide();
				logo_7->hide();
				logo_6->hide();
				logo_5->hide();
				logo_4->show();
				logo_3->show();
				logo_2->show();
				logo_1->show();

				actionPrevLogo->setEnabled(false);
				actionNextLogo->setEnabled(false);

				pushButton_1_imp->setEnabled(false);
				pushButton_2_imp->setEnabled(false);
				pushButton_3_imp->setEnabled(false);
				pushButton_4_imp->setEnabled(false);
				pushButton_5_imp->setEnabled(false);
				pushButton_6_imp->setEnabled(false);
				pushButton_7_imp->setEnabled(false);
				pushButton_8_imp->setEnabled(false);
				pushButton_9_imp->setEnabled(false);
				pushButton_1_exp->setEnabled(false);
				pushButton_2_exp->setEnabled(false);
				pushButton_3_exp->setEnabled(false);
				pushButton_4_exp->setEnabled(false);
				pushButton_5_exp->setEnabled(false);
				pushButton_6_exp->setEnabled(false);
				pushButton_7_exp->setEnabled(false);
				pushButton_8_exp->setEnabled(false);
				pushButton_9_exp->setEnabled(false);

				actionSave->setEnabled(false);

				if(logo.mid(calcLogo(0x4008), 2).toHex() == "424d" && logo.mid(calcLogo(0x400C), 2).toHex() == "424d")
				{
					while(logo.mid(calcLogo(0x4008 + logos*4), 2).toHex() == "424d")
					{
						logos++;
					};

					mixed = false;
				}

				do
				{
					if(mixed)
					{
						val1 = calcLogo(0x4008 + index*8 + 0);
						val2 = calcLogo(0x4008 + index*8 + 4);
					}
					else
					{
						val1 = calcLogo(0x4008 + index*4 + 0);
						val2 = calcLogo(0x4008 + index*4 + logos*4);
					}

					if(val1 && val2)
					{
						if(index < 9)
						{
							logo_ofs[index] = val1;
							logo_len[index] = val2;

							if(!loadLogo(val1, val2, index))
							{
								failed = true;
							}
						}

						index++;
					}
				}
				while((val1 && val2));

				screens = index;

				lcd->display(screens);

				if(screens > 4)
				{
					actionNextLogo->setEnabled(true);
				}

				if(screens > 9)
				{
					QMessageBox::warning(this, APPNAME, tr("More than 9 logo screens found!\n\nThis is not supported yet..."));

					actionSave->setEnabled(false);

					return false;
				}

				if(failed)
				{
					QMessageBox::warning(this, APPNAME, tr("Could not load logo screens!"));

					return false;
				}
				else
				{
					pushButton_1_imp->setEnabled(true);
					pushButton_2_imp->setEnabled(true);
					pushButton_3_imp->setEnabled(true);
					pushButton_4_imp->setEnabled(true);
					pushButton_5_imp->setEnabled(true);
					pushButton_6_imp->setEnabled(true);
					pushButton_7_imp->setEnabled(true);
					pushButton_8_imp->setEnabled(true);
					pushButton_9_imp->setEnabled(true);
					pushButton_1_exp->setEnabled(true);
					pushButton_2_exp->setEnabled(true);
					pushButton_3_exp->setEnabled(true);
					pushButton_4_exp->setEnabled(true);
					pushButton_5_exp->setEnabled(true);
					pushButton_6_exp->setEnabled(true);
					pushButton_7_exp->setEnabled(true);
					pushButton_8_exp->setEnabled(true);
					pushButton_9_exp->setEnabled(true);

					actionSave->setEnabled(true);
				}

				return true;
			}
		}
		else
		{
			QMessageBox::warning(this, APPNAME, tr("Could not open logo image!\n\n%1").arg(file.errorString()));
		}
	}

	return false;
}

void MainWindow::on_actionSave_triggered()
{
	QFile file(QFileDialog::getSaveFileName(this, tr("Export Logo Image"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/logo.img", "logo*.img", nullptr, QFileDialog::DontUseNativeDialog));

	if(!file.fileName().isEmpty())
	{
		if(file.open(QIODevice::WriteOnly))
		{
			file.write(logo.mid(0, logo_ofs[0]));

			for(int logo = 0; logo < screens; logo++)
			{
				saveLogo(file, logo);
			}

			file.close();

			QMessageBox::information(this, APPNAME, tr("Logo image saved successfully."));
		}
		else
		{
			QMessageBox::critical(this, APPNAME, tr("Could not save logo image!\n\n%1").arg(file.errorString()));
		}
	}
}

void MainWindow::on_actionDump_triggered()
{
	if(QMessageBox::question(this, APPNAME, tr("Dumping logo image needs root!\n\nReally continue?")) == QMessageBox::Yes)
	{
		flashDialog(this, DUMP).exec();
	}
}

void MainWindow::on_actionFlash_triggered()
{
	if(QMessageBox::question(this, APPNAME, tr("Flashing wrong logo image may brick your device!\n\nReally continue?")) == QMessageBox::Yes)
	{
		open_text = tr("Flash Logo Image");

		if(on_actionOpen_triggered())
		{
			flashDialog(this, FLASH).exec();
		}
	}
}

void MainWindow::on_actionPrevLogo_triggered()
{
	QWidget *logos[9] = { logo_1, logo_2, logo_3, logo_4, logo_5, logo_6, logo_7, logo_8, logo_9 };

	if(startscreen > 0)
	{
		startscreen--;

		logos[startscreen + 4]->hide();
		logos[startscreen]->show();
	}

	if(startscreen == 0)
	{
		actionPrevLogo->setEnabled(false);
	}

	if(!actionNextLogo->isEnabled())
	{
		actionNextLogo->setEnabled(true);
	}
}

void MainWindow::on_actionNextLogo_triggered()
{
	QWidget *logos[9] = { logo_1, logo_2, logo_3, logo_4, logo_5, logo_6, logo_7, logo_8, logo_9 };

	if(startscreen < screens - 4)
	{
		logos[startscreen]->hide();
		logos[startscreen + 4]->show();

		startscreen++;
	}

	if(startscreen == screens - 4)
	{
		actionNextLogo->setEnabled(false);
	}

	if(!actionPrevLogo->isEnabled())
	{
		actionPrevLogo->setEnabled(true);
	}
}

void MainWindow::on_actionAbout_triggered()
{
	aboutDialog(this).exec();
}

void MainWindow::on_actionHelp_triggered()
{
	helpDialog(this).exec();
}

void MainWindow::on_actionUpdate_triggered()
{
	new onlineUpdDialog(this, false);
}

void MainWindow::on_pushButton_1_imp_clicked()
{
	importImage(label_1_img, label_1_txt, logo_ofs[0], logo_len[0]);
}

void MainWindow::on_pushButton_2_imp_clicked()
{
	importImage(label_2_img, label_2_txt, logo_ofs[1], logo_len[1]);
}

void MainWindow::on_pushButton_3_imp_clicked()
{
	importImage(label_3_img, label_3_txt, logo_ofs[2], logo_len[2]);
}

void MainWindow::on_pushButton_4_imp_clicked()
{
	importImage(label_4_img, label_4_txt, logo_ofs[3], logo_len[3]);
}

void MainWindow::on_pushButton_5_imp_clicked()
{
	importImage(label_5_img, label_5_txt, logo_ofs[4], logo_len[4]);
}

void MainWindow::on_pushButton_6_imp_clicked()
{
	importImage(label_6_img, label_6_txt, logo_ofs[5], logo_len[5]);
}

void MainWindow::on_pushButton_7_imp_clicked()
{
	importImage(label_7_img, label_7_txt, logo_ofs[6], logo_len[6]);
}

void MainWindow::on_pushButton_8_imp_clicked()
{
	importImage(label_8_img, label_8_txt, logo_ofs[7], logo_len[7]);
}

void MainWindow::on_pushButton_9_imp_clicked()
{
	importImage(label_9_img, label_9_txt, logo_ofs[8], logo_len[8]);
}

void MainWindow::on_pushButton_1_exp_clicked()
{
	exportImage(label_1_img);
}

void MainWindow::on_pushButton_2_exp_clicked()
{
	exportImage(label_2_img);
}

void MainWindow::on_pushButton_3_exp_clicked()
{
	exportImage(label_3_img);
}

void MainWindow::on_pushButton_4_exp_clicked()
{
	exportImage(label_4_img);
}

void MainWindow::on_pushButton_5_exp_clicked()
{
	exportImage(label_5_img);
}

void MainWindow::on_pushButton_6_exp_clicked()
{
	exportImage(label_6_img);
}

void MainWindow::on_pushButton_7_exp_clicked()
{
	exportImage(label_7_img);
}

void MainWindow::on_pushButton_8_exp_clicked()
{
	exportImage(label_8_img);
}

void MainWindow::on_pushButton_9_exp_clicked()
{
	exportImage(label_9_img);
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
	if(ke->key() == Qt::Key_F1)
	{
		on_actionHelp_triggered();
	}
	else if(ke->key() == Qt::Key_Escape)
	{
		close();
	}

	QMainWindow::keyPressEvent(ke);
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
	if(forceclose || QMessageBox::question(this, APPNAME, tr("Really exit program?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		ce->accept();
	}
	else
	{
		ce->ignore();
	}
}
