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

	widget->hide();

	move(screen()->geometry().center() - rect().center());
}

quint32 MainWindow::calcLogo(int pos)
{
	QByteArray data = logo.mid(pos, 4);

	return (static_cast<quint8>(data.at(1)) << 24 | static_cast<quint8>(data.at(0)) << 16 | static_cast<quint8>(data.at(3)) << 8 | static_cast<quint8>(data.at(2))) >> 4;
}

void MainWindow::loadLogo(quint32 ofs, quint32 len, int index)
{
	QPixmap pm;
	QLabel *img[5] = { label_1_img, label_2_img, label_3_img, label_4_img, label_5_img };
	QLabel *txt[5] = { label_1_txt, label_2_txt, label_3_txt, label_4_txt, label_5_txt };

	pm.loadFromData(logo.mid(ofs, len), "BMP");

	img[index]->setPixmap(pm);
	txt[index]->setText(QString("%1 x %2\n").arg(pm.width()).arg(pm.height()) + "0x" + QString("%1\n").arg(ofs, 8, 16, QChar('0')).toUpper() + "0x" + QString("%1").arg(len, 8, 16, QChar('0')).toUpper());
}

void MainWindow::saveLogo(QFile &file, int index)
{
	QByteArray arr;
	QBuffer buf(&arr);
	QLabel *img[5] = { label_1_img, label_2_img, label_3_img, label_4_img, label_5_img };

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
				quint32 val1, val2;

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
						if(index < 5)
						{
							logo_ofs[index] = val1;
							logo_len[index] = val2;

							loadLogo(val1, val2, index);
						}

						index++;
					}
				}
				while((val1 && val2));

				index > 4 ? widget->show() : widget->hide();

				if(index > 5)
				{
					QMessageBox::warning(this, APPNAME, tr("More than 5 logo screens found!\n\nThis is not supported yet..."));

					actionSave->setEnabled(false);

					return false;
				}

				pushButton_1_imp->setEnabled(true);
				pushButton_2_imp->setEnabled(true);
				pushButton_3_imp->setEnabled(true);
				pushButton_4_imp->setEnabled(true);
				pushButton_5_imp->setEnabled(true);
				pushButton_1_exp->setEnabled(true);
				pushButton_2_exp->setEnabled(true);
				pushButton_3_exp->setEnabled(true);
				pushButton_4_exp->setEnabled(true);
				pushButton_5_exp->setEnabled(true);

				actionSave->setEnabled(true);

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

			saveLogo(file, 0);
			saveLogo(file, 1);
			saveLogo(file, 2);
			saveLogo(file, 3);

			if(widget->isVisible())
			{
				saveLogo(file, 4);
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

void MainWindow::on_actionFlash_triggered()
{
	if(QMessageBox::question(this, APPNAME, tr("Flashing wrong logo image may brick your device!\n\nReally continue?")) == QMessageBox::Yes)
	{
		open_text = tr("Flash Logo Image");

		if(on_actionOpen_triggered())
		{
			flashDialog(this).exec();
		}
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
	if(QMessageBox::question(this, APPNAME, tr("Really exit program?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		ce->accept();
	}
	else
	{
		ce->ignore();
	}
}
