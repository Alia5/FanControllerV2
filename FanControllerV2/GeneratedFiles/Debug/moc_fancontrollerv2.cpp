/****************************************************************************
** Meta object code from reading C++ file 'fancontrollerv2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fancontrollerv2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fancontrollerv2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FanControllerV2_t {
    QByteArrayData data[25];
    char stringdata[341];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FanControllerV2_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FanControllerV2_t qt_meta_stringdata_FanControllerV2 = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FanControllerV2"
QT_MOC_LITERAL(1, 16, 13), // "iconActivated"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(4, 65, 6), // "reason"
QT_MOC_LITERAL(5, 72, 12), // "programClose"
QT_MOC_LITERAL(6, 85, 11), // "restoreSize"
QT_MOC_LITERAL(7, 97, 31), // "on_hS_Transparency_valueChanged"
QT_MOC_LITERAL(8, 129, 16), // "SliderValChanged"
QT_MOC_LITERAL(9, 146, 5), // "value"
QT_MOC_LITERAL(10, 152, 15), // "AutoGraphUpdate"
QT_MOC_LITERAL(11, 168, 12), // "QMouseEvent*"
QT_MOC_LITERAL(12, 181, 1), // "e"
QT_MOC_LITERAL(13, 183, 6), // "update"
QT_MOC_LITERAL(14, 190, 14), // "updatesettings"
QT_MOC_LITERAL(15, 205, 24), // "on_tW_Tabs_tabBarClicked"
QT_MOC_LITERAL(16, 230, 1), // "n"
QT_MOC_LITERAL(17, 232, 16), // "startCalibration"
QT_MOC_LITERAL(18, 249, 16), // "resetCalibration"
QT_MOC_LITERAL(19, 266, 17), // "manualCalibration"
QT_MOC_LITERAL(20, 284, 12), // "calibrateFan"
QT_MOC_LITERAL(21, 297, 16), // "downloadFinished"
QT_MOC_LITERAL(22, 314, 14), // "QNetworkReply*"
QT_MOC_LITERAL(23, 329, 5), // "reply"
QT_MOC_LITERAL(24, 335, 5) // "about"

    },
    "FanControllerV2\0iconActivated\0\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0programClose\0restoreSize\0"
    "on_hS_Transparency_valueChanged\0"
    "SliderValChanged\0value\0AutoGraphUpdate\0"
    "QMouseEvent*\0e\0update\0updatesettings\0"
    "on_tW_Tabs_tabBarClicked\0n\0startCalibration\0"
    "resetCalibration\0manualCalibration\0"
    "calibrateFan\0downloadFinished\0"
    "QNetworkReply*\0reply\0about"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FanControllerV2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    1,   95,    2, 0x08 /* Private */,
      10,    1,   98,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    1,  103,    2, 0x08 /* Private */,
      17,    0,  106,    2, 0x08 /* Private */,
      18,    0,  107,    2, 0x08 /* Private */,
      19,    0,  108,    2, 0x08 /* Private */,
      20,    0,  109,    2, 0x08 /* Private */,
      21,    1,  110,    2, 0x08 /* Private */,
      24,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,

       0        // eod
};

void FanControllerV2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FanControllerV2 *_t = static_cast<FanControllerV2 *>(_o);
        switch (_id) {
        case 0: _t->iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 1: _t->programClose(); break;
        case 2: _t->restoreSize(); break;
        case 3: _t->on_hS_Transparency_valueChanged(); break;
        case 4: _t->SliderValChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->AutoGraphUpdate((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->update(); break;
        case 7: _t->updatesettings(); break;
        case 8: _t->on_tW_Tabs_tabBarClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->startCalibration(); break;
        case 10: _t->resetCalibration(); break;
        case 11: _t->manualCalibration(); break;
        case 12: _t->calibrateFan(); break;
        case 13: _t->downloadFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 14: _t->about(); break;
        default: ;
        }
    }
}

const QMetaObject FanControllerV2::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FanControllerV2.data,
      qt_meta_data_FanControllerV2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FanControllerV2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FanControllerV2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FanControllerV2.stringdata))
        return static_cast<void*>(const_cast< FanControllerV2*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FanControllerV2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
