#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSettings_S_triggered();

    void on_actionExit_X_triggered();

    void on_settings_cancel();
    void on_settings_save();

    void on_actionAbout_A_triggered();

private:
    Ui::MainWindow *ui;

    QDialog *setting_dialog;
};
#endif // MAINWINDOW_H
