#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPlainTextEdit>
#include <QSettings>
#include <QVariant>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

//#define SLOT(name) "1"#name
//#define SIGNAL(name) "2"#name

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->setHtml(tr("<font color=\"red\">%1</font>").arg("être"));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSettings_S_triggered()
{
    this->setting_dialog = new QDialog;
    setting_dialog->setModal(true);
    setting_dialog->setWindowTitle(tr("Settings"));
    setting_dialog->resize(400,300);
    setting_dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Add vertical layout
    QHBoxLayout *hb_ind = new QHBoxLayout();
    QHBoxLayout *hb_cond = new QHBoxLayout();
    QHBoxLayout *hb_sub = new QHBoxLayout();
    QHBoxLayout *hb_ok = new QHBoxLayout();
    QVBoxLayout *vb_total = new QVBoxLayout();

    // Add check box
    // indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait', 'futur-simple', 'futur-antérieur'
    // conditionnel_teneses = 'présent', 'passé',
    // subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
    QCheckBox *chk_ind_present = new QCheckBox();
    chk_ind_present->setText("présent");
    QCheckBox *chk_ind_passe_compose = new QCheckBox();
    chk_ind_passe_compose->setText("passé-composé");
    QCheckBox *chk_ind_imparfait = new QCheckBox();
    chk_ind_imparfait->setText("imparfait");
    QCheckBox *chk_ind_plus_que_parfait = new QCheckBox();
    chk_ind_plus_que_parfait->setText("plus-que-parfait");
    QCheckBox *chk_ind_futur_simple = new QCheckBox();
    chk_ind_futur_simple->setText("futur-simple");
    QCheckBox *chk_ind_futur_anterieur = new QCheckBox();
    chk_ind_futur_anterieur->setText("futur-antérieur");

    QCheckBox *chk_cond_present = new QCheckBox();
    chk_cond_present->setText("présent");
    QCheckBox *chk_cond_passe = new QCheckBox();
    chk_cond_passe->setText("passé");

    QCheckBox *chk_sub_present = new QCheckBox();
    chk_sub_present->setText("présent");
    QCheckBox *chk_sub_passe = new QCheckBox();
    chk_sub_passe->setText("passé");
    QCheckBox *chk_sub_imparfait = new QCheckBox();
    chk_sub_imparfait->setText("imparfait");
    QCheckBox *chk_sub_plus_que_parfait = new QCheckBox();
    chk_sub_plus_que_parfait->setText("plus-que-parfait");

    // Add checkbox to layout
    // Add label to layout
    QLabel *lbl_ind = new QLabel("<b>indicatif</b>");
    QLabel *lbl_cond = new QLabel("<b>conditionnel</b>");
    QLabel *lbl_sub = new QLabel("<b>subjonctif</b>");
    hb_ind->addWidget(lbl_ind);
    hb_cond->addWidget(lbl_cond);
    hb_sub->addWidget(lbl_sub);

    hb_ind->addWidget(chk_ind_present);
    hb_ind->addWidget(chk_ind_passe_compose);
    hb_ind->addWidget(chk_ind_imparfait);
    hb_ind->addWidget(chk_ind_plus_que_parfait);
    hb_ind->addWidget(chk_ind_futur_simple);
    hb_ind->addWidget(chk_ind_futur_anterieur);
    hb_ind->addStretch();


    hb_cond->addWidget(chk_cond_present);
    hb_cond->addWidget(chk_cond_passe);
    hb_cond->addStretch();

    hb_sub->addWidget(chk_sub_present);
    hb_sub->addWidget(chk_sub_passe);
    hb_sub->addWidget(chk_sub_imparfait);
    hb_sub->addWidget(chk_sub_plus_que_parfait);
    hb_sub->addStretch();

    // Add button
    QPushButton *btn_save = new QPushButton();
    btn_save->setText("Save");
    connect(btn_save,SIGNAL(clicked()), this, SLOT(on_settings_save));
    QPushButton *btn_cancel = new QPushButton();
    btn_cancel->setText("Cancel");
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(on_settings_cancel()));


    // Add spacer
    hb_ok->addStretch();
    hb_ok->addWidget(btn_save);
    hb_ok->addWidget(btn_cancel);
    // Add all hbox to vbox
    vb_total->addLayout(hb_ind);
    vb_total->addLayout(hb_cond);
    vb_total->addLayout(hb_sub);
    vb_total->addLayout(hb_ok);
    vb_total->addStretch();

    setting_dialog->setLayout(vb_total);

    setting_dialog->show();
}

void MainWindow::on_actionExit_X_triggered()
{
    this->close();
}

void MainWindow::on_settings_cancel()
{
    QMessageBox::StandardButton result = QMessageBox::information(
                setting_dialog, "Settings", "Are your sure to cancel?",
                QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (result==QMessageBox::Yes)
        this->setting_dialog->close();
}

void MainWindow::on_settings_save()
{
}

void MainWindow::on_actionAbout_A_triggered()
{
    QMessageBox::information(
                this, "About us",
                "<b>French Conjugator v1.0</b> aims to help people who suffer from the disgusting verb conjugations in French laguage in a related easy way.<br>"
                "Allrights reserved © Shi-Jie Xu, see <a href=\"https://github.com/shijie-xu\">my homepage</a> for more details.",
                QMessageBox::Yes, QMessageBox::Yes);
}
