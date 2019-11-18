#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "conjugate.h"

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QLineEdit>
#include <QProgressBar>
#include <QLabel>
#include <QJsonArray>
#include <QSpinBox>
#include <QMap>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QVariantMap>
#include <QTextToSpeech>
#include <QVoice>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void single_quiz();
    void update_tab3();
    void update_tab_cs();
    void init_tab_cs();

    QMap<QString,double> map_read_from_variant(QVariantMap);
    QVariantMap variant_read_from_map(QMap<QString, int>, QMap<QString, int>);

    QString take_a_word();

    void single_pronom_quiz();

    void words_freq_calc(QString file);

    qint64 string_hash(QString str);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionSettings_S_triggered();

    void on_actionExit_X_triggered();

    void on_settings_cancel();
    void on_settings_save();
    void on_settings_choose_words_file();
    void on_settings_reset();


    void on_actionAbout_A_triggered();

    void on_le_input_returnPressed();

    void on_lst_history_itemClicked(QListWidgetItem *item);

    void on_actionNew_Quiz_N_triggered();

    void on_actionFetch_Dictionary_F_triggered();

    void on_btn_play_word_clicked();

    void on_hide_window();

    void on_le_pronom_returnPressed();

    void new_sentence_complete_quiz();

    void on_btn_ok_clicked();

    void on_lst_words_itemClicked(QListWidgetItem *item);

    void on_btn_play_clicked();

    void on_clear_complete_operation();

    void on_chk_show_pronoms_stateChanged(int arg1);

    void update_translation(QString trans);

    void on_tabWidget_currentChanged(int index);

    void on_te_sentence_cursorPositionChanged();

    void on_btn_clear_clicked();

private:
    Ui::MainWindow *ui;

    QDialog *setting_dialog;
    QSettings *settings;

    QLineEdit *le_words_path;
    QSpinBox *spin_quiz_number;
    QSpinBox *spin_quiz_rate;
    QSpinBox *spin_quiz_new;

    // indicatif_tenses = 'présent', 'passé-composé', 'imparfait', 'plus-que-parfait',
    // 'passé-simple', 'passé-antérieur', 'futur-simple', 'futur-antérieur'
    // conditionnel_teneses = 'présent', 'passé',
    // subjonctif_tenses = 'présent', 'passé', 'imparfait', 'plus-que-parfait'
    // imperatif_tenses = 'présent', 'passé',
    // participe_tenses = 'présent', 'passé',
    QCheckBox *chk_ind_present;
    QCheckBox *chk_ind_passe_compose;
    QCheckBox *chk_ind_imparfait;
    QCheckBox *chk_ind_plus_que_parfait;
    QCheckBox *chk_ind_passe_simple;
    QCheckBox *chk_ind_passe_anterieur;
    QCheckBox *chk_ind_futur_simple;
    QCheckBox *chk_ind_futur_anterieur;

    QCheckBox *chk_cond_present;
    QCheckBox *chk_cond_passe;

    QCheckBox *chk_sub_present;
    QCheckBox *chk_sub_passe;
    QCheckBox *chk_sub_imparfait;
    QCheckBox *chk_sub_plus_que_parfait;

    QCheckBox *chk_imp_present;
    QCheckBox *chk_imp_passe;

    QCheckBox *chk_par_present;
    QCheckBox *chk_par_passe;

    QTextToSpeech *speech;


    QProgressBar *progress;
    QLabel *lbl_status;
    QLabel *lbl_network;
    QString words_file_path;
    QJsonArray words_list;

    int current_study;

    int words_count;

    int quiz_cycles;
    int passed_cycles;
    int right_cycles;

    int quiz_rate;
    int quiz_new_rate;
    QString quiz_word;

    int sent_complete;
    int right_sent_complete;

    QMap<QString, bool> tenses_switch;
    QMap<QString, double> study_history;
    QMap<QString, int> right_study;
    QMap<QString, int> wrong_study;


    QString quiz_answer;
    int quiz_proun;
    int quiz_tense;
    QString pro;

    QString pronom_answer;

    Conjugate conjugator;
    Conjugate fetch_dict;

    QList<QString> prouns;
    bool bshow;

    QList<QString> sent_list;
    QString quiz_sent;
    QString quiz_trans;

    QMap<QString,int> words_freq;
    qint64 total_words;
    QList<QString> words_learned;
    Conjugate *unsync_conj;
    QThread *thrd;
};

#endif // MAINWINDOW_H
