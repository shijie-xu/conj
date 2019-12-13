#include "QWord.h"
#include <QDebug>
#include <QString>


QWord::QWord(QObject *parent)
	: QObject(parent)
{
}

QWord::~QWord()
{
}


QSentence::QSentence()
{
}

QSentence::~QSentence()
{

}

void QSentence::init_pyengine()
{
	Py_Initialize();
	qDebug() << "parse " << buf;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./pyscripts/')");
	PyObject* module_name = PyUnicode_FromString("parselib");
	PyObject* pModule = PyImport_Import(module_name);
	pv = PyObject_GetAttrString(pModule, "fold_sentence");
	qDebug() << "initialized func ok";
}

void QSentence::exit_pyengine()
{
	Py_Finalize();
}

void QSentence::py_parse_sentence_tags()
{
	PyObject* args = PyTuple_New(1);
	//PyObject* arg1 = Py_BuildValue("s", buf.toLocal8Bit().data());
	// When I read from file, toLatin1() is required.
	PyObject* arg1 = Py_BuildValue("s", buf.toLatin1().data());
	PyTuple_SetItem(args, 0, arg1);
	PyObject* pRet = PyObject_CallObject(pv, args);
	qDebug() << "call ok";
	for (int i = 0; i < PyList_GET_SIZE(pRet); i++) {
		PyObject* p = PyList_GetItem(pRet, i);
		QString word = _PyUnicode_AsString(PyTuple_GetItem(p, 0));
		QString prop =  _PyUnicode_AsString(PyTuple_GetItem(p, 1));
		/*if (prop == "V") {
			PyRun_SimpleString("from pattern.fr import conjugate");
			QString str = "conjugate(" + word + ")";
			PyRun_SimpleString(str.toLocal8Bit().data());
		}*/
		qDebug() << word << prop;
	}
}

QList<QString> QSentence::py_fold_sentence()
{
	qDebug() << "initialize fold";
	PyObject* args = PyTuple_New(1);
	// PyObject* arg1 = Py_BuildValue("s", buf.toLocal8Bit().data());
	// When I read from file, toLatin1() is required.
	PyObject* arg1 = Py_BuildValue("s", buf.toLatin1().data());
	PyTuple_SetItem(args, 0, arg1);
	PyObject* pRet = PyObject_CallObject(pv, args);
	qDebug() << "call ok";
	// 
	QList<QString> ret = {};
	for (int i = 0; i < PyList_GET_SIZE(pRet); i++) {
		PyObject* p = PyList_GetItem(pRet, i);
		//QString word = _PyUnicode_AsString(PyTuple_GetItem(p, 0));
		//QString prop = _PyUnicode_AsString(PyTuple_GetItem(p, 1));
		QString word = _PyUnicode_AsString(p);
		//qDebug() << word;// << prop;
		ret.append(word);
	}
	return ret;
}
