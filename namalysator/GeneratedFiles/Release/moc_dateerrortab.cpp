/****************************************************************************
** Meta object code from reading C++ file 'dateerrortab.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dateerrortab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dateerrortab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dateerrortab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   14,   13,   13, 0x08,
      51,   46,   13,   13, 0x08,
      78,   70,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dateerrortab[] = {
    "dateerrortab\0\0category\0getcbCategory(QString)\0"
    "year\0getcbYear(QString)\0col,row\0"
    "getLine(int,int)\0"
};

void dateerrortab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dateerrortab *_t = static_cast<dateerrortab *>(_o);
        switch (_id) {
        case 0: _t->getcbCategory((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->getcbYear((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->getLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dateerrortab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dateerrortab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_dateerrortab,
      qt_meta_data_dateerrortab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dateerrortab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dateerrortab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dateerrortab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dateerrortab))
        return static_cast<void*>(const_cast< dateerrortab*>(this));
    return QWidget::qt_metacast(_clname);
}

int dateerrortab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
