/****************************************************************************
** Meta object code from reading C++ file 'cmodbus.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cmodbus.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cmodbus.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CModbus_t {
    QByteArrayData data[20];
    char stringdata0[221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CModbus_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CModbus_t qt_meta_stringdata_CModbus = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CModbus"
QT_MOC_LITERAL(1, 8, 11), // "errorDevice"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 18), // "connectDeviceState"
QT_MOC_LITERAL(4, 40, 9), // "dataReady"
QT_MOC_LITERAL(5, 50, 14), // "CDataUnitType&"
QT_MOC_LITERAL(6, 65, 15), // "timeoutResponse"
QT_MOC_LITERAL(7, 81, 7), // "rawData"
QT_MOC_LITERAL(8, 89, 11), // "QByteArray&"
QT_MOC_LITERAL(9, 101, 4), // "data"
QT_MOC_LITERAL(10, 106, 7), // "infoLog"
QT_MOC_LITERAL(11, 114, 13), // "connectDevice"
QT_MOC_LITERAL(12, 128, 16), // "disconnectDevice"
QT_MOC_LITERAL(13, 145, 7), // "request"
QT_MOC_LITERAL(14, 153, 4), // "unit"
QT_MOC_LITERAL(15, 158, 9), // "readyRead"
QT_MOC_LITERAL(16, 168, 9), // "errorPort"
QT_MOC_LITERAL(17, 178, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(18, 207, 5), // "error"
QT_MOC_LITERAL(19, 213, 7) // "timeout"

    },
    "CModbus\0errorDevice\0\0connectDeviceState\0"
    "dataReady\0CDataUnitType&\0timeoutResponse\0"
    "rawData\0QByteArray&\0data\0infoLog\0"
    "connectDevice\0disconnectDevice\0request\0"
    "unit\0readyRead\0errorPort\0"
    "QSerialPort::SerialPortError\0error\0"
    "timeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CModbus[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       3,    1,   82,    2, 0x06 /* Public */,
       4,    1,   85,    2, 0x06 /* Public */,
       6,    0,   88,    2, 0x06 /* Public */,
       7,    2,   89,    2, 0x06 /* Public */,
       7,    1,   94,    2, 0x26 /* Public | MethodCloned */,
      10,    1,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  100,    2, 0x0a /* Public */,
      12,    0,  101,    2, 0x0a /* Public */,
      13,    1,  102,    2, 0x0a /* Public */,
      15,    0,  105,    2, 0x0a /* Public */,
      16,    1,  106,    2, 0x0a /* Public */,
      19,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Bool,    9,    2,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,

       0        // eod
};

void CModbus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CModbus *_t = static_cast<CModbus *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorDevice((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->connectDeviceState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->dataReady((*reinterpret_cast< CDataUnitType(*)>(_a[1]))); break;
        case 3: _t->timeoutResponse(); break;
        case 4: _t->rawData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->rawData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->infoLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->connectDevice(); break;
        case 8: _t->disconnectDevice(); break;
        case 9: _t->request((*reinterpret_cast< CDataUnitType(*)>(_a[1]))); break;
        case 10: _t->readyRead(); break;
        case 11: _t->errorPort((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 12: _t->timeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CModbus::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::errorDevice)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CModbus::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::connectDeviceState)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CModbus::*_t)(CDataUnitType & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::dataReady)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CModbus::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::timeoutResponse)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CModbus::*_t)(QByteArray & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::rawData)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CModbus::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CModbus::infoLog)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject CModbus::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CModbus.data,
      qt_meta_data_CModbus,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CModbus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CModbus::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CModbus.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CModbus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CModbus::errorDevice(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CModbus::connectDeviceState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CModbus::dataReady(CDataUnitType & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CModbus::timeoutResponse()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CModbus::rawData(QByteArray & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void CModbus::infoLog(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
