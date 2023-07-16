/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata0[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "setClientPort"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 1), // "p"
QT_MOC_LITERAL(4, 28, 11), // "parseResult"
QT_MOC_LITERAL(5, 40, 12), // "connectionDb"
QT_MOC_LITERAL(6, 53, 12), // "insertStatus"
QT_MOC_LITERAL(7, 66, 10), // "statusName"
QT_MOC_LITERAL(8, 77, 14), // "insertFormPlan"
QT_MOC_LITERAL(9, 92, 11), // "insertLogin"
QT_MOC_LITERAL(10, 104, 13), // "insertMachine"
QT_MOC_LITERAL(11, 118, 12), // "insertSensor"
QT_MOC_LITERAL(12, 131, 12), // "insertDurasi"
QT_MOC_LITERAL(13, 144, 10), // "durasiName"
QT_MOC_LITERAL(14, 155, 9), // "parseData"
QT_MOC_LITERAL(15, 165, 9), // "dataParse"
QT_MOC_LITERAL(16, 175, 10), // "dataParse2"
QT_MOC_LITERAL(17, 186, 11), // "parseSensor"
QT_MOC_LITERAL(18, 198, 24), // "on_buttonConnect_clicked"
QT_MOC_LITERAL(19, 223, 21), // "on_buttonQuit_clicked"
QT_MOC_LITERAL(20, 245, 20), // "updateLogStateChange"
QT_MOC_LITERAL(21, 266, 18), // "brokerDisconnected"
QT_MOC_LITERAL(22, 285, 24), // "on_buttonPublish_clicked"
QT_MOC_LITERAL(23, 310, 26) // "on_buttonSubscribe_clicked"

    },
    "MainWindow\0setClientPort\0\0p\0parseResult\0"
    "connectionDb\0insertStatus\0statusName\0"
    "insertFormPlan\0insertLogin\0insertMachine\0"
    "insertSensor\0insertDurasi\0durasiName\0"
    "parseData\0dataParse\0dataParse2\0"
    "parseSensor\0on_buttonConnect_clicked\0"
    "on_buttonQuit_clicked\0updateLogStateChange\0"
    "brokerDisconnected\0on_buttonPublish_clicked\0"
    "on_buttonSubscribe_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x0a /* Public */,
       4,    0,  102,    2, 0x0a /* Public */,
       5,    0,  103,    2, 0x0a /* Public */,
       6,    1,  104,    2, 0x0a /* Public */,
       8,    0,  107,    2, 0x0a /* Public */,
       9,    0,  108,    2, 0x0a /* Public */,
      10,    0,  109,    2, 0x0a /* Public */,
      11,    0,  110,    2, 0x0a /* Public */,
      12,    1,  111,    2, 0x0a /* Public */,
      14,    2,  114,    2, 0x0a /* Public */,
      17,    0,  119,    2, 0x0a /* Public */,
      18,    0,  120,    2, 0x08 /* Private */,
      19,    0,  121,    2, 0x08 /* Private */,
      20,    0,  122,    2, 0x08 /* Private */,
      21,    0,  123,    2, 0x08 /* Private */,
      22,    0,  124,    2, 0x08 /* Private */,
      23,    0,  125,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setClientPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->parseResult(); break;
        case 2: _t->connectionDb(); break;
        case 3: _t->insertStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->insertFormPlan(); break;
        case 5: _t->insertLogin(); break;
        case 6: _t->insertMachine(); break;
        case 7: _t->insertSensor(); break;
        case 8: _t->insertDurasi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->parseData((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->parseSensor(); break;
        case 11: _t->on_buttonConnect_clicked(); break;
        case 12: _t->on_buttonQuit_clicked(); break;
        case 13: _t->updateLogStateChange(); break;
        case 14: _t->brokerDisconnected(); break;
        case 15: _t->on_buttonPublish_clicked(); break;
        case 16: _t->on_buttonSubscribe_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
