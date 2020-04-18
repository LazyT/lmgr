#include "about.h"

aboutDialog::aboutDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);
	setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

	label_Name->setText(QString("%1").arg(APPNAME));
	label_Version->setText(tr("Version %1 - %2").arg(APPVERS, APPDATE));

	textEdit->setAlignment(Qt::AlignCenter);
	textEdit->textCursor().insertText(tr("This program is Freeware and may be installed and used free of charge for non-commercial use on as many computers as you like without limitations.\n\nA liability for any damages resulting from the use is excluded. Use at your own risk!"));
	textEdit->moveCursor(QTextCursor::Start);
	textEdit->setFixedHeight(5 * textEdit->fontMetrics().height() + textEdit->document()->documentMargin() + 2);
}

void aboutDialog::mouseReleaseEvent(QMouseEvent *me)
{
	QWidget *child = QWidget::childAt(me->pos());

	if(child)
	{
		QString name = child->objectName();

		if(name == "label_Mail")
		{
			QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("bWFpbHRvOkxhenlUQG1haWxib3gub3JnP3N1YmplY3Q9TE1HUiZib2R5PVdyaXRlIGluIEVuZ2xpc2ggb3IgR2VybWFuIHBsZWFzZS4uLg==")));
		}
		else if(name == "label_Forum")
		{
			QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("aHR0cHM6Ly9mb3J1bS54ZGEtZGV2ZWxvcGVycy5jb20vazIwLXByby90aGVtZXMvYXBwLWxvZ28tbWFuYWdlci1zcGxhc2gtc2NyZWVucy10NDA4NDQ1NQ==")));
		}
		else if(name == "label_Donation")
		{
			if(QMessageBox::warning(this, APPNAME, tr("Please note the following points:\n\n* The payment is made voluntarily without the acquisition of claims.\n* You receive no rights to the offered software.\n* Because this is not a donation in juridical sense no certificate can be issued.\n\nWould you like to support the further development of this project nevertheless?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
			{
				QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("aHR0cHM6Ly93d3cucGF5cGFsLmNvbS9jZ2ktYmluL3dlYnNjcj9jbWQ9X2RvbmF0aW9ucyZidXNpbmVzcz1MYXp5VEBtYWlsYm94Lm9yZyZpdGVtX25hbWU9TG9nbyUyME1hbmFnZXImYW1vdW50PTAmY3VycmVuY3lfY29kZT1FVVI=")));
			}
		}
	}
}
