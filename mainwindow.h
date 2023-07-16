#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMqttClient>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include "config.h"
#include <QNetworkAccessManager>
#include <inttypes.h>
#include <stdio.h>


namespace Ui {
class MainWindow;
}

struct mydatas{
    QString ts;
    QString formattedTime;
    QString did;
    QString value;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setClientPort(int p);
    void parseResult();
    void connectionDb();
    void insertStatus(QString statusName);
//    void insertPowerOn(int value);
//    void insertIdle();

//    void insertEnergy();
    void insertFormPlan();
    void insertLogin();
    void insertMachine();
    void insertSensor();
    void insertDurasi(QString durasiName);
    void parseData(bool dataParse, QString dataParse2);
    void parseSensor();

//    void parsePowerOn();
//    void parseMtbf();
//    void parseIdle();
//    void parseSetup();
//    void parseLoginOn();
//    void parseRunning();
//    void parseBreak();
//    void parseQc();
//    void parseBreakdown();
//    void parseDown();
//    void parseGood();
//    void parseReject();
//    void parseWorkPiece();
//    void parseTools();

private slots:
    void on_buttonConnect_clicked();
    void on_buttonQuit_clicked();
    void updateLogStateChange();
    void brokerDisconnected();
    void on_buttonPublish_clicked();
    void on_buttonSubscribe_clicked();

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
    QSqlDatabase database;
    QNetworkAccessManager *networkManager;

public:
    int data = 0;
    int press = 0;
    int temp = 0;
    mydatas mydataf[10];
    QString thnStr;
    QString blnStr;
    QString tglStr;
    QString jamStr;
    QString mntStr;
    QString dtkStr;
    QString stsStr;
    QString gabTr;
    QString gabDur;
    QString stsMesin;

    int totalrowReceived;
    QString dataId;
    QString dataValue;
    QString ts;
    QString tsInst;
    QString leftData;
    QString midData;
    QString rightData;

    // Topic Name
    QString powerontime = "powerontime";
    QString mtbftime = "mtbf";
    QString idletime = "idletime";
    QString setuptime = "setuptime";
    QString loginontime = "loginontime";
    QString runtime = "runtime";
    QString breaktime = "breaktime";
    QString qctime = "qctime";
    QString breakdowntime = "breakdowntime";
    QString downtime = "downtime";
    QString goodproduct = "goodproduct";
    QString rejectproduct = "rejectproduct";
    QString workpiecetime = "workpiecetime";
    QString toolstime = "toolstime";

    QString workorderid = "workorderid";
    QString operatorid = "operatorid";
    QString jobid = "jobid";
    QString qcid = "qcid";
    QString stdtime = "stdtime";
    QString jobname = "jobname";
    QString timeplan = "timeplan";

    QString allsensor = "allsensor";

    time_t rawtime;
    QByteArray message2;
    QNetworkReply *reply;
    QDateTime *timestamp;

    // Mengambil nilai dari objek JSON
    bool poweron;
    bool mtbf;
    bool idle;
    bool setup;
    bool loginon;
    bool run;
    bool breakt;
    bool qc;
    bool breakdown;
    bool down;
    bool good;
    bool reject;
    bool workpiece;
    bool tools;

    int detik;
    int menit;
    int jam;
    int tanggal;
    int bulan;
    int tahun;
    int totalDetik;
    int totalMenit;
    int totalJam;
    int vrs;
    int vrt;
    int vst;
    int vrn;
    int vsn;
    int vtn;
    int ir;
    int is;
    int it;
    int pr;
    int ps;
    int pt;
    int w;
    int qr;
    int qs;
    int qt;
    int var;
    int sr;
    int ss;
    int st;
    int va;
    int pfr;
    int pfs;
    int pft;
    int pf;
    int wpp;
    int wpn;
    int wqp;
    int wqn;
    int epp;
    int epn;
    int eqp;
    int eqn;
    int rpm;
    int suhuMotor;
    int suhuOli;




    int64_t S64(const char *s);


};

#endif // MAINWINDOW_H
