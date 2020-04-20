#include "flash.h"

flashDialog::flashDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	QTimer::singleShot(250, this, SLOT(startFlashing()));
}

void flashDialog::prg_errorOccurred(__attribute__((unused)) QProcess::ProcessError error)
{
	QMessageBox::critical(this, APPNAME,tr("Could not run command!\n\n%1").arg(process->errorString()));

	failed = true;
	running = false;
}

void flashDialog::prg_readyReadStandardOutput()
{
	QString response = process->readAll();

	response.replace("\r\n", "\n");

	if(response != "\r\n" && response != "\n")
	{
		process_output = response;

		if(!response.endsWith("\n"))
		{
			response.append("\n");
		}

		if(response.contains("error") || response.contains("FAILED") || response.contains("unauthorized") || response.contains("permissions"))
		{
			textcolor = textEdit->textColor();

			textEdit->setTextColor(Qt::red);

			textEdit->append(response.replace("\n\n", "\n"));

			textEdit->setTextColor(textcolor);
		}
		else
		{
			textEdit->append(response.replace("\n\n", "\n"));
		}
	}
}

void flashDialog::prg_finished(__attribute__((unused)) int exitCode, __attribute__((unused)) QProcess::ExitStatus exitStatus)
{
	running = false;
}

void flashDialog::sendCommand(QString cmd)
{
	textcolor = textEdit->textColor();
	fontweight = textEdit->fontWeight();

	textEdit->setTextColor(Qt::blue);
	textEdit->setFontWeight(QFont::Bold);

	textEdit->append(cmd);

	textEdit->setTextColor(textcolor);
	textEdit->setFontWeight(fontweight);

	failed = false;
	running = true;

	process->start(QString("%1/%2").arg(QApplication::applicationDirPath()).arg(cmd));

	while(running)
	{
		QApplication::processEvents();
	}
}

void flashDialog::startFlashing()
{
	process = new QProcess(this);

	process->setProcessChannelMode(QProcess::MergedChannels);

	connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(prg_errorOccurred(QProcess::ProcessError)));
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(prg_readyReadStandardOutput()));
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(prg_finished(int, QProcess::ExitStatus)));

	buttonBox->button(QDialogButtonBox::Retry)->setEnabled(false);

	sendCommand("adb devices");

	if(process_output.contains("List of devices attached\n\n"))
	{
		label_adb->setPixmap(QPixmap(":/png/png/con-err.png"));

		buttonBox->button(QDialogButtonBox::Retry)->setEnabled(true);

		QMessageBox::warning(this, APPNAME, tr("No device found!\n\nConnect device via usb and try again..."));
	}
	else if(process_output.contains("unauthorized"))
	{
		label_serial->setText(process_output.split("\n").at(1).split("\t").at(0));

		label_adb->setPixmap(QPixmap(":/png/png/con-err.png"));

		buttonBox->button(QDialogButtonBox::Retry)->setEnabled(true);

		QMessageBox::warning(this, APPNAME, tr("Unauthorized device found!\n\nAuthorize on phone and try again..."));
	}
	else if(process_output.contains("permissions"))
	{
		label_serial->setText(process_output.split("\n").at(1).split("\t").at(0));

		label_adb->setPixmap(QPixmap(":/png/png/con-err.png"));

		buttonBox->button(QDialogButtonBox::Retry)->setEnabled(true);

		QMessageBox::warning(this, APPNAME, tr("No permissions for device!\n\nFix and try again..."));
	}
	else if(process_output.contains("List of devices attached\n"))
	{
		label_serial->setText(process_output.split("\n").at(1).split("\t").at(0));

		label_adb->setPixmap(QPixmap(":/png/png/con-suc.png"));

		sendCommand("adb shell getprop ro.product.model");

		label_model->setText(process_output.simplified());

		sendCommand("adb shell getprop ro.product.device");

		label_device->setText(process_output.simplified());

		foreach(QString device, devices)
		{
			supported = false;

			if(device == label_device->text())
			{
				supported = true;

				break;
			}
		}

		if(!supported)
		{
			QMessageBox::warning(this, APPNAME, tr("Unsupported device detected!\n\nUse at your own risk and report model if succesfully..."));
		}

		if(QMessageBox::question(this, APPNAME, tr("Start fastboot to flash logo image?")) == QMessageBox::Yes)
		{
			sendCommand("adb reboot-bootloader");

			textEdit->append("\nWaiting 5 seconds for device to finish reboot...\n");

			QCoreApplication::processEvents();

			QThread::msleep(5000);

			label_fbt->setPixmap(QPixmap(":/png/png/con-err.png"));

			abort = false;

			do
			{
				QThread::msleep(1000);

				sendCommand("fastboot devices");
			}
			while(!abort && (process_output.isEmpty() || !process_output.contains("\tfastboot\n")));

			if(abort)
			{
				QMessageBox::warning(this, APPNAME, tr("Waiting for fastboot aborted!"));
			}
			else
			{
				label_fbt->setPixmap(QPixmap(":/png/png/con-suc.png"));

				sendCommand("fastboot flash logo " + reinterpret_cast<MainWindow*>(parent())->flash_file);

				if(process_output.contains("error") || process_output.contains("FAILED"))
				{
					QMessageBox::warning(this, APPNAME, tr("Flashing logo image failed!\n\nSee log for details..."));
				}
				else
				{
					QMessageBox::warning(this, APPNAME, tr("Flashing logo image finished.\n\nClick OK to reboot device..."));
				}

				sendCommand("fastboot reboot");
			}
		}
	}
	else if(!failed)
	{
		QMessageBox::warning(this, APPNAME, tr("Unexpected response from device!"));
	}
}

void flashDialog::accept()
{
	startFlashing();
}

void flashDialog::reject()
{
	if(running)
	{
		if(QMessageBox::question(this, APPNAME, tr("Really abort running command?")) == QMessageBox::Yes)
		{
			abort = true;
			running = false;
		}
	}
	else
	{
		if(!failed)
		{
			sendCommand("adb kill-server");
		}

		QDialog::reject();
	}
}
