#include "help.h"

helpDialog::helpDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	lng = QFile::exists(QString("%1/hlp/%2.qhc").arg(QApplication::applicationDirPath()).arg(QLocale::system().name().mid(0, 2))) ? QLocale::system().name().mid(0, 2) : "en";

	helpEngine = new QHelpEngine(QString("%1/hlp/%2.qhc").arg(QApplication::applicationDirPath()).arg(lng));
	helpEngine->setupData();

	contentWidget = helpEngine->contentWidget();
	verticalLayout->addWidget((QWidget*)contentWidget);

	helpBrowser->setHelpEngine(helpEngine);
	helpBrowser->setSource(QUrl(QString("qthelp://de.lazyt.lmgr/help/%1.html").arg(lng)));

	connect(contentWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(setSourceFromContent(QModelIndex)));
	connect(contentWidget, SIGNAL(activated(QModelIndex)), this, SLOT(setSourceFromContent(QModelIndex)));

	QTimer::singleShot(100, this, SLOT(expand()));
}

void helpDialog::setSourceFromContent(QModelIndex index)
{
	helpBrowser->setSource(helpEngine->contentModel()->contentItemAt(index)->url());
}

void helpDialog::expand()
{
	contentWidget->expandAll();
}
