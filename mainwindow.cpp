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
#include <QLineEdit>
#include <QSpacerItem>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QListView>
#include <QTableWidget>
#include <QRandomGenerator>
#include <QSpinBox>
#include <QList>
#include <QUrl>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtTextToSpeech>
#include <QLocale>
#include <QShortcut>
#include <QKeySequence>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_input->setText(tr("%1").arg("être"));

    // Set windows title
    this->setWindowTitle("French Conjugator v0.1");

    // Set random seed
    // Since QRandomGenerator use facilities provide by OS,
    // no need produces for seed here.
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Read settings and set window
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    this->move(settings->value("pos").toPoint());
    this->resize(settings->value("size").toSize());
    this->words_file_path = settings->value("words file path").toString();
    this->quiz_cycles = settings->value("quiz cycles").toInt();
    this->quiz_rate = settings->value("quiz rate").toInt();
    this->quiz_new_rate = settings->value("quiz new rate").toInt();
    this->current_study = settings->value("current study").toInt();

    this->bshow = true;

    // Set passed cycles
    passed_cycles = 0;
    right_cycles = 0;

    // Set status bar
    this->progress = new QProgressBar();
    progress->setRange(0, quiz_cycles);
    ui->statusbar->addWidget(progress);
    this->lbl_status = new QLabel("French Conjugator v0.1\t");
    ui->statusbar->addWidget(lbl_status);
    this->lbl_network = new QLabel();
    ui->statusbar->addWidget(lbl_network);

    // Set tts
    this->speech = new QTextToSpeech();
    this->speech->setLocale(QLocale::French);

    // Set shortcuts
    // F5 for play word
    QShortcut *sc_play = new QShortcut(QKeySequence("F5"), this);
    connect(sc_play, SIGNAL(activated()), this, SLOT(on_btn_play_word_clicked()));
    QShortcut *sc_hide = new QShortcut(QKeySequence("F9"), this);
    connect(sc_hide, SIGNAL(activated()), this, SLOT(on_hide_window()));

    // Load words file
    QFile src(words_file_path);
    if(!src.open(QFile::ReadWrite)){
        QMessageBox::warning(this, "Conj", "Cannot load words file!");
    }
    this->words_list = QJsonDocument::fromJson(
                QString(src.readAll()).toUtf8()).array();
    words_count = words_list.count();
    // NOTICE: Unccessary komma is NOT permitde!
    // You must write word json file like this:
    // ["word1", "word2", ..., "wordn"]

    //qDebug() << words_file_path << words_list << words_count;

    // Set words study
    QFile fileStudy("study.json");
    if(fileStudy.open(QFile::ReadOnly)){
        QJsonDocument doc = QJsonDocument::fromJson(fileStudy.readAll());
        study_history = map_read_from_variant(doc.toVariant().toMap());
    }else{
        QMessageBox::warning(this, "Conj", "Study history not existed.");
    }

    // Set proun
    this->prouns << "je" << "tu" << "il"
                << "nous" << "vous" << "ils";

    // Init tenses
    // indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait', 'futur-simple', 'futur-antérieur'
    // conditionnel_teneses = 'présent', 'passé',
    // subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
    QList<QString> tenses = {
        "indicatif:présent", "indicatif:passé-composé",
        "indicatif:imparfait", "indicatif:plus-que-parfait",
        "indicatif:futur-simple", "indicatif:futur-antérieur",

        "conditionnel:présent", "conditionnel:passé",

        "subjonctif:présent","subjonctif:passé",
        "subjonctif:imparfait","subjonctif:plus-que-parfait"
    };
    for (int i = 0; i < tenses.count(); ++i) {
        QString tense = tenses.at(i);
        tenses_switch.insert(tense, settings->value(tense).toBool());
    }
    // Set conjugator
    this->conjugator.setup("http://verbe.cc/vcfr/conjugate/fr/");
    //qDebug()<<this->conjugator.conj("être");

    // Init quiz
    on_actionNew_Quiz_N_triggered();

    // Set focus
    ui->le_input->clear();
    ui->le_input->setFocus();
    //ui->le_search->setEnabled(false);

    // Set fixed size
    ui->lst_history->setFixedWidth(200);
    this->progress->setFixedWidth(300);

    // Set tab3
    update_tab3();

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::single_quiz()
{
    //qDebug () << this->current_study << this->words_count;
    if (this->current_study >= this->words_count) {
        ui->le_input->setEnabled(false);
        //ui->le_input->setText("No new quiz.");
        ui->lbl_word->setText("No new quiz.");
        return;
    }

    // Check quiz finished
    //qDebug() << passed_cycles << quiz_cycles;
    if (passed_cycles == quiz_cycles){
        //if( quiz_cycles == right_cycles){
        if ( 100*right_cycles / (double) quiz_cycles >= this->quiz_rate){
            current_study++;
            this->lbl_network->setText(tr("You have passed quiz %1").arg(this->current_study));
        }else{
            this->lbl_network->clear();
        }
        //qDebug() << this->quiz_cycles << right_cycles;

        // Save study history & update study_history
        QFile fileStudy("study.json");
        fileStudy.open(QFile::WriteOnly);
        QVariantMap vmap = variant_read_from_map(right_study, wrong_study);
        QJsonObject obj = QJsonObject::fromVariantMap(vmap);
        //if(!vmap.isEmpty()) qDebug() << vmap.keys() << vmap.values()[0].toDouble();
        QJsonDocument doc(obj);
        fileStudy.write(doc.toJson());

        for(QMap<QString,int>::iterator it = right_study.begin(); it != right_study.end(); it++){
            // Update study_history
            if (right_study[it.key()]+wrong_study[it.key()])
                study_history[it.key()] = (double)right_study[it.key()]/((double)right_study[it.key()]+(double)wrong_study[it.key()]);
            // Clear right and wrong table
            right_study[it.key()] = 0;
            wrong_study[it.key()] = 0;
        }
        // Finished study
        if ( this->current_study >= this->words_count){
            ui->lbl_word->setText(tr("<img src=\"win.jpg\" height=\"200\" /><br><font color=\"green\">Cong! You have passed all quizzes.</font>"));
            ui->le_input->setEnabled(false);
            return;
        }

        this->on_actionNew_Quiz_N_triggered();
    }

    //lbl_status->setText(tr("rand %1").arg(QRandomGenerator::global()->bounded(words_count)));

    this->quiz_word = take_a_word();
    ui->lbl_word->setText(tr("<b>%1</b>").arg(this->quiz_word));
    this->progress->setValue(passed_cycles);

    int random_proun = QRandomGenerator::global()->bounded(this->prouns.count());
    QMap<QString,bool>::iterator it;
    QList<QString> available_tense;
    for (it = this->tenses_switch.begin(); it != this->tenses_switch.end(); it++) {
        if (it.value())
            available_tense << it.key();
    }

    QString tense;
    if(available_tense.isEmpty()){
        tense = "présent";
    }
    else{
        int random_tense = QRandomGenerator::global()->bounded(available_tense.count());
        tense = available_tense[random_tense].split(":").last();
    }



    // We first fetch answer storaged in local, then in the internet
    QFile fileDict("dict.json");
    fileDict.open(QFile::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(fileDict.readAll());
    QVariantMap map = doc.toVariant().toMap();
    QString data = map.value(this->quiz_word).toString();

//    qDebug() << data;
    if(!data.isEmpty()){
        doc = QJsonDocument::fromJson(data.toUtf8());
    }else {
        this->lbl_network->setText("Fetching answer....\t");
        ui->le_input->setEnabled(false);
        QString query = this->conjugator.conj(this->quiz_word);
        this->lbl_network->setText("Network ready.");
        ui->le_input->setEnabled(true);
        ui->le_input->setFocus();

        doc = QJsonDocument::fromJson(query.toUtf8());
    }
    QString full_proun = doc["value"]["moods"]["indicatif"][tense][random_proun].toString();
    this->quiz_answer = full_proun.split(QRegExp("[ \']")).last();

    //qDebug() << this->quiz_answer;
    //    qDebug() << tense;

    //QString pr = prouns[random_proun];
    //QString pro = take_proun(pr,quiz_word,tense);
    this->pro = full_proun.left(full_proun.length()-this->quiz_answer.length());
    ui->lbl_tense->setText(tr("<font color=\"#0000ff\">%1</font>/%2/%3\t(%4): %5")
                           .arg(this->right_cycles)
                           .arg(this->passed_cycles).arg(this->quiz_cycles)
                           .arg(tense).arg(pro));

    //qDebug() << speech->availableLocales();
    //this->speech->setVoice(QVoice::)
    //this->speech->say(this->quiz_word);
    on_btn_play_word_clicked();
}

void MainWindow::update_tab3()
{
    QMap<QString,double>::iterator it;
    int right = 0, wrong = 0, remains;
    for(it = study_history.begin(); it != study_history.end(); it++){
        //qDebug() << it.value();
        if (it.value() > .99) right++;
        else wrong++;
    }
    remains = this->words_count - right - wrong;

    QPieSeries *series = new QPieSeries();
    series->append(tr("Remerbered %1").arg(right), right);
    series->append(tr("Studying %1").arg(wrong), wrong);
    series->append(tr("Remaining %1").arg(remains), remains);
    series->setLabelsVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("<h3>Study Progress (Last Time)</h3>"));
    //chart->legend()->hide();

    QChartView *chartView_words = new QChartView(chart);
    chartView_words->setRenderHint(QPainter::Antialiasing);

//    QLineSeries *series_history = new QLineSeries();
//    series_history->append(0,1);
//    series_history->append(1,2);
//    QChart *chart_history = new QChart();
//    chart_history->addSeries(series_history);
//    chart_history->setTitle("<h3>Calender</h3>");
//    QChartView *chartView_history = new QChartView(chart_history);

    ui->hb_sta->addWidget(chartView_words);
//    ui->hb_sta->addWidget(chartView_history);
}

QMap<QString, double> MainWindow::map_read_from_variant(QVariantMap vmap)
{
    QMap<QString, double> map;
    QVariantMap::iterator it;
    for (it = vmap.begin(); it!=vmap.end(); it++) {
        map[it.key()] = it.value().toDouble();
        //qDebug()<<it.key() <<it.value().toDouble();
    }
    return map;
}

QVariantMap MainWindow::variant_read_from_map(QMap<QString, int> map1, QMap<QString, int> map2)
{
    QVariantMap vmap;
    QMap<QString,int>::iterator it;
    for(it = map1.begin(); it != map1.end(); it++){
        int right = it.value();
        int wrong = map2[it.key()];
        vmap[it.key()] = QVariant(right/((double)(wrong+right)));
//        qDebug() << it.key() << right << wrong;
    }
    QMap<QString,double>::iterator i;
    for(i=this->study_history.begin();i!=this->study_history.end();i++){
        if (!vmap.contains(i.key()))
            vmap[i.key()] = QVariant(i.value());
    }
    return vmap;
}

QString MainWindow::take_a_word()
{
    int idx;
    int ch = QRandomGenerator::global()->bounded(100);
    if(!current_study)current_study=1;
    if(ch>this->quiz_new_rate){// Old word
        //qDebug() << "Old word";
        idx = QRandomGenerator::global()->bounded(current_study);
    }else{
        // New word
        //qDebug() << "New word at " << current_study;
        idx = current_study;
    }
    return this->words_list[idx].toString();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Set mainwindow position and size
    settings->setValue("pos", QVariant(this->pos()).toPoint());
    settings->setValue("size", QVariant(this->size()).toSize());
    settings->setValue("quiz cycles", QVariant(this->quiz_cycles).toInt());
    settings->setValue("current study", QVariant(this->current_study).toInt());

    // Save study history
//    QFile fileStudy("study.json");
//    fileStudy.open(QFile::WriteOnly);
//    QVariantMap vmap = variant_read_from_map(right_study, wrong_study);
//    QJsonObject obj = QJsonObject::fromVariantMap(vmap);
//    if(!vmap.isEmpty()) qDebug() << vmap.keys() << vmap.values()[0].toDouble();
//    QJsonDocument doc(obj);
//    fileStudy.write(doc.toJson());
}


void MainWindow::on_actionSettings_S_triggered()
{
    this->setting_dialog = new QDialog;
    setting_dialog->setModal(true);
    setting_dialog->setWindowTitle(tr("Settings"));
    setting_dialog->resize(400,300);
    setting_dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Add vertical layouts
    QHBoxLayout *hb_ind = new QHBoxLayout();
    QHBoxLayout *hb_cond = new QHBoxLayout();
    QHBoxLayout *hb_sub = new QHBoxLayout();
    QHBoxLayout *hb_ok = new QHBoxLayout();
    QVBoxLayout *vb_total = new QVBoxLayout();

    // Add check boxes
    // indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait', 'futur-simple', 'futur-antérieur'
    // conditionnel_teneses = 'présent', 'passé',
    // subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
    chk_ind_present = new QCheckBox();
    chk_ind_present->setText("présent");
    chk_ind_present->setChecked(tenses_switch["indicatif:présent"]);
    chk_ind_passe_compose = new QCheckBox();
    chk_ind_passe_compose->setText("passé-composé");
    chk_ind_passe_compose->setChecked(tenses_switch["indicatif:passé-composé"]);
    chk_ind_imparfait = new QCheckBox();
    chk_ind_imparfait->setText("imparfait");
    chk_ind_imparfait->setChecked(tenses_switch["indicatif:imparfait"]);
    chk_ind_plus_que_parfait = new QCheckBox();
    chk_ind_plus_que_parfait->setText("plus-que-parfait");
    chk_ind_plus_que_parfait->setChecked(tenses_switch["indicatif:plus-que-parfait"]);


    chk_ind_futur_simple = new QCheckBox();
    chk_ind_futur_simple->setText("futur-simple");
    chk_ind_futur_simple->setChecked(tenses_switch["indicatif:futur-simple"]);
    chk_ind_futur_anterieur = new QCheckBox();
    chk_ind_futur_anterieur->setText("futur-antérieur");
    chk_ind_futur_anterieur->setChecked(tenses_switch["indicatif:futur-antérieur"]);

    chk_cond_present = new QCheckBox();
    chk_cond_present->setText("présent");
    chk_cond_present->setChecked(tenses_switch["conditionnel:présent"]);
    chk_cond_passe = new QCheckBox();
    chk_cond_passe->setText("passé");
    chk_cond_passe->setChecked(tenses_switch["conditionnel:passé"]);

    chk_sub_present = new QCheckBox();
    chk_sub_present->setText("présent");
    chk_sub_present->setChecked(tenses_switch["subjonctif:présent"]);
    chk_sub_passe = new QCheckBox();
    chk_sub_passe->setText("passé");
    chk_sub_passe->setChecked(tenses_switch["subjonctif:passé"]);
    chk_sub_imparfait = new QCheckBox();
    chk_sub_imparfait->setText("imparfait");
    chk_sub_imparfait->setChecked(tenses_switch["subjonctif:imparfait"]);
    chk_sub_plus_que_parfait = new QCheckBox();
    chk_sub_plus_que_parfait->setText("plus-que-parfait");
    chk_sub_plus_que_parfait->setChecked(tenses_switch["subjonctif:plus-que-parfait"]);

    // Add checkboxes to layouts
    // Add labels to layouts
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


    // Add data path settings
    QHBoxLayout *hb_data_path = new QHBoxLayout();
    QLabel *lbl_words_path = new QLabel("Choose words file: ");
    le_words_path = new QLineEdit();
    le_words_path->setReadOnly(true);
    le_words_path->setEnabled(false);
    le_words_path->setText(words_file_path);

    QPushButton *btn_choose = new QPushButton("Choose");
    connect(btn_choose, SIGNAL(clicked()), this, SLOT(on_settings_choose_words_file()));
    QPushButton *btn_reset = new QPushButton("Reset Studying Progress");
    connect(btn_reset, SIGNAL(clicked()), this, SLOT(on_settings_reset()));

    hb_data_path->addWidget(lbl_words_path);
    hb_data_path->addWidget(le_words_path);
    hb_data_path->addWidget(btn_choose);
    hb_data_path->addWidget(btn_reset);
    hb_data_path->addStretch();



    // Add quiz settings
    QLabel *lbl_quiz = new QLabel("Max quiz cycles: ");
    spin_quiz_number = new QSpinBox();
    spin_quiz_number->setMinimum(10);
    spin_quiz_number->setMaximum(1000);
    spin_quiz_number->setValue(this->quiz_cycles);
    QLabel *lbl_quiz_pass_rate = new QLabel("Pass rate: ");
    spin_quiz_rate = new QSpinBox();
    spin_quiz_rate->setMinimum(60);
    spin_quiz_rate->setMaximum(100);
    spin_quiz_rate->setValue(this->quiz_rate);
    QLabel *lbl_quiz_new_rate = new QLabel("New word rate: ");
    spin_quiz_new = new QSpinBox();
    spin_quiz_new->setMinimum(10);
    spin_quiz_new->setMaximum(90);
    spin_quiz_new->setValue(this->quiz_new_rate);

    QHBoxLayout *hb_quiz = new QHBoxLayout();
    hb_quiz->addWidget(lbl_quiz);
    hb_quiz->addWidget(spin_quiz_number);
    hb_quiz->addWidget(lbl_quiz_pass_rate);
    hb_quiz->addWidget(spin_quiz_rate);
    hb_quiz->addWidget(lbl_quiz_new_rate);
    hb_quiz->addWidget(spin_quiz_new);
    hb_quiz->addStretch();

    // Add buttons
    QPushButton *btn_save = new QPushButton();
    btn_save->setText("Save");
    connect(btn_save,SIGNAL(clicked()), this, SLOT(on_settings_save()));
    btn_save->setDefault(true);
    QPushButton *btn_cancel = new QPushButton();
    btn_cancel->setText("Cancel");
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(on_settings_cancel()));
    // Add spacer
    hb_ok->addStretch();
    hb_ok->addWidget(btn_save);
    hb_ok->addWidget(btn_cancel);

    // Add all hboxes to vbox
    vb_total->addLayout(hb_ind);
    vb_total->addLayout(hb_cond);
    vb_total->addLayout(hb_sub);
    vb_total->addLayout(hb_data_path);
    vb_total->addLayout(hb_quiz);
    vb_total->addLayout(hb_ok);

    vb_total->addStretch();

    setting_dialog->setLayout(vb_total);

    setting_dialog->show();
    btn_save->setFocus();
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
    if (result==QMessageBox::Yes){
        // Save settings

        this->setting_dialog->close();
    }
}

void MainWindow::on_settings_save()
{
    // update gloabel variables
    this->current_study = 0;
    this->quiz_cycles = spin_quiz_number->value();
    this->quiz_rate = spin_quiz_rate->value();
    this->quiz_new_rate = spin_quiz_new->value();

    // indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait', 'futur-simple', 'futur-antérieur'
    // conditionnel_teneses = 'présent', 'passé',
    // subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
    tenses_switch["indicatif:présent"] = chk_ind_present->isChecked();
    tenses_switch["indicatif:passé-composé"] = chk_ind_passe_compose->isChecked();
    tenses_switch["indicatif:imparfait"] = chk_ind_imparfait->isChecked();
    tenses_switch["indicatif:plus-que-parfait"] = chk_ind_plus_que_parfait->isChecked();
    tenses_switch["indicatif:futur-simple"] = chk_ind_futur_simple->isChecked();
    tenses_switch["indicatif:futur-antérieur"] = chk_ind_futur_anterieur->isChecked();

    tenses_switch["conditionnel:présent"] = chk_cond_present->isChecked();
    tenses_switch["conditionnel:passé"] = chk_cond_passe->isChecked();

    tenses_switch["subjonctif:présent"] = chk_sub_present->isChecked();
    tenses_switch["subjonctif:passé"] = chk_sub_passe->isChecked();
    tenses_switch["subjonctif:imparfait"] = chk_sub_imparfait->isChecked();
    tenses_switch["subjonctif:plus-que-parfait"] = chk_sub_plus_que_parfait->isChecked();

    // Save settings
    settings->setValue("words file path", QVariant(words_file_path).toString());
    settings->setValue("quiz cycles", QVariant(quiz_cycles).toInt());
    settings->setValue("quiz rate", QVariant(quiz_rate).toInt());
    settings->setValue("quiz new rate", QVariant(quiz_new_rate).toInt());
    QMap<QString,bool>::iterator i;
    for(i=tenses_switch.begin();i!=tenses_switch.end();i++){
        settings->setValue(i.key(),i.value());
    }

    setting_dialog->close();


    // Show status
    this->lbl_status->setText("Save settings successfully!\t");
}

void MainWindow::on_settings_choose_words_file()
{
    words_file_path = QFileDialog::getOpenFileName(setting_dialog, "Choose words file",
                                                     ".");
    if(!words_file_path.isEmpty() && le_words_path)
        le_words_path->setText(words_file_path);
    else if(words_file_path.isEmpty()){
        //QMessageBox::warning(setting_dialog, "Warning", tr("Invalid file path: %1!").arg(words_file_path), QMessageBox::Ok);
        lbl_status->setText(tr("<font color=\"red\">Invalid file path</font>\t").arg(words_file_path));
    }else{
        QMessageBox::warning(setting_dialog, "Warning", tr("Unkown error!"), QMessageBox::Ok);
        //qDebug()<<le_words_path<<"\n";
    }
}

void MainWindow::on_settings_reset()
{
    //qDebug() << this->current_study;
    this->current_study = 0;
}

void MainWindow::on_actionAbout_A_triggered()
{
    QMessageBox::information(
                this, "About us",
                "<b>French Conjugator v1.0</b> aims to help people who suffer from the disgusting verb conjugations in French laguage in a related easy way. "
                "Allrights reserved © <a href=\"https://github.com/shijie-xu/conj/releases\">Shi-Jie Xu</a>. "
                "Thanks to <a href=\"http://verbe.cc\">http://verbe.cc</a> for providing verbs conjugation interface, and to <a href=\"https://en.wiktionary.org/wiki/Category:French_irregular_verbs\">Wikitionary</a> for providing irregular verbs list. "
                "See more details <a href=\"https://en.wiktionary.org/wiki/Appendix:French_irregular_verbs\">here.</a><br>"
                + tr("Built with Qt %1 on %2.").arg(QT_VERSION_STR).arg(QLocale("en_US").toDate(QString(__DATE__).simplified(), tr("MMM d yyyy")).toString("yyyy-MM-d")),
                QMessageBox::Yes, QMessageBox::Yes);
}

void MainWindow::on_le_input_returnPressed()
{
    // DEBUG
    //qDebug() << this->right_study << this->wrong_study << this->study_history;
    // Check answer
    passed_cycles++;
    QString response = ui->le_input->text();
    if (quiz_answer.isEmpty()){
        right_cycles++;
//        lbl_status->setText("Unkown word.\t");
        lbl_status->clear();
        if(this->right_study.contains((this->quiz_word)))
            this->right_study[this->quiz_word]++;
        else
            this->right_study[this->quiz_word] = 1;
    }
    else if (quiz_answer == response){
        right_cycles++;
        lbl_status->setText("<font color=\"green\">Right!\t</font>");
        if (this->right_study.contains(this->quiz_word))
            this->right_study[this->quiz_word]++;
        else
            this->right_study[this->quiz_word] = 1;
    }else{
        lbl_status->setText(tr("Wrong: <font color=\"red\">%1.</font>\t").arg(quiz_answer));
        if(this->wrong_study.contains(this->quiz_word))
            this->wrong_study[this->quiz_word]++;
        else
            this->wrong_study[this->quiz_word] = 1;
    }
    // Update ui
    this->progress->setRange(0, this->quiz_cycles);
    this->progress->setValue(passed_cycles);
    ui->le_input->clear();
    ui->lst_history->insertItem(0, this->quiz_word + tr("\t%1%").arg(this->study_history[this->quiz_word]*100.0));
//    ui->lst_history->setCurrentRow(ui->lst_history->count());
    // New quiz cycle
    single_quiz();
}


void MainWindow::on_lst_history_itemClicked(QListWidgetItem *item)
{
//    qDebug() << "clicked item " << item->text();
//    item->setToolTip(item->text());
    QString search_url = "http://en.wiktionary.org/wiki/"+item->text().split("\t").first();
    //ui->tabWidget->setCurrentIndex(1);
    //ui->le_search->setText(search_url);
    //ui->le_search->setEnabled(false);

    QMessageBox::information(this, "Wikitionary",
                             tr("The author just HATE MSVC kit and no QtWebView module can be compiled with on Windows.<br>Please click <a href=\"%1\">%2</a>").arg(search_url).arg(search_url));
   // this->lbl_network->setText(tr("<a href=\"%1\">%2</a>").arg(search_url).arg(search_url));
   // ui->verticalLayout_4->addWidget()
//    Conjugate *wiki = new Conjugate();
//    wiki->setup("http://en.wiktionary.org/wiki/");

//    QString verb = item->text().split("\t").first();
//    QString content = wiki->conj(verb);
//    if (content.isEmpty())
//        ui->tb_wiki->setHtml(tr("<html><head></head><body><h1>Connection faild: %1</h1></body></html>").arg(content));
//    else
//        ui->tb_wiki->setHtml(wiki->conj(verb));
}

void MainWindow::on_actionNew_Quiz_N_triggered()
{
    // New quiz
    this->passed_cycles = 0;
    this->right_cycles = 0;
//    this->right_study.empty();
//    this->wrong_study.empty();

    this->lbl_status->setText("New quiz begins.\t");
    this->progress->setValue(passed_cycles);

    ui->lbl_word->clear();
    ui->lbl_tense->clear();
    ui->le_input->clear();

    single_quiz();
}

void MainWindow::on_actionFetch_Dictionary_F_triggered()
{
    // Set ui
    ui->le_input->setEnabled(false);

    QJsonObject obj;
    this->progress->setRange(0, words_count);
    for (int i = 0; i < words_list.count(); ++i) {
        QString word = words_list.at(i).toString();
        QString data = conjugator.conj(word);
        obj.insert(word, data);

        this->progress->setValue(i+1);
        this->lbl_status->setText(tr("Fetching <b>%1....\t").arg(word));
    }
    this->progress->setValue(0);
    this->lbl_status->setText("Data Fetched.\t");

    QJsonDocument doc(obj);
    QFile dictFile("dict.json");
    dictFile.open(QFile::WriteOnly);
    dictFile.write(doc.toJson());

    // Recover ui
    ui->le_input->setEnabled(true);
}

void MainWindow::on_btn_play_word_clicked()
{
    //qDebug() << this->quiz_word;
    if(ui->chk_word->isChecked())
        speech->say(this->quiz_word);
    else
        speech->say(this->pro+" "+this->quiz_answer);
}

void MainWindow::on_hide_window()
{
    if (this->bshow){
        this->setWindowState(Qt::WindowMinimized);
    }
}
