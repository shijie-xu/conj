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

    QMap<QString,double> map_read_from_variant(QVariantMap);
    QVariantMap variant_read_from_map(QMap<QString, int>, QMap<QString, int>);

    QString take_a_word();
    QString take_proun(QString proun, QString verb, QString tense);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionSettings_S_triggered();

    void on_actionExit_X_triggered();

    void on_settings_cancel();
    void on_settings_save();
    void on_settings_choose_words_file();

    void on_actionAbout_A_triggered();

    void on_le_input_returnPressed();

    void on_lst_history_itemClicked(QListWidgetItem *item);

    void on_actionNew_Quiz_N_triggered();

    void on_actionFetch_Dictionary_F_triggered();

private:
    Ui::MainWindow *ui;

    QDialog *setting_dialog;
    QSettings *settings;

    QLineEdit *le_words_path;
    QSpinBox *spin_quiz_number;
    QSpinBox *spin_quiz_rate;
    QSpinBox *spin_quiz_new;

    QCheckBox *chk_ind_present;
    QCheckBox *chk_ind_passe_compose;
    QCheckBox *chk_ind_imparfait;
    QCheckBox *chk_ind_plus_que_parfait;
    QCheckBox *chk_ind_futur_simple;
    QCheckBox *chk_ind_futur_anterieur;

    QCheckBox *chk_cond_present;
    QCheckBox *chk_cond_passe;

    QCheckBox *chk_sub_present;
    QCheckBox *chk_sub_passe;
    QCheckBox *chk_sub_imparfait;
    QCheckBox *chk_sub_plus_que_parfait;


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

    QMap<QString, bool> tenses_switch;
    QMap<QString, double> study_history;
    QMap<QString, int> right_study;
    QMap<QString, int> wrong_study;


    QString quiz_answer;
    int quiz_proun;
    int quiz_tense;

    Conjugate conjugator;
    Conjugate fetch_dict;

    QList<QString> prouns;
};

#endif // MAINWINDOW_H
