#include "sqlconversationmodel.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <iostream>

//conversation data table

static const char *conversationsTableName = "Conversations";

static void createTable()   //Create conversation table
{
    if (QSqlDatabase::database().tables().contains(conversationsTableName)) {
        // The table already exists; we don't need to do anything.
        return;

        qDebug("conversation table is exist\n");
        QSqlQuery query;
        if (!query.exec("DROP TABLE IF EXISTS 'Conversations'"))
            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }

    QSqlQuery query;
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS 'Conversations' ("
        "'author' TEXT NOT NULL,"
        "'recipient' TEXT NOT NULL,"
        "'timestamp' TEXT NOT NULL,"
        "'message' TEXT NOT NULL,"
        "FOREIGN KEY('author') REFERENCES Contacts ( name ),"
        "FOREIGN KEY('recipient') REFERENCES Contacts ( name )"
        ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }

    //기본 대화
    //query.exec("INSERT INTO Conversations VALUES('Me', 'Albert Einstein', '2020-10-01T11:24:53', 'Hi!')");
    //query.exec("INSERT INTO Conversations VALUES('Albert Einstein', 'Me', '2020-10-01T14:36:16', 'Good morning.')");
    /*query.exec("INSERT INTO Conversations VALUES('Me', 'Albert Einstein', '2020-10-01T11:24:53', '안녕!')");
    query.exec("INSERT INTO Conversations VALUES('Albert Einstein', 'Me', '2020-10-01T12:36:16', '안녕!')");
    query.exec("INSERT INTO Conversations VALUES('Me', 'Albert Einstein', '2020-10-01T12:36:53', '뭐하고 있어?')");
    query.exec("INSERT INTO Conversations VALUES('Albert Einstein', 'Me', '2020-10-01T12:37:16', '나 암호학 과제 하는중!')");*/


    query.exec("INSERT INTO Conversations VALUES('Me', 'Ernest Hemingway', '2020-10-07T14:36:06', 'Hello!')");
    query.exec("INSERT INTO Conversations VALUES('Ernest Hemingway', 'Me', '2020-10-07T14:36:16', 'Good afternoon.')");
    query.exec("INSERT INTO Conversations VALUES('Hans Gude', 'Me', '2020-10-12T06:30:02', 'God morgen. Har du fått mitt maleri?')");
    query.exec("INSERT INTO Conversations VALUES('Me', 'Hans Gude', '2020-10-12T08:21:03', 'God morgen, Hans. Ja, det er veldig fint. Tusen takk! "
               "Hvor mange timer har du brukt på den?')");
}

SqlConversationModel::SqlConversationModel(QObject *parent) : QSqlTableModel(parent)
{
    createTable();//create table
    setTable(conversationsTableName);
    setSort(2, Qt::DescendingOrder);
    // Ensures that the model is sorted correctly after submitting a new row.
    // 새 행을 제출한 후 모델이 올바르게 정렬되었는지 확인
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QString SqlConversationModel::recipient() const
{
    return m_recipient;
}

void SqlConversationModel::setRecipient(const QString &recipient)
{
    if (recipient == m_recipient)
        return;

    m_recipient = recipient;

    const QString filterString = QString::fromLatin1(
        "(recipient = '%1' AND author = 'Me') OR (recipient = 'Me' AND author='%1')").arg(m_recipient);
    setFilter(filterString);
    select();

    emit recipientChanged();
}

QVariant SqlConversationModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> SqlConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "author";
    names[Qt::UserRole + 1] = "recipient";
    names[Qt::UserRole + 2] = "timestamp";
    names[Qt::UserRole + 3] = "message";
    return names;
}

void SqlConversationModel::sendMessage(const QString &recipient, const QString &message)    //테이블에 새 레코드 삽입
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlRecord newRecord = record();

    if(0)
    {
        newRecord.setValue("author", recipient);
        newRecord.setValue("recipient", "Me");
        newRecord.setValue("timestamp", timestamp);
        newRecord.setValue("message", message);

        if (!insertRecord(rowCount(), newRecord)) {
            qWarning() << "Failed to send message:" << lastError().text();
            return;
        }
    }else
    {
        newRecord.setValue("author", "Me");
        newRecord.setValue("recipient", recipient);
        newRecord.setValue("timestamp", timestamp);
        newRecord.setValue("message", message);
        //std::cout<<message.toStdString()<<std::endl;    //print message at Debug console

        if (!insertRecord(rowCount(), newRecord)) {
            qWarning() << "Failed to send message:" << lastError().text();
            return;
        }
    }

    submitAll();
}
