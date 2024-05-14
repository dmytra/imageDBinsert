#include "mainwindow.h"
#include "ui_mainwindow.h"
 
#include <QApplication>
#include <QBuffer>
#include <QScreen>

#include <QMessageBox>
#include <QSqlRecord>
#include <QByteArray>
#include <QFileDialog>
#include <QFile>
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 
    db = new DataBase();
    db->connectToDataBase();
 
    this->setupModel(TABLE,
                     QStringList() << "id"
                                   << "Имя изображения"
                                   << "изображение"
                                   << "описание"
                     );
 
    this->createUI();
}
 
MainWindow::~MainWindow()
{
    delete ui;
}
 
void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlTableModel(this);
    model->setTable(tableName);
 
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}
 
void MainWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
 
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentPic(QModelIndex)));

    model->select();

    // Qtableview editable cells
    // https://forum.qt.io/topic/122581/qtableview-editable-cells

}
 
void MainWindow::on_screenButton_clicked()
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap inPixmap = screen->grabWindow( 0 );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );
 
    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray, " xxx ");
 
    model->select();
}
 
void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());
    ui->label->setPixmap(outPixmap.scaled(400,300));
}

void MainWindow::on_Delete_Button_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Do you really want to delete?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        QSqlRecord record;
        int i = ui->tableView->currentIndex().row();

        record = model->record(i);
        int  recordId = record.value("id").toInt(nullptr);

        if( DataBase::deleteIntoTable(recordId) )
        qDebug() << "Yes was delete COL Into Table";

        ui->tableView->reset();
        ui->tableView->repaint();
        model->select();
    }
    else {
        qDebug() << "Yes was *not* clicked";

    }
}

void MainWindow::on_pushButton_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap pixmapTarget = QPixmap(filename);
    //pixmapTarget = pixmapTarget.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio , Qt::SmoothTransformation);
    ui->label->setPixmap(pixmapTarget);

    // https://stackoverflow.com/questions/34181449/inserting-an-image-as-a-blob-type-to-database-in-qt-c
    // https://wiki.qt.io/How_to_Store_and_Retrieve_Image_on_SQLite
    // https://forum.qt.io/topic/135675/save-and-load-pictures-in-qt-c-from-sqlite/9
    QByteArray inByteArray;
    QBuffer buffer(&inByteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmapTarget.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format

    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray, " xxx222xxx ");

    model->select();
}


void MainWindow::on_pushButton_save_clicked()
{
    // https://forum.qt.io/topic/135675/save-and-load-pictures-in-qt-c-from-sqlite/9

    QSqlRecord record;
    int i = ui->tableView->currentIndex().row();

    record = model->record(i);
    int  recordId = record.value("id").toInt(nullptr);

    QPixmap outPixmap = QPixmap();

    outPixmap.loadFromData(model->data(model->index(ui->tableView->currentIndex().row(), 2)).toByteArray());
    ui->label->setPixmap(outPixmap.scaled(400,300));


    QByteArray inByteArray;
    QBuffer buffer(&inByteArray);
    buffer.open(QIODevice::WriteOnly);
    outPixmap.save(&buffer, "PNG");

    //    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
    //                                                    tr("Images (*.png *.xpm *.jpg)"),outPixmap);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "/untitled.png",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    // https://www.linux.org.ru/forum/development/10938329

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (!file.fileName().isEmpty()) {
        file.write(inByteArray);

        file.close();
    }
}

