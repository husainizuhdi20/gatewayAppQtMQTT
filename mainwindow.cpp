#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoglabel.h"
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QUrl>
#include <QSqlError>
#include <qsqlerror.h>
#include <stdio.h>
#include <time.h>
#include <bits/stdc++.h>
#include <QDateTime>
#include <bits/stdc++.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect broker host
    m_client = new QMqttClient(this);
    m_client->setHostname("localhost");
    m_client->setPort(1883);


    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);


    //connect topic dan receive message
    //topic1
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message,  const QMqttTopicName &topic) {
    const QString content = QDateTime::currentDateTime().toString()
        + QLatin1String(" Received Topic: ")
        + topic.name()
        + QLatin1String(" Message: ")
        + message
        + QLatin1Char('\n');
    message2 = message;
    ui->editLog->insertPlainText(content);

    if(topic.name() == "powerontime"){
//        parsePowerOn();
        parseData("poweron","poweron");
        connectionDb();
        insertStatus("Power On");

        if(stsMesin == "0"){
            insertDurasi("Power On");
        }
    }
    if(topic.name() == "mtbf"){
//        parseMtbf();
        parseData("mtbf", "mtbf");
        connectionDb();
        insertStatus("mtbf");
        insertDurasi("mtbf");
    }
    if(topic.name() == "idletime"){
        parseData("idle", "idle");
//        parseIdle();
        connectionDb();
        insertStatus("Idle");

        if(stsMesin == "0"){
            insertDurasi("Idle");
        }
    }
    if(topic.name() == "setuptime"){
//        parseResult();
//        parseSetup();
        parseData("setup", "setup");
        connectionDb();
        insertStatus("Setup");

        if(stsMesin == "0"){
            insertDurasi("Setup");
        }
    }
    if(topic.name() == "loginontime"){
//        parseLoginOn();
        parseData("loginon", "loginon");
        connectionDb();
        insertStatus("Login On");

        if(stsMesin == "0"){
            insertDurasi("Login On");
        }
    }
    if(topic.name() == "runtime"){
        parseData("run", "run");
//        parseRunning();
        connectionDb();
        insertStatus("Running");

        if(stsMesin == "0"){
            insertDurasi("Running");
        }

    }
    if(topic.name() == "breaktime"){
//        parseBreak();
        parseData("break", "break");
        connectionDb();
        insertStatus("Break");

        if(stsMesin == "0"){
            insertDurasi("Break");
        }
    }

    if(topic.name() == "qctime"){
//        parseQc();
        parseData("qc", "qc");
        connectionDb();
        insertStatus("QC ");

        if(stsMesin == "0"){
            insertDurasi("QC");
        }
    }

    if(topic.name() == "breakdowntime"){
//        parseBreakdown();
        parseData("breakdown", "breakdown");
        connectionDb();
        insertStatus("Breakdown");

        if(stsMesin == "0"){
            insertDurasi("Breakdown");
        }
    }

    if(topic.name() == "downtime"){
//        parseDown();
        parseData("downtime", "downtime");
        connectionDb();
        insertStatus("Downtime");

        if(stsMesin == "0"){
            insertDurasi("Downtime");
        }
    }

    if(topic.name() == "goodproduct"){
//        parseGood();
        parseData("goodproduct", "goodproduct");
        connectionDb();
        insertStatus("Good Product");

//        if(stsMesin == "0"){
//            insertDurasi("Good Product");
//        }
    }

    if(topic.name() == "rejectproduct"){
//        parseReject();
        parseData("rejectproduct", "rejectproduct");
        connectionDb();
        insertStatus("Reject Product");

//        if(stsMesin == "0"){
//            insertDurasi("Reject Product");
//        }
    }

    if(topic.name() == "workpiecetime"){
//        parseWorkPiece();
        parseData("workpiece", "workpiece");
        connectionDb();
        insertStatus("Workpiece Setup");

        if(stsMesin == "0"){
            insertDurasi("Workpiece Setup");
        }

    }
    if(topic.name() == "toolstime"){
//        parseTools();
        parseData("tools", "tools");
        connectionDb();
        insertStatus("Tools Setup");

        if(stsMesin == "0"){
            insertDurasi("Tools Setup");
        }
    }

    if(topic.name() == "allsensor"){
        connectionDb();
        parseSensor();
        insertSensor();

    }


    });

    connect(m_client, &QMqttClient::pingResponseReceived, this, [this]() {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" PingResponse")
                    + QLatin1Char('\n');
        ui->editLog->insertPlainText(content);
    });

    connect(ui->lineEditHost, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
    connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::setClientPort);
    updateLogStateChange();

    totalrowReceived = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buttonConnect_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->lineEditHost->setEnabled(false);
        ui->spinBoxPort->setEnabled(false);
        ui->buttonConnect->setText(tr("Disconnect"));
        m_client->connectToHost();
    } else {
        ui->lineEditHost->setEnabled(true);
        ui->spinBoxPort->setEnabled(true);
        ui->buttonConnect->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
}

void MainWindow::on_buttonQuit_clicked()
{
    QApplication::quit();
}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    ui->editLog->insertPlainText(content);
}

void MainWindow::brokerDisconnected()
{
    ui->lineEditHost->setEnabled(true);
    ui->spinBoxPort->setEnabled(true);
    ui->buttonConnect->setText(tr("Connect"));
}

void MainWindow::setClientPort(int p)
{
    m_client->setPort(p);
}

void MainWindow::on_buttonPublish_clicked()
{
    if (m_client->publish(ui->lineEditTopic->text(), ui->lineEditMessage->text().toUtf8()) == -1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void MainWindow::on_buttonSubscribe_clicked()
{
    //topic1
    auto topic1 = m_client->subscribe(powerontime);
    if (!topic1) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic2 = m_client->subscribe(mtbftime);
    if (!topic2) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic3
    auto topic3 = m_client->subscribe(idletime);
    if (!topic3) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic4
    auto topic4 = m_client->subscribe(setuptime);
    if (!topic4) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic5
    auto topic5 = m_client->subscribe(loginontime);
    if (!topic5) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic6 = m_client->subscribe(runtime);
    if (!topic6) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic3
    auto topic7 = m_client->subscribe(breaktime);
    if (!topic7) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic8 = m_client->subscribe(qctime);
    if (!topic8) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic3
    auto topic9 = m_client->subscribe(breakdowntime);
    if (!topic9) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic10 = m_client->subscribe(downtime);
    if (!topic10) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic1
    auto topic11 = m_client->subscribe(goodproduct);
    if (!topic11) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic12 = m_client->subscribe(rejectproduct);
    if (!topic12) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic3
    auto topic13 = m_client->subscribe(workpiecetime);
    if (!topic13) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

    //topic2
    auto topic14 = m_client->subscribe(toolstime);
    if (!topic14) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

    //topic2
    auto topic15 = m_client->subscribe(allsensor);
    if (!topic14) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
        return;
    }

//    //topic2
//    auto topic17 = m_client->subscribe(operatorid);
//    if (!topic17) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }

//    //topic1
//    auto topic18 = m_client->subscribe(jobid);
//    if (!topic18) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//        return;
//    }

//    //topic2
//    auto topic19 = m_client->subscribe(qcid);
//    if (!topic19) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }

//    //topic3
//    auto topic20 = m_client->subscribe(stdtime);
//    if (!topic20) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//        return;
//    }

//    //topic2
//    auto topic21 = m_client->subscribe(workhours);
//    if (!topic21) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }

//    //topic1
//    auto topic22 = m_client->subscribe(objectid);
//    if (!topic22) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//        return;
//    }

//    //topic2
//    auto topic23 = m_client->subscribe(allsensor2);
//    if (!topic23) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }

//    //topic2
//    auto topic24 = m_client->subscribe(workpiecetime);
//    if (!topic24) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }

//    //topic2
//    auto topic25 = m_client->subscribe(toolstime);
//    if (!topic25) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection"));
//        return;
//    }



}

void MainWindow::parseResult()
{
    //2
    // So create an object Json Document, by reading into it all the data from the response
    QJsonDocument document = QJsonDocument::fromJson(message2);

    // Taking from the document root object
    QJsonObject root = document.object();
    /* We find the object "departament", which is the very first in the root object.
    * Use the keys() method gets a list of all objects and the first index
    * Take away the name of the object on which we obtain its value
    * */
    //ui->textEdit->append(root.keys().at(0) + ": " + root.value(root.keys().at(0)).toString());

    // The second value prescribe line
    QJsonValue jv = root.value("values"); //employes diganti values
    // If the value is an array, ...
    int i;
    if(jv.isArray()){
        // ... then pick from an array of properties
        QJsonArray ja = jv.toArray();
        // Going through all the elements of the array ...
        for(i = 0; i < ja.count(); i++){
            QJsonObject subtree = ja.at(i).toObject();
            // Taking the values of the properties and last name by adding them to textEdit
            ui->textEdit->append(subtree.value("id").toString() +
                                 " " +
                                 subtree.value("v").toString());

            //variabel id
          // dataId = subtree.value("id").toString();
//           qDebug() << "dataId " << dataId;




           mydataf[i].did = subtree.value("id").toString();
            qDebug() << "dataId " << i << " str " << mydataf[i].did;

            mydataf[i].value = subtree.value("v").toString();
            qDebug() << "dataValue" << i << " str " << mydataf[i].value ;

            if( mydataf[i].did == "m.b.thnrun"){
                thnStr =  mydataf[i].value ;

                qDebug() << "m.b.thnrun = " << thnStr ;
            }
            if( mydataf[i].did == "m.b.blnrun"){
                blnStr =  mydataf[i].value ;
                qDebug() << "m.b.thnrun = " << blnStr;


            }
            if( mydataf[i].did == "m.b.tglrun"){
                tglStr =  mydataf[i].value ;
                qDebug() << "m.b.blnrun = " << tglStr;


            }
            if( mydataf[i].did == "m.b.jamrun"){
                jamStr =  mydataf[i].value ;
                qDebug() << "m.b.jamrunn = " << jamStr;


            }
            if( mydataf[i].did == "m.b.mntrun"){
                mntStr =  mydataf[i].value ;
                qDebug() << "m.b.mntrun = " << mntStr;


            }
            if( mydataf[i].did == "m.b.dtkrun"){
                dtkStr =  mydataf[i].value ;
                qDebug() << "m.b.dtkrun = " << dtkStr;


            }
            if( mydataf[i].did == "m.b.sts"){
                stsStr =  mydataf[i].value ;
                qDebug() << "m.b.sts = " << stsStr;

            }
        }
    }

    totalrowReceived = i;


    int p = 0;
    for(p= 0; p<i;p++){
        QDateTime date = QDateTime::currentDateTime();
        mydataf[p].formattedTime = date.toString("yyyy-MM-dd hh:mm:ss,zzz");
//        formattedTime = date.toString("yyyy.MM.dd hh:mm:ss,zzz");
        QByteArray formattedTimeMsg = mydataf[p].formattedTime.toLocal8Bit();

        qDebug() << "Timestamp New : "+mydataf[p].formattedTime;

//        mydataf[p].ts = dt.toString("yyyy-MM-dd hh:mm:ss");


        //    ts = QString::fromUtf8(str.c_str());//QString::number(str);

//        qDebug() << "Value row ke " << p << " isinya data " << mydataf[p].ts;
        qDebug() << "Value row ke " << p << " isinya data " << mydataf[p].formattedTime;
        qDebug() << "Value row ke " << p << " isinya id " << mydataf[p].did;
        qDebug() << "Value row ke " << p << " isinya value " << mydataf[p].value;
//        ui->textEdit->append(mydataf[p].ts);
        ui->textEdit->append(mydataf[p].formattedTime);
        ui->textEdit->append(mydataf[p].did);
        ui->textEdit->append(mydataf[p].value);

    }
}



void MainWindow::connectionDb()
{
    //connecting to mysql
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setUserName("root");
    database.setPassword("");
    database.setDatabaseName("digitalisasi_bubut");
}

void MainWindow::insertStatus(QString statusName)
{
    if(database.open())
    {
        QSqlQuery qryStatus;


            qryStatus.prepare("INSERT INTO tb_status (tmp, status, value) VALUES (:tmp, :status, :value)");

            qryStatus.bindValue(":tmp", gabTr);
            qryStatus.bindValue(":status", statusName);
            qryStatus.bindValue(":value", stsMesin);

            if(qryStatus.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }


    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }
}

void MainWindow::insertFormPlan()
{
    if(database.open())
    {
        QSqlQuery qryStatus;

        for(int k = 0;k<totalrowReceived;k++){
            qryStatus.prepare("INSERT INTO tb_status (waktu, status, value) VALUES (:tsStatus, :status, :value)");
            qryStatus.bindValue(":tsStatus", mydataf[k].formattedTime);
            qryStatus.bindValue(":status", mydataf[k].did);
            qryStatus.bindValue(":value", mydataf[k].value);
            qDebug() << "data 1 = " << mydataf[k].formattedTime;
            qDebug() << "data 2 = " << mydataf[k].did;
            qDebug() << "data 3 = " << mydataf[k].value;

            if(qryStatus.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }
        }
    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }
}

void MainWindow::insertLogin()
{
    if(database.open())
    {
        QSqlQuery qryStatus;

        for(int k = 0;k<totalrowReceived;k++){
            qryStatus.prepare("INSERT INTO tb_status (waktu, status, value) VALUES (:tsStatus, :status, :value)");
            qryStatus.bindValue(":tsStatus", mydataf[k].formattedTime);
            qryStatus.bindValue(":status", mydataf[k].did);
            qryStatus.bindValue(":value", mydataf[k].value);
            qDebug() << "data 1 = " << mydataf[k].formattedTime;
            qDebug() << "data 2 = " << mydataf[k].did;
            qDebug() << "data 3 = " << mydataf[k].value;

            if(qryStatus.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }
        }
    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }
}

void MainWindow::insertMachine()
{
    if(database.open())
    {
        QSqlQuery qryStatus;

        for(int k = 0;k<totalrowReceived;k++){
            qryStatus.prepare("INSERT INTO tb_status (waktu, status, value) VALUES (:tsStatus, :status, :value)");
            qryStatus.bindValue(":tsStatus", mydataf[k].formattedTime);
            qryStatus.bindValue(":status", mydataf[k].did);
            qryStatus.bindValue(":value", mydataf[k].value);
            qDebug() << "data 1 = " << mydataf[k].formattedTime;
            qDebug() << "data 2 = " << mydataf[k].did;
            qDebug() << "data 3 = " << mydataf[k].value;

            if(qryStatus.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }
        }

    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }



}

void MainWindow::insertSensor()
{
//    if(database.open())
//    {
//        QSqlQuery qryStatus;

//        for(int k = 0;k<totalrowReceived;k++){
//            qryStatus.prepare("INSERT INTO tb_status (waktu, status, value) VALUES (:tsStatus, :status, :value)");
//            qryStatus.bindValue(":tsStatus", mydataf[k].formattedTime);
//            qryStatus.bindValue(":status", mydataf[k].did);
//            qryStatus.bindValue(":value", mydataf[k].value);
//            qDebug() << "data 1 = " << mydataf[k].formattedTime;
//            qDebug() << "data 2 = " << mydataf[k].did;
//            qDebug() << "data 3 = " << mydataf[k].value;

//            if(qryStatus.exec()){
//    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
//            }
//            else{
//                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
//            }
//        }
//    }
//    else{
//        QMessageBox::warning(this, "Status", "Failed");
//    }
//    if(database.open())
//    {
//        QSqlQuery qrySensor;


//            qrySensor.prepare("INSERT INTO tb_sensor (tmp, vrs, vst, vrn, vsn, vtn, ir, is, it, w, var, va, cospi, kwh, suhu_motor, suhu_oli) VALUES (:tmp, :vrs, :vst, :vrn, :vsn, :vtn, :ir, :is, :it, :w, :var, :va, :cospi, :kwh, :suhu_motor, :suhu_oli)");
//            qrySensor.bindValue(":tmp", "2023-03-07 10:49:00");
//            qrySensor.bindValue(":vrs", "10");
//            qrySensor.bindValue(":vst", "10");
//            qrySensor.bindValue(":vrn", "10");
//            qrySensor.bindValue(":vsn", "10");
//            qrySensor.bindValue(":vtn", "10");
//            qrySensor.bindValue(":ir", "10");
//            qrySensor.bindValue(":is", "10");
//            qrySensor.bindValue(":it", "10");
//            qrySensor.bindValue(":w", "10");
//            qrySensor.bindValue(":var", "10");
//            qrySensor.bindValue(":va", "10");
//            qrySensor.bindValue(":cospi", "10");
//            qrySensor.bindValue(":kwh", "10");
//            qrySensor.bindValue(":suhu_motor", "10");
//            qrySensor.bindValue(":suhu_oli", "10");

//            if(qrySensor.exec()){
//    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
//            }
//            else{
//                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
//            }
//    }
//    else{
//        QMessageBox::warning(this, "Status", "Failed");
//    }

    if(database.open())
    {
        QSqlQuery qrySensor;


            qrySensor.prepare("INSERT INTO tb_sensor (tmp, rpm, suhu_motor, vrs, vst, vrt, vrn, vsn, vtn, ir, is, it, pf) VALUES (:tmp, :rpm, :suhu_motor, :vrs, :vst, :vrt, :vrn, :vsn, :vtn, :ir, :is, :it, :pf)");
                qrySensor.bindValue(":tmp", gabTr);
                qrySensor.bindValue(":rpm", rpm);
                qrySensor.bindValue(":suhu_motor", suhuMotor);
                qrySensor.bindValue(":vrs", vrs);
                qrySensor.bindValue(":vst", vst);
                qrySensor.bindValue(":vrt", vrt);
                qrySensor.bindValue(":vrn", vrn);
                qrySensor.bindValue(":vsn", vsn);
                qrySensor.bindValue(":vtn", vtn);
                qrySensor.bindValue(":ir", ir);
                qrySensor.bindValue(":is", is);
                qrySensor.bindValue(":it", it);
                qrySensor.bindValue(":pf", pf);
//                qrySensor.bindValue(":w", "10");
//                qrySensor.bindValue(":var", "10");
//                qrySensor.bindValue(":va", "10");
//                qrySensor.bindValue(":cospi", "10");
//                qrySensor.bindValue(":kwh", "10");
//                qrySensor.bindValue(":suhu_oli", "10");

            if(qrySensor.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }


    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }

}

void MainWindow::insertDurasi(QString durasiName)
{
    if(database.open())
    {
        QSqlQuery qryStatus;


            qryStatus.prepare("INSERT INTO tb_durasi (tmp, status, durasi) VALUES (:tmp, :status, :durasi)");
            qryStatus.bindValue(":tmp", gabTr);
            qryStatus.bindValue(":status", durasiName);
            qryStatus.bindValue(":durasi", gabDur);

            if(qryStatus.exec()){
    //                QMessageBox::information(this, "Status", "Data Inserted Sucessfully");
            }
            else{
                QMessageBox::warning(this, "Not Inserted", "Data is Not Inserted");
            }
    }
    else{
        QMessageBox::warning(this, "Status", "Failed");
    }
}

void MainWindow::parseData(bool dataParse, QString dataParse2)
{
    // Mengonversi JSON string menjadi objek JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
    QJsonObject jsonObj = jsonDoc.object();

    // Mengambil nilai dari objek JSON
    dataParse = jsonObj.value(dataParse2).toArray().at(0).toBool();
    detik = jsonObj.value("detik").toArray().at(0).toInt();
    menit = jsonObj.value("menit").toArray().at(0).toInt();
    jam = jsonObj.value("jam").toArray().at(0).toInt();
    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
    stsMesin = QString::number(dataParse).toLower();

    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);

    qDebug() << "Status:" << dataParse;
    qDebug() << "detik:" << detik;
    qDebug() << "menit:" << menit;
    qDebug() << "jam:" << jam;
    qDebug() << "tanggal:" << tanggal;
    qDebug() << "bulan:" << bulan;
    qDebug() << "tahun:" << tahun;
    qDebug() << "total (detik):" << totalDetik;
    qDebug() << "total (menit):" << totalMenit;
    qDebug() << "total (jam):" << totalJam;
    qDebug() << "Status Sementara" << stsMesin;
    qDebug() << "Gabungan Waktu :" << gabTr;
    qDebug() << "Gabungan Durasi :" << gabDur;
}

void MainWindow::parseSensor()
{
    // Mengonversi JSON string menjadi objek JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
    QJsonObject jsonObj = jsonDoc.object();

    // Mengambil nilai dari objek JSON
    vrs = jsonObj.value("vrs").toArray().at(0).toDouble();
    vst = jsonObj.value("vst").toArray().at(0).toDouble();
    vrt = jsonObj.value("vrt").toArray().at(0).toDouble();
    vrn = jsonObj.value("vrn").toArray().at(0).toDouble();
    vsn = jsonObj.value("vsn").toArray().at(0).toDouble();
    vtn = jsonObj.value("vtn").toArray().at(0).toDouble();

    ir = jsonObj.value("ir").toArray().at(0).toDouble();
    is = jsonObj.value("is").toArray().at(0).toDouble();
    it = jsonObj.value("it").toArray().at(0).toDouble();
    pf = jsonObj.value("pf").toArray().at(0).toDouble();

//    w = jsonObj.value("w").toArray().at(0).toInt();
//    var = jsonObj.value("var").toArray().at(0).toInt();
//    va = jsonObj.value("va").toArray().at(0).toInt();
//    cospi = jsonObj.value("cospi").toArray().at(0).toInt();
//    kwh = jsonObj.value("kwh").toArray().at(0).toInt();
    rpm = jsonObj.value("rpm").toArray().at(0).toInt();
    suhuMotor = jsonObj.value("suhu motor").toArray().at(0).toInt();
//    suhuOli = jsonObj.value("suhu oli").toArray().at(0).toInt();
    detik = jsonObj.value("detik").toArray().at(0).toInt();
    menit = jsonObj.value("menit").toArray().at(0).toInt();
    jam = jsonObj.value("jam").toArray().at(0).toInt();
    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(run).toLower();
//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
    qDebug() << "Vrs:" << vrs;
    qDebug() << "Vst:" << vst;
    qDebug() << "Vst:" << vrt;
    qDebug() << "Vrn:" << vrn;
    qDebug() << "Vsn:" << vsn;
    qDebug() << "Vtn:" << vtn;
    qDebug() << "Ir:" << ir;
    qDebug() << "Is:" << is;
    qDebug() << "It:" << it;
    qDebug() << "Pf:" << pf;
//    qDebug() << "W:" << w;
//    qDebug() << "Var:" <<var;
//    qDebug() << "Va:" <<va;
//    qDebug() << "Cospi:" <<cospi;
//    qDebug() << "Kwh:" <<kwh;
    qDebug() << "rpm:" <<rpm;
    qDebug() << "Suhu Motor:" << suhuMotor;
//    qDebug() << "Suhu Oli:" << suhuOli;
    qDebug() << "detik:" << detik;
    qDebug() << "menit:" << menit;
    qDebug() << "jam:" << jam;
    qDebug() << "tanggal:" << tanggal;
    qDebug() << "bulan:" << bulan;
    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
}

//void MainWindow::parseMtbf()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    mtbf = jsonObj.value("mtbf").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(mtbf).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << mtbf;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}



//void MainWindow::parseRunning()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    run = jsonObj.value("run").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(run).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << run;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;

//}

//void MainWindow::parseBreak()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    breakt = jsonObj.value("break").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(breakt).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << breakt;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;

//}

//void MainWindow::parseQc()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    qc = jsonObj.value("qc").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(qc).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << qc;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseBreakdown()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    breakdown = jsonObj.value("breakdown").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(breakdown).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << breakdown;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseDown()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    down = jsonObj.value("downtime").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(down).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << down;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseGood()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    good = jsonObj.value("goodproduct").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(good).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << good;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseReject()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    reject = jsonObj.value("rejectproduct").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(reject).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);
//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << reject;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseIdle()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    idle = jsonObj.value("idle").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(idle).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);

//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << idle;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;

//}

//void MainWindow::parseSetup()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    setup = jsonObj.value("setup").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(setup).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);

//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << setup;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseLoginOn()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    loginon = jsonObj.value("login").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(loginon).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);


//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << loginon;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parsePowerOn()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    poweron = jsonObj.value("poweron").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(poweron).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);


//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << poweron;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseWorkPiece()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    workpiece = jsonObj.value("workpiece").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();

//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);
//    stsMesin = QString::number(workpiece).toLower();

//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);


//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << workpiece;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}

//void MainWindow::parseTools()
//{
//    // Mengonversi JSON string menjadi objek JSON
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(message2);
//    QJsonObject jsonObj = jsonDoc.object();

//    // Mengambil nilai dari objek JSON
//    tools = jsonObj.value("tools").toArray().at(0).toBool();
//    detik = jsonObj.value("detik").toArray().at(0).toInt();
//    menit = jsonObj.value("menit").toArray().at(0).toInt();
//    jam = jsonObj.value("jam").toArray().at(0).toInt();
//    tanggal = jsonObj.value("tanggal").toArray().at(0).toInt();
//    bulan = jsonObj.value("bulan").toArray().at(0).toInt();
//    tahun = jsonObj.value("tahun").toArray().at(0).toInt();
//    totalDetik = jsonObj.value("total detik").toArray().at(0).toInt();
//    totalMenit = jsonObj.value("total menit").toArray().at(0).toInt();
//    totalJam = jsonObj.value("total jam").toArray().at(0).toInt();
//    gabTr = QString::number(tahun) + "-" + QString::number(bulan) + "-" + QString::number(tanggal) + " " + QString::number(jam) + ":" + QString::number(menit) + ":" + QString::number(detik);

//    stsMesin = QString::number(tools).toLower();
//    gabDur = QString::number(totalJam) + ":" + QString::number(totalMenit) + ":" + QString::number(totalDetik);

//    // Menampilkan nilai yang diambil menggunakan qDebug()
//    qDebug() << "Status:" << tools;
//    qDebug() << "detik:" << detik;
//    qDebug() << "menit:" << menit;
//    qDebug() << "jam:" << jam;
//    qDebug() << "tanggal:" << tanggal;
//    qDebug() << "bulan:" << bulan;
//    qDebug() << "tahun:" << tahun;
//    qDebug() << "total (detik):" << totalDetik;
//    qDebug() << "total (menit):" << totalMenit;
//    qDebug() << "total (jam):" << totalJam;
//    qDebug() << "Gabungan Waktu :" << gabTr;
//    qDebug() << "Gabungan Durasi :" << gabDur;
//}
