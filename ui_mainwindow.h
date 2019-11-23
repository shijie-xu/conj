/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSettings_S;
    QAction *actionExit_X;
    QAction *actionAbout_A;
    QAction *actionNew_Quiz_N;
    QAction *actionFetch_Dictionary_F;
    QAction *actionPlay_word_W;
    QAction *actionPlay_sentence_S;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chk_word;
    QPushButton *btn_play_word;
    QVBoxLayout *verticalLayout_2;
    QLabel *lbl_word;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_tense;
    QLineEdit *le_input;
    QListWidget *lst_history;
    QWidget *tab_words;
    QHBoxLayout *horizontalLayout_15;
    QWebEngineView *webEngineView;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *hb_sta;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *tbl_pronom;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *chk_show_pronoms;
    QLabel *lbl_pronom_pos;
    QLineEdit *le_pronom;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_13;
    QListWidget *lst_words;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_12;
    QLabel *lbl_words;
    QLabel *lbl_complete_info;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label;
    QSpinBox *spin_freq;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_2;
    QSpinBox *spin_min;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_3;
    QSpinBox *spin_max;
    QTextEdit *te_sentence;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btn_ok;
    QPushButton *btn_play;
    QPushButton *btn_clear;
    QVBoxLayout *verticalLayout_4;
    QLabel *lbl_origin;
    QLabel *lbl_sent;
    QLabel *lbl_trans;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_16;
    QHBoxLayout *hb_history;
    QMenuBar *menubar;
    QMenu *menuFile_F;
    QMenu *menuHelp_H;
    QStatusBar *statusbar;
    QButtonGroup *buttonGroup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(370, 301);
        actionSettings_S = new QAction(MainWindow);
        actionSettings_S->setObjectName(QString::fromUtf8("actionSettings_S"));
        actionExit_X = new QAction(MainWindow);
        actionExit_X->setObjectName(QString::fromUtf8("actionExit_X"));
        actionAbout_A = new QAction(MainWindow);
        actionAbout_A->setObjectName(QString::fromUtf8("actionAbout_A"));
        actionNew_Quiz_N = new QAction(MainWindow);
        actionNew_Quiz_N->setObjectName(QString::fromUtf8("actionNew_Quiz_N"));
        actionFetch_Dictionary_F = new QAction(MainWindow);
        actionFetch_Dictionary_F->setObjectName(QString::fromUtf8("actionFetch_Dictionary_F"));
        actionPlay_word_W = new QAction(MainWindow);
        actionPlay_word_W->setObjectName(QString::fromUtf8("actionPlay_word_W"));
        actionPlay_sentence_S = new QAction(MainWindow);
        actionPlay_sentence_S->setObjectName(QString::fromUtf8("actionPlay_sentence_S"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        chk_word = new QCheckBox(tab);
        chk_word->setObjectName(QString::fromUtf8("chk_word"));
        chk_word->setChecked(false);

        horizontalLayout_2->addWidget(chk_word);

        btn_play_word = new QPushButton(tab);
        btn_play_word->setObjectName(QString::fromUtf8("btn_play_word"));
        btn_play_word->setStyleSheet(QString::fromUtf8("QPushButton#btn_play_word {\n"
"    background-color: rgb(255,255,240);\n"
"    border-style: outset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    font: bold 14px;\n"
"    padding: 6px;\n"
"}\n"
"QPushButton#btn_play_word:pressed {\n"
"    background-color: rgb(250,250,210);\n"
"    border-style: inset;\n"
"}"));

        horizontalLayout_2->addWidget(btn_play_word);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lbl_word = new QLabel(tab);
        lbl_word->setObjectName(QString::fromUtf8("lbl_word"));
        QFont font;
        font.setFamily(QString::fromUtf8("Cascadia Code"));
        font.setPointSize(18);
        lbl_word->setFont(font);
        lbl_word->setStyleSheet(QString::fromUtf8("QLabel#lbl_word {\n"
"    background-color: rgb(	255,235,205);\n"
"    border-radius: 15px;\n"
"}"));
        lbl_word->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lbl_word);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbl_tense = new QLabel(tab);
        lbl_tense->setObjectName(QString::fromUtf8("lbl_tense"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cascadia Code"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setWeight(50);
        lbl_tense->setFont(font1);

        horizontalLayout->addWidget(lbl_tense);

        le_input = new QLineEdit(tab);
        le_input->setObjectName(QString::fromUtf8("le_input"));
        le_input->setFont(font1);
        le_input->setStyleSheet(QString::fromUtf8("QLineEdit#le_input {\n"
"    border-style: inset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    padding: 4px;\n"
"}\n"
""));

        horizontalLayout->addWidget(le_input);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        lst_history = new QListWidget(tab);
        lst_history->setObjectName(QString::fromUtf8("lst_history"));
        lst_history->setStyleSheet(QString::fromUtf8("QListWidget#lst_history {\n"
"    border-style: inset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QListWidget::item:selected {\n"
"	background: rgb(135,206,235);\n"
"}\n"
""));

        horizontalLayout_3->addWidget(lst_history);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab, QString());
        tab_words = new QWidget();
        tab_words->setObjectName(QString::fromUtf8("tab_words"));
        horizontalLayout_15 = new QHBoxLayout(tab_words);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        webEngineView = new QWebEngineView(tab_words);
        webEngineView->setObjectName(QString::fromUtf8("webEngineView"));
        webEngineView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        horizontalLayout_15->addWidget(webEngineView);

        tabWidget->addTab(tab_words, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_5 = new QHBoxLayout(tab_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        hb_sta = new QHBoxLayout();
        hb_sta->setObjectName(QString::fromUtf8("hb_sta"));

        horizontalLayout_5->addLayout(hb_sta);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_7 = new QHBoxLayout(tab_2);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        tbl_pronom = new QTableWidget(tab_2);
        tbl_pronom->setObjectName(QString::fromUtf8("tbl_pronom"));

        verticalLayout_6->addWidget(tbl_pronom);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        chk_show_pronoms = new QCheckBox(tab_2);
        chk_show_pronoms->setObjectName(QString::fromUtf8("chk_show_pronoms"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Cascadia Code"));
        font2.setPointSize(8);
        chk_show_pronoms->setFont(font2);

        horizontalLayout_6->addWidget(chk_show_pronoms);

        lbl_pronom_pos = new QLabel(tab_2);
        lbl_pronom_pos->setObjectName(QString::fromUtf8("lbl_pronom_pos"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Cascadia Code"));
        font3.setPointSize(12);
        lbl_pronom_pos->setFont(font3);

        horizontalLayout_6->addWidget(lbl_pronom_pos);

        le_pronom = new QLineEdit(tab_2);
        le_pronom->setObjectName(QString::fromUtf8("le_pronom"));
        le_pronom->setFont(font3);

        horizontalLayout_6->addWidget(le_pronom);


        verticalLayout_6->addLayout(horizontalLayout_6);


        horizontalLayout_7->addLayout(verticalLayout_6);

        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_14 = new QHBoxLayout(tab_4);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        lst_words = new QListWidget(tab_4);
        lst_words->setObjectName(QString::fromUtf8("lst_words"));
        lst_words->setFont(font1);
        lst_words->setStyleSheet(QString::fromUtf8("QListWidget#lst_words {\n"
"    border-style: inset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QListWidget::item:selected {\n"
"	background: rgb(135,206,235);\n"
"}\n"
""));

        horizontalLayout_13->addWidget(lst_words);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        lbl_words = new QLabel(tab_4);
        lbl_words->setObjectName(QString::fromUtf8("lbl_words"));
        lbl_words->setFont(font3);
        lbl_words->setStyleSheet(QString::fromUtf8("QLabel#lbl_words {\n"
"    background-color: rgb(255,255,240);\n"
"    border-radius: 5px;\n"
"}"));

        horizontalLayout_12->addWidget(lbl_words);

        lbl_complete_info = new QLabel(tab_4);
        lbl_complete_info->setObjectName(QString::fromUtf8("lbl_complete_info"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Cascadia Code"));
        font4.setPointSize(10);
        lbl_complete_info->setFont(font4);
        lbl_complete_info->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_12->addWidget(lbl_complete_info);


        verticalLayout_5->addLayout(horizontalLayout_12);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label = new QLabel(tab_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font4);

        horizontalLayout_9->addWidget(label);

        spin_freq = new QSpinBox(tab_4);
        spin_freq->setObjectName(QString::fromUtf8("spin_freq"));
        spin_freq->setStyleSheet(QString::fromUtf8("QSpinBox#spin_freq {\n"
"    background-color: rgb(255,255,240);\n"
"    border-style: inset;\n"
"    border-radius: 5px;\n"
"    border-width: 1px;\n"
"    border-color: rgb(107,142,35);\n"
"}"));
        spin_freq->setMaximum(100000);
        spin_freq->setValue(50);
        spin_freq->setDisplayIntegerBase(10);

        horizontalLayout_9->addWidget(spin_freq);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_2 = new QLabel(tab_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font4);

        horizontalLayout_10->addWidget(label_2);

        spin_min = new QSpinBox(tab_4);
        spin_min->setObjectName(QString::fromUtf8("spin_min"));
        spin_min->setStyleSheet(QString::fromUtf8("QSpinBox#spin_min {\n"
"    background-color: rgb(255,255,240);\n"
"    border-style: inset;\n"
"    border-radius: 5px;\n"
"    border-width: 1px;\n"
"    border-color: rgb(107,142,35);\n"
"}"));
        spin_min->setValue(3);

        horizontalLayout_10->addWidget(spin_min);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_3 = new QLabel(tab_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font4);

        horizontalLayout_11->addWidget(label_3);

        spin_max = new QSpinBox(tab_4);
        spin_max->setObjectName(QString::fromUtf8("spin_max"));
        QFont font5;
        font5.setPointSize(8);
        spin_max->setFont(font5);
        spin_max->setStyleSheet(QString::fromUtf8("QSpinBox#spin_max {\n"
"    background-color: rgb(255,255,240);\n"
"    border-style: inset;\n"
"    border-radius: 5px;\n"
"    border-width: 1px;\n"
"    border-color: rgb(107,142,35);\n"
"}"));
        spin_max->setValue(5);

        horizontalLayout_11->addWidget(spin_max);


        verticalLayout_5->addLayout(horizontalLayout_11);


        verticalLayout_7->addLayout(verticalLayout_5);

        te_sentence = new QTextEdit(tab_4);
        te_sentence->setObjectName(QString::fromUtf8("te_sentence"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Cascadia Code"));
        font6.setPointSize(14);
        font6.setBold(false);
        font6.setWeight(50);
        te_sentence->setFont(font6);
        te_sentence->setStyleSheet(QString::fromUtf8("QTextEdit#te_sentence {\n"
"    border-style: inset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    padding: 4px;\n"
"}\n"
""));

        verticalLayout_7->addWidget(te_sentence);


        horizontalLayout_13->addLayout(verticalLayout_7);


        verticalLayout_8->addLayout(horizontalLayout_13);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btn_ok = new QPushButton(tab_4);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        QFont font7;
        font7.setBold(true);
        font7.setItalic(false);
        font7.setWeight(75);
        btn_ok->setFont(font7);
        btn_ok->setAcceptDrops(false);
        btn_ok->setStyleSheet(QString::fromUtf8("QPushButton#btn_ok {\n"
"    background-color: rgb(255,255,240);\n"
"    border-style: outset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    font: bold 14px;\n"
"    padding: 6px;\n"
"}\n"
"QPushButton#btn_ok:pressed {\n"
"    background-color: rgb(250,250,210);\n"
"    border-style: inset;\n"
"}"));

        horizontalLayout_8->addWidget(btn_ok);

        btn_play = new QPushButton(tab_4);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        btn_play->setFont(font7);
        btn_play->setStyleSheet(QString::fromUtf8("QPushButton#btn_play{\n"
"    background-color: rgb(176,224,230);\n"
"    border-style: outset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    font: bold 14px;\n"
"    padding: 6px;\n"
"}\n"
"QPushButton#btn_play:pressed {\n"
"    background-color: rgb(135,206,235);\n"
"    border-style: inset;\n"
"}"));

        horizontalLayout_8->addWidget(btn_play);

        btn_clear = new QPushButton(tab_4);
        btn_clear->setObjectName(QString::fromUtf8("btn_clear"));
        btn_clear->setStyleSheet(QString::fromUtf8("QPushButton#btn_clear {\n"
"    background-color: rgb(176,224,230);\n"
"    border-style: outset;\n"
"    border-width: 2px;\n"
"    border-radius: 10px;\n"
"    border-color: rgb(107,142,35);\n"
"    font: bold 14px;\n"
"    padding: 6px;\n"
"}\n"
"QPushButton#btn_clear:pressed {\n"
"    background-color: rgb(135,206,235);\n"
"    border-style: inset;\n"
"}"));

        horizontalLayout_8->addWidget(btn_clear);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lbl_origin = new QLabel(tab_4);
        lbl_origin->setObjectName(QString::fromUtf8("lbl_origin"));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Cascadia Code"));
        font8.setPointSize(10);
        font8.setBold(false);
        font8.setWeight(50);
        lbl_origin->setFont(font8);
        lbl_origin->setStyleSheet(QString::fromUtf8("QLabel#lbl_origin {\n"
"    background-color: rgb(	255,235,205);\n"
"    border-radius: 5px;\n"
"}"));

        verticalLayout_4->addWidget(lbl_origin);

        lbl_sent = new QLabel(tab_4);
        lbl_sent->setObjectName(QString::fromUtf8("lbl_sent"));
        lbl_sent->setFont(font8);
        lbl_sent->setStyleSheet(QString::fromUtf8("QLabel#lbl_sent {\n"
"    background-color: rgb(72,209,204);\n"
"    border-radius: 5px;\n"
"}"));

        verticalLayout_4->addWidget(lbl_sent);

        lbl_trans = new QLabel(tab_4);
        lbl_trans->setObjectName(QString::fromUtf8("lbl_trans"));
        QFont font9;
        font9.setFamily(QString::fromUtf8("Cascadia Code"));
        font9.setPointSize(10);
        font9.setBold(false);
        font9.setItalic(true);
        font9.setWeight(50);
        lbl_trans->setFont(font9);
        lbl_trans->setStyleSheet(QString::fromUtf8("QLabel#lbl_trans {\n"
"    background-color: rgb(255,192,203);\n"
"    border-radius: 5px;\n"
"}"));

        verticalLayout_4->addWidget(lbl_trans);


        horizontalLayout_8->addLayout(verticalLayout_4);


        verticalLayout_8->addLayout(horizontalLayout_8);


        horizontalLayout_14->addLayout(verticalLayout_8);

        tabWidget->addTab(tab_4, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        horizontalLayout_16 = new QHBoxLayout(tab_6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        hb_history = new QHBoxLayout();
        hb_history->setObjectName(QString::fromUtf8("hb_history"));

        horizontalLayout_16->addLayout(hb_history);

        tabWidget->addTab(tab_6, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 370, 18));
        menuFile_F = new QMenu(menubar);
        menuFile_F->setObjectName(QString::fromUtf8("menuFile_F"));
        menuHelp_H = new QMenu(menubar);
        menuHelp_H->setObjectName(QString::fromUtf8("menuHelp_H"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile_F->menuAction());
        menubar->addAction(menuHelp_H->menuAction());
        menuFile_F->addAction(actionNew_Quiz_N);
        menuFile_F->addAction(actionFetch_Dictionary_F);
        menuFile_F->addSeparator();
        menuFile_F->addAction(actionSettings_S);
        menuFile_F->addAction(actionExit_X);
        menuHelp_H->addAction(actionAbout_A);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        btn_ok->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "French Conjugator v0.1", nullptr));
        actionSettings_S->setText(QCoreApplication::translate("MainWindow", "Settings(&S)", nullptr));
        actionExit_X->setText(QCoreApplication::translate("MainWindow", "Exit(&X)", nullptr));
        actionAbout_A->setText(QCoreApplication::translate("MainWindow", "About(&A)", nullptr));
        actionNew_Quiz_N->setText(QCoreApplication::translate("MainWindow", "New Quiz(&N)", nullptr));
        actionFetch_Dictionary_F->setText(QCoreApplication::translate("MainWindow", "Fetch Dictionary(&F)", nullptr));
        actionPlay_word_W->setText(QCoreApplication::translate("MainWindow", "Play word(&W)", nullptr));
        actionPlay_sentence_S->setText(QCoreApplication::translate("MainWindow", "Play sentence(&S)", nullptr));
        chk_word->setText(QCoreApplication::translate("MainWindow", "read word/sentence", nullptr));
        btn_play_word->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        lbl_word->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        lbl_tense->setText(QCoreApplication::translate("MainWindow", "(pr\303\251sent): ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Conj.", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_words), QCoreApplication::translate("MainWindow", "Words", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Prog.", nullptr));
        chk_show_pronoms->setText(QCoreApplication::translate("MainWindow", "Show/Hide Pronoms Table", nullptr));
        lbl_pronom_pos->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Pronom", nullptr));
        lbl_words->setText(QCoreApplication::translate("MainWindow", "Complete", nullptr));
        lbl_complete_info->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Avg freqency of words", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Min sentence length", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Max sentence length", nullptr));
        btn_ok->setText(QCoreApplication::translate("MainWindow", "Check", nullptr));
        btn_play->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        btn_clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        lbl_origin->setText(QString());
        lbl_sent->setText(QString());
        lbl_trans->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Complete", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("MainWindow", "History", nullptr));
        menuFile_F->setTitle(QCoreApplication::translate("MainWindow", "File(&F)", nullptr));
        menuHelp_H->setTitle(QCoreApplication::translate("MainWindow", "Help(&H)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
