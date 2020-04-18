#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QWidget>
#include <QTextBrowser>
#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
public:

	HelpBrowser(QWidget *parent = NULL) : QTextBrowser(parent) {};

	void setHelpEngine(QHelpEngine *engine) { helpEngine = engine; };

	QVariant loadResource(int type, const QUrl &name) { return (name.scheme() == "qthelp" ? QVariant(helpEngine->fileData(name)) : QTextBrowser::loadResource(type, name)); }

private:

	QHelpEngine *helpEngine;
};

#endif // HELPBROWSER_H
