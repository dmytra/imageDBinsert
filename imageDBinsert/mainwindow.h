#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
 
#include "database.h"
 
namespace Ui {
class MainWindow;
}
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
 
private slots:
    void on_screenButton_clicked();
    void slotCurrentPic(QModelIndex index);

    void on_Delete_Button_clicked();


    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::MainWindow *ui;
    DataBase        *db;
    QSqlTableModel  *model;
 
private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};
 
#endif // MAINWINDOW_H
