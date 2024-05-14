#include "database.h"
#include <QStandardPaths>


DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    qDebug() << path+"/"+DATABASE_NAME;
    if(!QFile(path+"/"+DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);

    QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    qDebug() << path+"/"+DATABASE_NAME;
    db.setDatabaseName(path+"/"+DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    TABLE_NAME     " VARCHAR(255)    NOT NULL,"
                    TABLE_PIC      " BLOB            NOT NULL,"
                    TABLE_EDIT     " VARCHAR(255)    NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_NAME ", " TABLE_PIC "," TABLE_EDIT " ) "
                  "VALUES (:Name, :Pic, :Edit)");
    query.bindValue(":Name",        data[0].toString() );
    query.bindValue(":Pic",         data[1].toByteArray() );
    query.bindValue(":Edit",        data[2].toString() );

    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoTable(const QString &name, const QByteArray &pic, const QString &name2)
{
    QVariantList data;
    data.append(name);
    data.append(pic);
    data.append(name2);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}


bool DataBase::deleteIntoTable(const int &name)
{
    QSqlQuery query;
    query.prepare("DELETE FROM " TABLE "  WHERE id = :Name");
    QVariantList data;
    data.append(name);
    query.bindValue(":Name",        data[0].toString());

    qDebug() << name ;

    if(!query.exec()){
        qDebug() << "error delete into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


//QByteArray DataBase::selectIntoTable(const int &name)
//{
//    QSqlQuery query;
//    query.prepare("SELECT " TABLE_PIC " FROM " TABLE "  WHERE id = :Name");
//    QVariantList data;
//    data.append(name);
//    query.bindValue(":Name",        data[0].toString());

//    qDebug() << name ;

//    if(!query.exec()){
//        qDebug() << "error delete into " << TABLE;
//        qDebug() << query.lastError().text();
//        return false;
//    } else {

// https://wiki.qt.io/How_to_Store_and_Retrieve_Image_on_SQLite?ref=driverlayer.com%2Fweb&veaction=edit

//        QByteArray outByteArray = query.value( 0 ).toByteArray();
//        QPixmap outPixmap = QPixmap();
//        outPixmap.loadFromData( outByteArray );
//        db.close();

//        return true;
//    }
//    return false;
//}
