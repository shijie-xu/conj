#pragma once

#include <QObject>

// Errs when Qt encounters python
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

class QWord : public QObject
{
	Q_OBJECT
private:
	QString buf;
public:
	QWord(QObject *parent);
	~QWord();

	QString stem;
	QString cat;
};

class QSentence {
private:
	QString buf;
	PyObject* pv;
public:
	QSentence();
	~QSentence();

	// parse sentence structure
	void set_setence(QString str) {
		buf = str;
	}
	void init_pyengine();
	void exit_pyengine();
	void py_parse_sentence_tags();
	QList<QString> py_fold_sentence();
};
