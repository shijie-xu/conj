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
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtTextToSpeech>
#include <QLocale>
#include <QShortcut>
#include <QKeySequence>
#include <QThread>
#include <QTextFormat>
#include <QColor>
#include <QDataStream>
#include <QByteArray>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtMultimedia/QSound>
#include <QDate>
#include <QDateTime>
#include <QWord.h>


QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget* parent)
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
	srand((unsigned int)time(nullptr));

	// Read settings and set window
	settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	this->move(settings->value("pos").toPoint());
	this->resize(settings->value("size").toSize());
	this->words_file_path = settings->value("words file path").toString();
	this->quiz_cycles = settings->value("quiz cycles").toInt();
	this->quiz_rate = settings->value("quiz rate").toInt();
	this->quiz_new_rate = settings->value("quiz new rate").toInt();
	this->current_study = settings->value("current study").toInt();
	ui->tabWidget->setCurrentIndex(settings->value("last tab").toInt());
	ui->spin_max->setValue(settings->value("sentence max len").toInt());
	ui->spin_min->setValue(settings->value("sentence min len").toInt());
	ui->spin_freq->setValue(settings->value("sentence freq").toInt());
	this->sent_complete = settings->value("sentence complete").toInt();
	this->right_sent_complete = settings->value("right sentence complete").toInt();

	this->bshow = true;

	// Set status bar
	this->progress = new QProgressBar(this);
	progress->setRange(0, quiz_cycles);
	ui->statusbar->addWidget(progress);
	this->lbl_network = new QLabel(this);
	ui->statusbar->addWidget(lbl_network);
	this->lbl_status = new QLabel("French Conjugator v0.1\t", this);
	ui->statusbar->addWidget(lbl_status);

	// Set tts
	this->speech = new QTextToSpeech(this);
	this->speech->setLocale(QLocale::French);

	// Set shortcuts
	// F5 for play word
	QShortcut* sc_play = new QShortcut(QKeySequence("F5"), this);
	connect(sc_play, SIGNAL(activated()), this, SLOT(on_btn_play_word_clicked()));
	QShortcut* sc_hide = new QShortcut(QKeySequence("F9"), this);
	connect(sc_hide, SIGNAL(activated()), this, SLOT(on_hide_window()));
	QShortcut* sc_clear = new QShortcut(QKeySequence("Q"), this);
	connect(sc_clear, SIGNAL(activated()), this, SLOT(on_clear_complete_operation()));

	// To use following shortcuts, you are supposed to block the syncling state
	// and the codes are invalid now
	//QShortcut* sc_check = new QShortcut(QKeySequence("W"), this);
	//connect(sc_check, SIGNAL(activated()), this, SLOT(on_btn_ok_clicked()));

	// Load words file
	QFile src(words_file_path);
	if (!src.open(QFile::ReadWrite)) {
		QMessageBox::warning(this, "Conj", "Cannot load words file!");
	}
	this->words_list = QJsonDocument::fromJson(
		QString(src.readAll()).toUtf8()).array();
	words_count = words_list.count();

	// NOTICE: Unccessary komma is NOT permitted!
	// You must write word json file like this:
	// ["word1", "word2", ..., "wordn"]

	//qDebug() << words_file_path << words_list << words_count;

	// Set words study
	QFile fileStudy("study.json");
	if (fileStudy.open(QFile::ReadOnly)) {
		QJsonDocument doc = QJsonDocument::fromJson(fileStudy.readAll());
		study_history = map_from_variant(doc.toVariant().toMap());
	}
	else {
		QMessageBox::warning(this, "Conj", "Study history not existed.");
	}

	// Set proun
	this->prouns << "je" << "tu" << "il"
		<< "nous" << "vous" << "ils";

	// Init tenses
	// indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait',
	// 'passé-simple', 'passé-antérieur', 'futur-simple', 'futur-antérieur'
	// conditionnel_teneses = 'présent', 'passé',
	// subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
	// imperatif_tenses = 'présent', 'passé',
	// participe_tenses = 'présent', 'passé',
	QList<QString> tenses = {
		"indicatif:présent", "indicatif:passé-composé",
		"indicatif:imparfait", "indicatif:plus-que-parfait",
		"indicatif:passé-simple", "indicatif:passé-antérieur",
		"indicatif:futur-simple", "indicatif:futur-antérieur",

		"conditionnel:présent", "conditionnel:passé",

		"subjonctif:présent", "subjonctif:passé",
		"subjonctif:imparfait", "subjonctif:plus-que-parfait"

		"imperatif:présent", "imperatif:passé",

		"participe:présent", "participe:passé"
	};
	for (int i = 0; i < tenses.count(); ++i) {
		QString tense = tenses.at(i);
		tenses_switch.insert(tense, settings->value(tense).toBool());
	}

	// Set conjugator
	this->conjugator.setup("http://verbe.cc/vcfr/conjugate/fr/");
	this->cur_queries_count = 0;
	this->thrd = NULL;

	// Init quiz
	on_actionNew_Quiz_N_triggered();
	single_pronom_quiz();

	// Set focus
	ui->le_input->clear();
	ui->le_input->setFocus();
	//ui->le_search->setEnabled(false);

	// Set fixed size
	ui->lst_history->setFixedWidth(200);
	this->progress->setFixedWidth(300);
	ui->lst_words->setFixedWidth(300);
	ui->lst_words->setMinimumHeight(200);
	ui->lbl_hint->setMinimumWidth(200);
	ui->btn_ok->setFixedSize(75, 80);
	ui->btn_play->setFixedSize(50, 80);
	ui->btn_clear->setFixedSize(50, 80);
	ui->lbl_origin->setWordWrap(true);
	ui->lbl_sent->setWordWrap(true);
	ui->lbl_trans->setWordWrap(true);

	// Connect to database
	this->db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("history.db");
	if (!db.open()) qDebug() << db.lastError();

	// Update learned times from db
	// read_learned_times_from_db("dit le petit prince");

	// Update tabs
	update_tab_conj_history();
	update_tab_sentence_complete_history();
	read_today_learning_sentences();

	// Set tab4
	ui->te_sentence->setReadOnly(true);

	// File must be saved in `UTF-8 with BOM` format
	// Calculate word frequency
	// Initialized Python engine
	//this->sentence_parse = QSentence();
	//sentence_parse.init_pyengine();

	double alpha = 0.8;
	ui->lbl_words->setText(tr("Complete (<font color=\"blue\">%1</font>/%2/%3%/%4)")
		.arg(this->right_sent_complete)
		.arg(this->sent_complete)
		.arg(100 * this->right_sent_complete / this->sent_complete)
		.arg((int)((alpha * this->sent_complete - this->right_sent_complete) / (1 - alpha))));
	this->total_words = 0;
	calc_words_freq("src.txt");
	QFile sourceTxt("src.txt");
	sourceTxt.open(QIODevice::ReadOnly);
	QTextStream tsSource(&sourceTxt);
	QString content = tsSource.readAll();
	QRegExp rx("\\.|\\?|\\n|\\…|\\:|\\;|\\,|\\!|\\*|\\(|\\)|\\«|\\»|\\–|[0-9]");
	this->sentence_list = content.split(rx);
	single_sentence_complete_quiz();
}

MainWindow::~MainWindow()
{
	//this->sentence_parse.exit_pyengine();
	delete ui;
}

void MainWindow::single_conj_quiz()
{
	if (this->current_study >= this->words_count) {
		ui->le_input->setEnabled(false);
		ui->lbl_word->setText("No new quiz.");
		return;
	}

	// Check quiz finished
	if (passed_cycles == quiz_cycles) {
		if (100 * right_cycles / (double)quiz_cycles >= this->quiz_rate) {
			current_study++;
			this->lbl_network->setText(tr("You have passed quiz %1").arg(this->current_study));
		}
		else {
			this->lbl_network->clear();
		}

		// Save study history & update study_history
		QFile fileStudy("study.json");
		fileStudy.open(QFile::WriteOnly);
		QVariantMap vmap = variant_from_map(right_study, wrong_study);
		QJsonObject obj = QJsonObject::fromVariantMap(vmap);
		QJsonDocument doc(obj);
		fileStudy.write(doc.toJson());

		for (QMap<QString, int>::iterator it = right_study.begin(); it != right_study.end(); it++) {
			// Update study_history
			if (right_study[it.key()] + wrong_study[it.key()])
				study_history[it.key()] = (double)right_study[it.key()] / ((double)right_study[it.key()] + (double)wrong_study[it.key()]);
			// Clear right and wrong table
			right_study[it.key()] = 0;
			wrong_study[it.key()] = 0;
		}
		// Finished study
		if (this->current_study >= this->words_count) {
			ui->lbl_word->setText(tr("<img src=\"win.jpg\" height=\"200\" /><br><font color=\"green\">Cong! You have passed all quizzes.</font>"));
			ui->le_input->setEnabled(false);
			return;
		}

		this->on_actionNew_Quiz_N_triggered();
		return;
	}
	this->quiz_word = take_a_word();
	ui->lbl_word->setText(tr("<b>%1</b>").arg(this->quiz_word));
	this->progress->setValue(passed_cycles);

	int random_proun = QRandomGenerator::global()->bounded(this->prouns.count());
	QMap<QString, bool>::iterator it;
	QList<QString> available_tense;
	for (it = this->tenses_switch.begin(); it != this->tenses_switch.end(); it++) {
		if (it.value())
			available_tense << it.key();
	}

	QString tense, mood;
	if (available_tense.isEmpty()) {
		tense = "présent";
		mood = "indicatif";
	}
	else {
		int random_tense = QRandomGenerator::global()->bounded(available_tense.count());
		tense = available_tense[random_tense].split(":").last();
		mood = available_tense[random_tense].split(":").first();
	}

	// We first fetch answer storaged in local, then in the internet
	QFile fileDict("dict.json");
	fileDict.open(QFile::ReadOnly);
	QJsonDocument doc = QJsonDocument::fromJson(fileDict.readAll());
	QVariantMap map = doc.toVariant().toMap();
	QString data = map.value(this->quiz_word).toString();

	if (!data.isEmpty()) {
		doc = QJsonDocument::fromJson(data.toUtf8());
	}
	else {
		this->lbl_network->setText("Fetching answer....\t");
		ui->le_input->setEnabled(false);
		QString query = this->conjugator.conj(this->quiz_word);
		this->lbl_network->setText("Network ready.");
		ui->le_input->setEnabled(true);
		ui->le_input->setFocus();

		doc = QJsonDocument::fromJson(query.toUtf8());
	}
	//    QString full_proun = doc["value"]["moods"]["indicatif"][tense][random_proun].toString();
	QString full_proun = doc["value"]["moods"][mood][tense][random_proun].toString();
	this->quiz_answer = full_proun.split(QRegExp("[ \']")).last();

	this->pro = full_proun.left(full_proun.length() - this->quiz_answer.length());

	QString clr;
	if (this->passed_cycles - this->right_cycles > this->quiz_cycles* (1 - this->quiz_rate / 100.0))
		clr = "#ff0000";
	else
		clr = "#0000ff";
	ui->lbl_tense->setText(tr("<font color=\"%1\">%2</font>/%3/%4\t(%5): %6")
		.arg(clr)
		.arg(this->right_cycles)
		.arg(this->passed_cycles)
		.arg(this->quiz_cycles)
		.arg(mood.left(3) + "." + tense).arg(pro));

	on_btn_play_word_clicked();
}

void MainWindow::update_tab_conj_history()
{
	QMap<QString, double>::iterator it;
	int right = 0, wrong = 0, remains;
	for (it = study_history.begin(); it != study_history.end(); it++) {
		if (it.value() > .99) right++;
		else wrong++;
	}
	remains = this->words_count - right - wrong;

	QPieSeries* series = new QPieSeries(this);
	series->append(tr("Remerbered %1").arg(right), right);
	series->append(tr("Studying %1").arg(wrong), wrong);
	series->append(tr("Remaining %1").arg(remains), remains);
	series->setLabelsVisible();

	QChart* chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(tr("<h3>Study Progress (Last Time)</h3>"));
	//chart->legend()->hide();

	QChartView* chartView_words = new QChartView(chart, this);
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

void MainWindow::update_tab_sentence_complete_history()
{
	// Connect to the database
	QSqlQuery query;

	// Query records;
	QBarSeries* series = new QBarSeries();
	query.exec("select * from sentences_complete_records");
	while (query.next()) {
		QString date = query.value(0).toString();
		QBarSet* hist_set = new QBarSet(date);
		*hist_set << query.value(1).toInt();
		series->append(hist_set);
	}

	QChart* chart = new QChart();
	chart->addSeries(series);
	//    chart->setTitle("Requests");
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "Number of Requests";
	QBarCategoryAxis* axis = new QBarCategoryAxis();
	axis->append(categories);
	chart->createDefaultAxes();
	chart->setAxisX(axis, series);

	//    chart->legend()->setVisible(true);
	//    chart->legend()->setAlignment(Qt::AlignBottom);

	QChartView* chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	ui->hb_history->addWidget(chartView);
}

QMap<QString, double> MainWindow::map_from_variant(QVariantMap vmap)
{
	QMap<QString, double> map;
	QVariantMap::iterator it;
	for (it = vmap.begin(); it != vmap.end(); it++) {
		map[it.key()] = it.value().toDouble();
	}
	return map;
}

QVariantMap MainWindow::variant_from_map(QMap<QString, int> map1, QMap<QString, int> map2)
{
	QVariantMap vmap;
	QMap<QString, int>::iterator it;
	for (it = map1.begin(); it != map1.end(); it++) {
		int right = it.value();
		int wrong = map2[it.key()];
		vmap[it.key()] = QVariant(right / ((double)(wrong + right)));
	}
	QMap<QString, double>::iterator i;
	for (i = this->study_history.begin(); i != this->study_history.end(); i++) {
		if (!vmap.contains(i.key()))
			vmap[i.key()] = QVariant(i.value());
	}
	return vmap;
}

QString MainWindow::take_a_word()
{
	int idx;
	int ch = QRandomGenerator::global()->bounded(100);
	if (ch > this->quiz_new_rate) {
		// Old word
		idx = QRandomGenerator::global()->bounded(current_study + 1);
	}
	else {
		idx = current_study;
	}
	return this->words_list[idx].toString();
}

void MainWindow::single_pronom_quiz()
{
	QString pos[] = { "subject", "direct object", "indirect object", "reflexive", "emphatic" };
	QString pronoms[] = { "je", "tu", "il", "elle", "nous", "vous", "ils", "elles" };
	QString pro[] = {
	"je", "me", "me", "me", "moi",
	"tu", "te", "te", "te", "toi",
	"il", "le", "lui", "se", "lui",
	"elle", "la", "lui", "se", "elle",
	"nous", "nous", "nous", "nous", "nous",
	"vous", "vous", "vous", "vous", "vous",
	"ils", "les", "leur", "se", "eux",
	"elles", "les", "leur", "se", "elles" };
	if (ui->chk_show_pronoms->isChecked()) {
		ui->tbl_pronom->setColumnCount(5);
		ui->tbl_pronom->setRowCount(8);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 5; j++) {
				QString w = pro[i * 5 + j];
				QTableWidgetItem* item = new QTableWidgetItem(w);
				ui->tbl_pronom->setItem(i, j, item);
			}
		}
	}
	else {
		ui->tbl_pronom->clear();
	}
	int p = QRandomGenerator::global()->bounded(5);
	int pr = QRandomGenerator::global()->bounded(8);
	this->pronom_answer = pro[pr * 5 + p];
	ui->lbl_pronom_pos->setText(tr("(%1, %2): ")
		.arg(pos[p])
		.arg(pronoms[pr]));
}

void MainWindow::calc_words_freq(QString file)
{
	QFile sourceTxt(file);
	sourceTxt.open(QIODevice::ReadOnly);
	QTextStream tsSource(&sourceTxt);
	QString content = tsSource.readAll();
	QRegExp rx("’|\\ |\\.|\\?|\\n|\\…|\\:|\\;|\\,|\\!|\\*|\\—|\\(|\\)|\\«|\\»");
	QList<QString> word_list = content.split(rx);
	for (QList<QString>::iterator it = word_list.begin(); it != word_list.end(); it++) {
		if (!it->isEmpty()) {
			QString word = it->toLower();
			if (this->words_freq.contains(word)) {
				words_freq[word]++;
			}
			else {
				words_freq[word] = 1;
			}
			this->total_words++;
		}
	}
}

qint64 MainWindow::string_hash(QString str)
{
	QByteArray hash = QCryptographicHash::hash(
		QByteArray::fromRawData((const char*)str.utf16(), str.length() * 2),
		QCryptographicHash::Md5);
	Q_ASSERT(hash.size() == 16);
	QDataStream stream(hash);
	qint64 a, b;
	stream >> a >> b;
	return a ^ b;
}

QString MainWindow::compiled_day()
{
	return QLocale("en_US").toDate(QString(__DATE__).simplified(), tr("MMM d yyyy")).toString("yyyy-MM-d");
}

QString MainWindow::this_day()
{
	QDateTime local(QDateTime::currentDateTime());
	return local.toString("yyyy-MM-dd");
}

int MainWindow::read_learned_times_from_db(QString sentence)
{
	QSqlQuery query;
	// Create table
	//query.exec("create table sentence_times(sentence text primary key, times int)");

	// We do NOT read the whole database when initialize, otherwise, read in single quiz
	query.exec(tr("select * from sentence_times where sentence=\"%1\"").arg(sentence));
	if (query.next()) {
		return query.value(1).toInt();
	}
	return 0;
}

void MainWindow::update_learned_times_into_db(QString sentence, int times)
{
	QSqlQuery query;
	// try to insert
	query.exec(tr("insert into sentence_times values(\"%1\",%2)")
		.arg(sentence)
		.arg(times));
	// update
	query.exec(tr("update sentence_times set times=%1 where sentence=\"%2\"")
		.arg(times)
		.arg(sentence));
}

QString MainWindow::read_word_description_from_db(QString word)
{
	QSqlQuery query;
	query.exec(tr("select * from dictionary where word=\"%1\"")
		.arg(word.toLower()));
	if (query.next()) return query.value(1).toString();
	return "<NaN>";
}

void MainWindow::update_words_dictionary_into_db(QString word, QString description)
{
	// try to insert
	QSqlQuery query;
	query.exec(tr("insert into dictionary values(\"%1\",\"%2\")")
		.arg(word.toLower())
		.arg(description));
}

void MainWindow::update_words_hint(QString word)
{
	int word_threshold = 50;
	// Check frequency
	if (this->words_freq.contains(word) &&
		this->words_freq[word] < word_threshold) {
		// show hint for less-use word
		// check if existed in database
		QString desp = read_word_description_from_db(word);
		if (desp == "<NaN>") {
			// fetch from internet and update to dictionary
			QString url = tr("https://translate.yandex.net/api/v1.5/tr.json/translate?key=trnsl.1.1.20191116T145524Z.a006c2b4351496e9.677dfb7949c1d615bf5aab3d2071ddb45ee750f7&text=%1&lang=fr-%2")
				.arg(word).arg("en");
			QString res = conjugator.sync_get(url);
			QString desp = "";

			QJsonDocument doc = QJsonDocument::fromJson(res.toLocal8Bit().data());
			QJsonObject obj = doc.object();
			QJsonArray arr = obj.take("text").toArray();
			// Check if response empty
			if (arr.isEmpty()) return;
			desp = arr.toVariantList().at(0).toString();
			update_words_dictionary_into_db((word).toLower(), desp);
		}
		// show hint
		ui->lbl_hint->setText(tr("<b>%1: </b>%2\n").arg(word, desp));
	}
}

void MainWindow::update_complete_percent()
{
	QSqlQuery query;
	int total_length = 60.0, sentence_1_level = 1, sentence_2_level = 1, sentence_3_level = 1;

	query.exec(tr("SELECT count(sentence) FROM sentence_times where times == 1;"));
	if (query.next()) sentence_1_level = query.value(0).toInt();
	query.exec(tr("SELECT count(sentence) FROM sentence_times where times == 2;"));
	if (query.next()) sentence_2_level = query.value(0).toInt();
	query.exec(tr("SELECT count(sentence) FROM sentence_times where times >= 3;"));
	if (query.next()) sentence_3_level = query.value(0).toInt();

	// figure out
	QString text1;
	text1.fill('>', (int)(total_length * sentence_1_level / (sentence_1_level + sentence_2_level + sentence_3_level)));
	QString text2;
	text2.fill('>', (int)(total_length * sentence_2_level / (sentence_1_level + sentence_2_level + sentence_3_level)));
	QString text3;
	text3.fill('>', (int)(total_length * sentence_3_level / (sentence_1_level + sentence_2_level + sentence_3_level)));

	ui->lbl_cs_percent->setText(
		tr("<b><font color=\"#0000ff\">%3%4</font><font color=\"#00ff00\">%2%5</font>%1%6</b>")
		.arg(text1)
		.arg(text2)
		.arg(text3)
		.arg(sentence_3_level)
		.arg(sentence_2_level)
		.arg(sentence_1_level));
}

void MainWindow::update_everyday_learning()
{
	QSqlQuery query;
	QString today = this_day();
	// try to insert
	query.exec(tr("insert into sentences_complete_records values(\"%1\",%2)")
		.arg(today)
		.arg(this->today_learning_sentences));
	// update
	query.exec(tr("update sentences_complete_records set times = %1 where date = \"%2\"")
		.arg(this->today_learning_sentences)
		.arg(today));
}

void MainWindow::read_today_learning_sentences()
{
	QSqlQuery query;
	QString today = this_day();
	query.exec(tr("select * from sentences_complete_records where date = \"%1\"").arg(today));
	if (query.next()) this->today_learning_sentences = query.value(1).toInt();
	else this->today_learning_sentences = 0;
	//qDebug() << query.value(0).toInt();
}

void MainWindow::closeEvent(QCloseEvent*)
{
	// Set mainwindow position and size
	settings->setValue("pos", QVariant(this->pos()).toPoint());
	settings->setValue("size", QVariant(this->size()).toSize());
	settings->setValue("quiz cycles", QVariant(this->quiz_cycles).toInt());
	settings->setValue("current study", QVariant(this->current_study).toInt());
	settings->setValue("last tab", QVariant(ui->tabWidget->currentIndex()));
	settings->setValue("sentence max len", QVariant(ui->spin_max->value()));
	settings->setValue("sentence min len", QVariant(ui->spin_min->value()));
	settings->setValue("sentence freq", QVariant(ui->spin_freq->value()));
	settings->setValue("sentence complete", QVariant(this->sent_complete).toInt());
	settings->setValue("right sentence complete", QVariant(this->right_sent_complete).toInt());

	// close db when quit
	db.close();
}

void MainWindow::on_actionSettings_S_triggered()
{
	this->setting_dialog = new QDialog(this);
	setting_dialog->setModal(true);
	setting_dialog->setWindowTitle(tr("Settings"));
	setting_dialog->resize(400, 300);
	setting_dialog->setAttribute(Qt::WA_DeleteOnClose);

	// Add vertical layouts
	QHBoxLayout* hb_ind = new QHBoxLayout(this);
	QHBoxLayout* hb_cond = new QHBoxLayout(this);
	QHBoxLayout* hb_sub = new QHBoxLayout(this);
	QHBoxLayout* hb_imp = new QHBoxLayout(this);
	QHBoxLayout* hb_par = new QHBoxLayout(this);
	QHBoxLayout* hb_ok = new QHBoxLayout(this);
	QVBoxLayout* vb_total = new QVBoxLayout(this);

	// Add check boxes
	// indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait',
	// 'passé-simple', 'passé-antérieur', 'futur-simple', 'futur-antérieur'
	// conditionnel_teneses = 'présent', 'passé',
	// subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
	// imperatif_tenses = 'présent', 'passé',
	// participe_tenses = 'présent', 'passé',
	chk_ind_present = new QCheckBox(this);
	chk_ind_present->setText("présent");
	chk_ind_present->setChecked(tenses_switch["indicatif:présent"]);
	chk_ind_passe_compose = new QCheckBox(this);
	chk_ind_passe_compose->setText("passé-composé");
	chk_ind_passe_compose->setChecked(tenses_switch["indicatif:passé-composé"]);
	chk_ind_imparfait = new QCheckBox(this);
	chk_ind_imparfait->setText("imparfait");
	chk_ind_imparfait->setChecked(tenses_switch["indicatif:imparfait"]);
	chk_ind_plus_que_parfait = new QCheckBox(this);
	chk_ind_plus_que_parfait->setText("plus-que-parfait");
	chk_ind_plus_que_parfait->setChecked(tenses_switch["indicatif:plus-que-parfait"]);

	chk_ind_passe_simple = new QCheckBox(this);
	chk_ind_passe_simple->setText("passé-simple");
	chk_ind_passe_simple->setChecked(tenses_switch["indicatif:passé-simple"]);
	chk_ind_passe_anterieur = new QCheckBox(this);
	chk_ind_passe_anterieur->setText("passé-antérieur");
	chk_ind_passe_anterieur->setChecked(tenses_switch["indicatif:passé-antérieur"]);
	chk_ind_futur_simple = new QCheckBox(this);
	chk_ind_futur_simple->setText("futur-simple");
	chk_ind_futur_simple->setChecked(tenses_switch["indicatif:futur-simple"]);
	chk_ind_futur_anterieur = new QCheckBox(this);
	chk_ind_futur_anterieur->setText("futur-antérieur");
	chk_ind_futur_anterieur->setChecked(tenses_switch["indicatif:futur-antérieur"]);

	chk_cond_present = new QCheckBox(this);
	chk_cond_present->setText("présent");
	chk_cond_present->setChecked(tenses_switch["conditionnel:présent"]);
	chk_cond_passe = new QCheckBox(this);
	chk_cond_passe->setText("passé");
	chk_cond_passe->setChecked(tenses_switch["conditionnel:passé"]);

	chk_sub_present = new QCheckBox(this);
	chk_sub_present->setText("présent");
	chk_sub_present->setChecked(tenses_switch["subjonctif:présent"]);
	chk_sub_passe = new QCheckBox(this);
	chk_sub_passe->setText("passé");
	chk_sub_passe->setChecked(tenses_switch["subjonctif:passé"]);
	chk_sub_imparfait = new QCheckBox(this);
	chk_sub_imparfait->setText("imparfait");
	chk_sub_imparfait->setChecked(tenses_switch["subjonctif:imparfait"]);
	chk_sub_plus_que_parfait = new QCheckBox(this);
	chk_sub_plus_que_parfait->setText("plus-que-parfait");
	chk_sub_plus_que_parfait->setChecked(tenses_switch["subjonctif:plus-que-parfait"]);

	chk_imp_present = new QCheckBox(this);
	chk_imp_present->setText("présent");
	chk_imp_present->setChecked(tenses_switch["imperatif:présent"]);
	chk_imp_passe = new QCheckBox(this);
	chk_imp_passe->setText("passé");
	chk_imp_passe->setChecked(tenses_switch["imperatif:passé"]);

	chk_par_present = new QCheckBox(this);
	chk_par_present->setText("présent");
	chk_par_present->setChecked(tenses_switch["participe:présent"]);
	chk_par_passe = new QCheckBox(this);
	chk_par_passe->setText("passé");
	chk_par_passe->setChecked(tenses_switch["participe:passé"]);

	// Add checkboxes to layouts
	// Add labels to layouts
	QLabel* lbl_ind = new QLabel("<b>indicatif</b>", this);
	QLabel* lbl_cond = new QLabel("<b>conditionnel</b>", this);
	QLabel* lbl_sub = new QLabel("<b>subjonctif</b>", this);
	QLabel* lbl_imp = new QLabel("<b>imperatif</b>", this);
	QLabel* lbl_par = new QLabel("<b>participe</b>", this);
	hb_ind->addWidget(lbl_ind);
	hb_cond->addWidget(lbl_cond);
	hb_sub->addWidget(lbl_sub);
	hb_imp->addWidget(lbl_imp);
	hb_par->addWidget(lbl_par);

	hb_ind->addWidget(chk_ind_present);
	hb_ind->addWidget(chk_ind_passe_compose);
	hb_ind->addWidget(chk_ind_imparfait);
	hb_ind->addWidget(chk_ind_plus_que_parfait);
	hb_ind->addWidget(chk_ind_passe_simple);
	hb_ind->addWidget(chk_ind_passe_anterieur);
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

	hb_imp->addWidget(chk_imp_present);
	hb_imp->addWidget(chk_imp_passe);
	hb_imp->addStretch();

	hb_par->addWidget(chk_par_present);
	hb_par->addWidget(chk_par_passe);
	hb_par->addStretch();

	// Add data path settings
	QHBoxLayout* hb_data_path = new QHBoxLayout(this);
	QLabel* lbl_words_path = new QLabel("Choose words file: ", this);
	le_irregular_words_path = new QLineEdit(this);
	le_irregular_words_path->setReadOnly(true);
	le_irregular_words_path->setEnabled(false);
	le_irregular_words_path->setText(words_file_path);

	QPushButton* btn_choose = new QPushButton("Choose", this);
	connect(btn_choose, SIGNAL(clicked()), this, SLOT(on_settings_choose_words_file()));
	QPushButton* btn_reset = new QPushButton("Reset Studying Progress", this);
	connect(btn_reset, SIGNAL(clicked()), this, SLOT(on_settings_reset()));

	hb_data_path->addWidget(lbl_words_path);
	hb_data_path->addWidget(le_irregular_words_path);
	hb_data_path->addWidget(btn_choose);
	hb_data_path->addWidget(btn_reset);
	hb_data_path->addStretch();



	// Add quiz settings
	QLabel* lbl_quiz = new QLabel("Max quiz cycles: ", this);
	spin_quiz_number = new QSpinBox(this);
	spin_quiz_number->setMinimum(10);
	spin_quiz_number->setMaximum(1000);
	spin_quiz_number->setValue(this->quiz_cycles);
	QLabel* lbl_quiz_pass_rate = new QLabel("Pass rate: ", this);
	spin_quiz_rate = new QSpinBox(this);
	spin_quiz_rate->setMinimum(60);
	spin_quiz_rate->setMaximum(100);
	spin_quiz_rate->setValue(this->quiz_rate);
	QLabel* lbl_quiz_new_rate = new QLabel("New word rate: ", this);
	spin_quiz_new = new QSpinBox(this);
	spin_quiz_new->setMinimum(10);
	spin_quiz_new->setMaximum(90);
	spin_quiz_new->setValue(this->quiz_new_rate);

	QHBoxLayout* hb_quiz = new QHBoxLayout(this);
	hb_quiz->addWidget(lbl_quiz);
	hb_quiz->addWidget(spin_quiz_number);
	hb_quiz->addWidget(lbl_quiz_pass_rate);
	hb_quiz->addWidget(spin_quiz_rate);
	hb_quiz->addWidget(lbl_quiz_new_rate);
	hb_quiz->addWidget(spin_quiz_new);
	hb_quiz->addStretch();

	// Add buttons
	QPushButton* btn_save = new QPushButton(this);
	btn_save->setText("Save");
	connect(btn_save, SIGNAL(clicked()), this, SLOT(on_settings_save()));
	btn_save->setDefault(true);
	QPushButton* btn_cancel = new QPushButton(this);
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
	vb_total->addLayout(hb_imp);
	vb_total->addLayout(hb_par);
	vb_total->addLayout(hb_data_path);
	vb_total->addLayout(hb_quiz);
	vb_total->addLayout(hb_ok);

	vb_total->addStretch();

	setting_dialog->setLayout(vb_total);

	setting_dialog->show();
	btn_save->setFocus();

	this->lbl_status->setText("<font color=\"#ff0000\">passé-simple and passé-antérieur are uncommon.</font>");
}

void MainWindow::on_actionExit_X_triggered()
{
	this->close();
}

void MainWindow::on_settings_cancel()
{
	QMessageBox::StandardButton result = QMessageBox::information(
		setting_dialog, "Settings", "Are your sure to cancel?",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (result == QMessageBox::Yes) {
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

	// indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait',
	// 'passé-simple', 'passé-antérieur', 'futur-simple', 'futur-antérieur'
	// conditionnel_teneses = 'présent', 'passé',
	// subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
	// imperatif_tenses = 'présent', 'passé',
	// participe_tenses = 'présent', 'passé',
	tenses_switch["indicatif:présent"] = chk_ind_present->isChecked();
	tenses_switch["indicatif:passé-composé"] = chk_ind_passe_compose->isChecked();
	tenses_switch["indicatif:imparfait"] = chk_ind_imparfait->isChecked();
	tenses_switch["indicatif:plus-que-parfait"] = chk_ind_plus_que_parfait->isChecked();
	tenses_switch["indicatif:passé-simple"] = chk_ind_passe_simple->isChecked();
	tenses_switch["indicatif:passé-antérieur"] = chk_ind_passe_anterieur->isChecked();
	tenses_switch["indicatif:futur-simple"] = chk_ind_futur_simple->isChecked();
	tenses_switch["indicatif:futur-antérieur"] = chk_ind_futur_anterieur->isChecked();

	tenses_switch["conditionnel:présent"] = chk_cond_present->isChecked();
	tenses_switch["conditionnel:passé"] = chk_cond_passe->isChecked();

	tenses_switch["subjonctif:présent"] = chk_sub_present->isChecked();
	tenses_switch["subjonctif:passé"] = chk_sub_passe->isChecked();
	tenses_switch["subjonctif:imparfait"] = chk_sub_imparfait->isChecked();
	tenses_switch["subjonctif:plus-que-parfait"] = chk_sub_plus_que_parfait->isChecked();

	tenses_switch["imperatif:présent"] = chk_imp_present->isChecked();
	tenses_switch["imperatif:passé"] = chk_imp_passe->isChecked();

	tenses_switch["participe:présent"] = chk_par_present->isChecked();
	tenses_switch["participe:passé"] = chk_par_passe->isChecked();
	// Save settings
	settings->setValue("words file path", QVariant(words_file_path).toString());
	settings->setValue("quiz cycles", QVariant(quiz_cycles).toInt());
	settings->setValue("quiz rate", QVariant(quiz_rate).toInt());
	settings->setValue("quiz new rate", QVariant(quiz_new_rate).toInt());
	QMap<QString, bool>::iterator i;
	for (i = tenses_switch.begin(); i != tenses_switch.end(); i++) {
		settings->setValue(i.key(), i.value());
	}

	setting_dialog->close();


	// Show status
	this->lbl_status->setText("Save settings successfully!\t");
}

void MainWindow::on_settings_choose_words_file()
{
	words_file_path = QFileDialog::getOpenFileName(setting_dialog, "Choose words file",
		".");
	if (!words_file_path.isEmpty() && le_irregular_words_path)
		le_irregular_words_path->setText(words_file_path);
	else if (words_file_path.isEmpty()) {
		lbl_status->setText(tr("<font color=\"red\">Invalid file path</font>\t").arg(words_file_path));
	}
	else {
		QMessageBox::warning(setting_dialog, "Warning", tr("Unkown error!"), QMessageBox::Ok);
	}
}

void MainWindow::on_settings_reset()
{
	this->current_study = 0;
}

void MainWindow::on_actionAbout_A_triggered()
{
	QMessageBox::information(
		this, "About us",
		"<b>French Conjugator v1.0</b> aims to help people who suffer from the disgusting verb conjugations"
		" in French laguage in a related easy way. "
		"Allrights reserved © <a href=\"https://github.com/shijie-xu/conj/releases\">Shi-Jie Xu</a>. "
		"Thanks to <a href=\"http://verbe.cc\">http://verbe.cc</a> for providing verbs conjugation interface,"
		" to <a href=\"https://tech.yandex.com/translate/\">Yandex translate</a> for the translation api,"
		" and to <a href=\"https://en.wiktionary.org/wiki/Category:French_irregular_verbs\">Wikitionary</a>"
		" for providing irregular verbs list. Some of exercise text are downloaded from "
		"<a href=\"https://b-ok.org/\">ZLibrary</a>. "
		"See more details <a href=\"https://en.wiktionary.org/wiki/Appendix:French_irregular_verbs\">here.</a><br>"
		+ tr("Built with Qt %1 on %2.").arg(QT_VERSION_STR).arg(compiled_day()),
		QMessageBox::Yes, QMessageBox::Yes);
}

void MainWindow::on_le_input_returnPressed()
{
	// Check answer
	passed_cycles++;
	QString response = ui->le_input->text();
	if (quiz_answer.isEmpty()) {
		right_cycles++;
		lbl_status->clear();
		if (this->right_study.contains((this->quiz_word)))
			this->right_study[this->quiz_word]++;
		else
			this->right_study[this->quiz_word] = 1;
	}
	else if (quiz_answer == response) {
		right_cycles++;
		lbl_status->setText("<font color=\"green\">Right!\t</font>");
		if (this->right_study.contains(this->quiz_word))
			this->right_study[this->quiz_word]++;
		else
			this->right_study[this->quiz_word] = 1;
	}
	else {
		lbl_status->setText(tr("Wrong: <font color=\"red\">%1.</font>\t").arg(quiz_answer));
		if (this->wrong_study.contains(this->quiz_word))
			this->wrong_study[this->quiz_word]++;
		else
			this->wrong_study[this->quiz_word] = 1;
	}
	// Update ui
	this->progress->setRange(0, this->quiz_cycles);
	this->progress->setValue(passed_cycles);
	ui->le_input->clear();
	ui->lst_history->insertItem(0, this->quiz_word + tr("\t%1%").arg(this->study_history[this->quiz_word] * 100.0));
	single_conj_quiz();
}


void MainWindow::on_lst_history_itemClicked(QListWidgetItem* item)
{
	QString search_url = "http://en.wiktionary.org/wiki/" + item->text().split("\t").first();
	ui->webEngineView->load(search_url);
	ui->tabWidget->setCurrentIndex(1);
	//QMessageBox::information(this, "Wikitionary",
	//	tr("The author just HATE MSVC kit and no QtWebView module can be compiled with on Windows.<br>Please click <a href=\"%1\">%2</a>").arg(search_url).arg(search_url));
}

void MainWindow::on_actionNew_Quiz_N_triggered()
{
	// New quiz
	this->passed_cycles = 0;
	this->right_cycles = 0;

	this->lbl_status->setText("New quiz begins.\t");
	this->progress->setValue(passed_cycles);

	ui->lbl_word->clear();
	ui->lbl_tense->clear();
	ui->le_input->clear();

	single_conj_quiz();
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

		this->progress->setValue(i + 1);
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
	if (ui->chk_word->isChecked())
		speech->say(this->quiz_word);
	else
		speech->say(this->pro + " " + this->quiz_answer);
}

void MainWindow::on_hide_window()
{
	if (this->bshow) {
		this->setWindowState(Qt::WindowMinimized);
	}
}

void MainWindow::on_le_pronom_returnPressed()
{
	this->speech->say(this->pronom_answer);
	if (ui->le_pronom->text() == this->pronom_answer) {
		this->lbl_status->setText("<font color=\"green\">Right!\t</font>");
	}
	else {
		this->lbl_status->setText(tr("Wrong: <font color=\"red\">%1\t</font>")
			.arg(this->pronom_answer));
	}
	ui->le_pronom->clear();
	single_pronom_quiz();
}

void MainWindow::single_sentence_complete_quiz()
{
	qDebug() << "new sentence complete quiz begins";
	update_complete_percent();

	int min_length = ui->spin_min->value();
	int max_length = ui->spin_max->value();
	int avg_freq = ui->spin_freq->value();

	ui->lst_words->clear();
	ui->te_sentence->clear();
	bool succ = false;
	int loops = 0;
	do {
		loops++;
		if (loops > 100) this->lbl_status->setText("Loops too large, please increase threshold!");

		//int k = QRandomGenerator::global()->bounded(this->sent_list.count());
		int k = rand() % (this->sentence_list.count());
		this->quiz_sentence = this->sentence_list.at(k).trimmed();
		this->quiz_sentence_learned_times = read_learned_times_from_db(this->quiz_sentence.toLower());

		// Chech duplication
		int threshold = ui->spin_threshold->value();
		if ( this->quiz_sentence_learned_times >= threshold) {
			continue;
		}

		QList<QString> words = this->quiz_sentence.split(" ");
		//QVector<QStringRef> words_ref = this->quiz_sent.splitRef(" ");
		// Check length
		if (words.count() < min_length || words.count() > max_length)continue;
		// Calclate difficulty
		int diff = 0;
		for (QList<QString>::iterator it = words.begin(); it != words.end(); it++) {
			diff += this->words_freq[*it];
		}
		// Check frequency
		if (1000.0 * diff / this->total_words < avg_freq) continue;

		// Shuffle words sequence
		//std::random_shuffle(words_ref.begin(), words_ref.end());
		std::random_shuffle(words.begin(), words.end());

		// Parse sentence constructure
		//qDebug() << "split sentence";
		//sentence_parse.set_setence(this->quiz_sentence);
		//QList<QString> split_sentence = sentence_parse.py_fold_sentence();
		//qDebug() << split_sentence << "\nsplit ok.";

		// Show words in list view
		
		for (int i = 0; i < words.count(); ++i) {
			QString w = words.at(i);
			if (!w.isEmpty()) {
				ui->lst_words->addItem(words.at(i));
				succ = true;
			}
		}/*
		for (int i = 0; i < split_sentence.count(); ++i) {
			QString w = split_sentence.at(i).trimmed();
			if (!w.isEmpty()) {
				ui->lst_words->addItem(split_sentence.at(i).toLocal8Bit());
				succ = true;
			}
		}*/
	} while (!succ);
	qDebug() << "loops: " << loops;

	// Translate
	// Install openssl dll firstly: http://slproweb.com/products/Win32OpenSSL.html
	// Then create yandex api
	QString target_lang = "en";
	QString url = tr("https://translate.yandex.net/api/v1.5/tr.json/translate?key=trnsl.1.1.20191116T145524Z.a006c2b4351496e9.677dfb7949c1d615bf5aab3d2071ddb45ee750f7&text=%1&lang=fr-%2").arg(this->quiz_sentence).arg(target_lang);
	this->lbl_network->setText("Fetching translation...\t");

	// Set unsync get
	// Quit the last thread right now
	if (thrd) {
		thrd->quit();
		thrd->wait();
		delete unsync_conj;
	}

	thrd = new QThread();

	unsync_conj = new Conjugate();
	unsync_conj->moveToThread(thrd);
	unsync_conj->set_trans_url(url);

	connect(thrd, &QThread::started,
		unsync_conj, &Conjugate::doWork);
	connect(unsync_conj, &Conjugate::TranslationComplete, this, &MainWindow::update_translation);
	connect(this, &MainWindow::close_translation_safely, thrd, &QThread::quit);

	thrd->start();
	ui->btn_ok->setEnabled(false);
}

// Check correction of sentences
void MainWindow::on_btn_ok_clicked()
{
	//if (!ui->btn_ok->isEnabled()) return;
	this->sent_complete++;
	QString respone = ui->te_sentence->toPlainText();
	ui->lbl_sent->clear();
	ui->lbl_origin->clear();
	if (respone.isEmpty()) {
		// empty response as WRONG
		ui->lbl_origin->setText("<b>Empty</b>");
		QSound::play(":/incorrect.wav");
	}
	else if (ui->te_sentence->toPlainText().trimmed() == this->quiz_sentence) {
		ui->lbl_origin->setText("<b>Right!</b>");
		this->lbl_status->setText("<font color=\"green\">OK.\t</font>");
		this->right_sent_complete++;
		QSound::play(":/correct.wav");
		// update learned times
		this->quiz_sentence_learned_times++;
		update_learned_times_into_db(this->quiz_sentence.toLower(), this->quiz_sentence_learned_times);
		// update sentences count of today's learning
		this->today_learning_sentences++;
		update_everyday_learning();
	}
	else {
		int k;
		for (k = 0; k < respone.count(); k++) {
			if (respone.at(k) != this->quiz_sentence.at(k))break;
		}
		this->lbl_status->setText("<font color=\"red\">Wrong.\t</font>");
		ui->lbl_origin->setText(tr("<font color=\"green\">Respon:</font> %1<font color=\"red\">%2</font>")
			.arg(respone.left(k))
			.arg(respone.remove(0, k)));
		QSound::play(":/incorrect.wav");
	}

	// setup new quiz
	double alpha = 0.8;
	ui->lbl_words->setText(tr("Complete (<font color=\"blue\">%1</font>/%2/%3%/%4)")
		.arg(this->right_sent_complete)
		.arg(this->sent_complete)
		.arg(100 * this->right_sent_complete / this->sent_complete)
		.arg((int)((alpha * this->sent_complete - this->right_sent_complete) / (1 - alpha))));
	ui->lbl_sent->setText("<font color=\"green\">Origin:</font> " + this->quiz_sentence);
	ui->lbl_trans->setText("<font color=\"blue\">Transl:</font> " + this->quiz_translation);

	single_sentence_complete_quiz();
}

void MainWindow::on_lst_words_itemClicked(QListWidgetItem* item)
{
	if (item->text().at(0) == "*")return;
	QString cur_word = item->text();
	item->setText(tr("*%1").arg(cur_word));
	this->lbl_status->setText(tr("You chose %1.\t").arg(cur_word));
	//update_words_hint(cur_word);

	// Choose colors, ref here: https://blog.csdn.net/daichanglin/article/details/1563299
	// Colorful apperance may reduce the pressure of studying.
	QStringList colors = {
		"#FFB6C1",      //浅粉红
		"#FFC0CB",      //粉红
//        "#FF69B4",      //热情的粉红
//        "#DA70D6",      //兰花的紫色
//        "#EE82EE",      //紫罗兰
//        "#9400D3",      //深紫罗兰色
//        "#7B68EE",      //适中的板岩暗蓝灰色
		"#E6E6FA",      //熏衣草花的淡紫色
//        "#4169E1",      //皇军蓝
//        "#6495ED",      //矢车菊的蓝色
		"#B0C4DE",      //淡钢蓝
		"#87CEFA",      //淡蓝色
		"#87CEEB",      //天蓝色
//        "#00BFFF",      //深天蓝
		"#ADD8E6",      //淡蓝
		"#B0E0E6",      //火药蓝
//        "#F0FFFF",      //蔚蓝色
		"#E1FFFF",      //淡青色
		"#AFEEEE",      //苍白的绿宝石
//        "#00FFFF",      //青色
//        "#00CED1",      //深绿宝石
//        "#48D1CC",      //适中的绿宝石
//        "#20B2AA",      //浅海洋绿
		"#40E0D0",      //绿宝石
//        "#00FA9A",      //适中的碧绿色
//        "#F5FFFA",      //适中的春天的绿色
//        "#F0FFF0",      //蜂蜜
		"#90EE90",      //淡绿色
		"#98FB98",      //苍白的绿色
//        "#6B8E23",      //米色(浅褐色)
//        "#FAFAD2",      //浅秋麒麟黄
//        "#FFFFF0",      //象牙
//        "#FFFFE0",      //浅黄色
//        "#BDB76B",      //深卡其布
//        "#FFFACD",      //柠檬薄纱
//        "#F0E68C",      //卡其布
		"#FFD700",      //金
//        "#FFFAF0",      //花的白色
//        "#FDF5E6",      //老饰带
		"#F5DEB3",      //小麦色
		"#FFE4B5",      //鹿皮鞋
//        "#FFA500",      //橙色
		"#FFEFD5",      //番木瓜
		"#FFEBCD",      //漂白的杏仁
		"#FFDEAD",      //Navajo白
		"#FAEBD7",      //古代的白色
//        "#D2B48C",      //晒黑
		"#FFE4C4",      //(浓汤)乳脂,番茄等
//        "#FF8C00",      //深橙色
//        "#FAF0E6",      //亚麻布
//        "#CD853F",      //秘鲁
		"#FFDAB9",      //桃色
//        "#F4A460",      //沙棕色
//        "#FFF5EE",      //海贝壳
		"#FFA07A",      //浅鲜肉(鲑鱼)色
//        "#FF7F50",      //珊瑚
//        "#FF4500",      //橙红色
//        "#E9967A",      //深鲜肉(鲑鱼)色
//        "#FF6347",      //番茄
		"#FFE4E1",      //薄雾玫瑰
//        "#FA8072",      //鲜肉(鲑鱼)色
//        "#FFFAFA",      //雪
//        "#FFFFFF",      //纯白
//        "#F5F5F5",      //白烟
//        "#DCDCDC",      //Gainsboro
		"#D3D3D3",      //浅灰色
	};
	// Compute the hash function of string, then decide the color of the string,
	// therefore each string has the unique permanent color chose.
	int ch = (unsigned int)string_hash(cur_word) % colors.count();
	ui->te_sentence->insertHtml(tr("<span style=\"background-color: %1\">%2</span>")
		.arg(colors.at(ch))
		.arg(cur_word) + " ");
	if (ui->te_sentence->toPlainText().trimmed() == this->quiz_sentence) {
		on_btn_ok_clicked();
	}
}

void MainWindow::on_btn_play_clicked()
{
	this->speech->say(this->quiz_sentence);
}

void MainWindow::on_clear_complete_operation()
{
	ui->te_sentence->clear();
	for (int i = 0; i < ui->lst_words->count(); i++) {
		QString text = ui->lst_words->item(i)->text();
		if (text.at(0) == "*")
			ui->lst_words->item(i)->setText(text.remove(0, 1));
	}
}

void MainWindow::on_chk_show_pronoms_stateChanged(int)
{
	QString pos[] = { "subject", "direct object", "indirect object", "reflexive", "emphatic" };
	QString pronoms[] = { "je", "tu", "il", "elle", "nous", "vous", "ils", "elles" };
	QString pro[] = {
		"je", "me", "me", "me", "moi",
		"tu", "te", "te", "te", "toi",
		"il", "le", "lui", "se", "lui",
		"elle", "la", "lui", "se", "elle",
		"nous", "nous", "nous", "nous", "nous",
		"vous", "vous", "vous", "vous", "vous",
		"ils", "les", "leur", "se", "eux",
		"elles", "les", "leur", "se", "elles"
	};
	if (ui->chk_show_pronoms->isChecked()) {
		ui->tbl_pronom->setColumnCount(5);
		ui->tbl_pronom->setRowCount(8);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 5; j++) {
				QString w = pro[i * 5 + j];
				QTableWidgetItem* item = new QTableWidgetItem(w);
				ui->tbl_pronom->setItem(i, j, item);
			}
		}
	}
	else {
		ui->tbl_pronom->clear();
	}
}

void MainWindow::update_translation(QString translation)
{
	QJsonDocument doc = QJsonDocument::fromJson(translation.toLocal8Bit().data());
	if (!doc.isEmpty()) {
		//qDebug() << doc;
		QJsonObject obj = doc.object();
		if (!obj.isEmpty()) {
			//qDebug() << obj;
			QJsonArray arr = obj.take("text").toArray();
			if (!arr.isEmpty()) {
				this->quiz_translation = arr.toVariantList().at(0).toString();
				this->lbl_network->setText("Fetched OK.\t");
			}
		}
	}else{
		this->quiz_translation = "<no translation>";
		this->lbl_network->setText("cannot translate.\t");
	}
	ui->btn_ok->setEnabled(true);
	ui->lbl_trans->setText("<font color=\"blue\">Transl:</font> " + this->quiz_translation);
	emit close_translation_safely();

	qDebug() << "update translation OK: "<< this->quiz_translation;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	if (index == 4) {
		// Update sentence complete tabs
		//update_tab_sentence_complete_history();
	}
}

void MainWindow::on_te_sentence_cursorPositionChanged()
{
	QTextCursor cur = ui->te_sentence->textCursor();
	while (cur.position() != 0 && ui->te_sentence->toPlainText().at(cur.position() - 1) != " ") {
		ui->te_sentence->moveCursor(QTextCursor::Left);
		cur = ui->te_sentence->textCursor();
	}
}

void MainWindow::on_btn_clear_clicked()
{
	on_clear_complete_operation();
}

