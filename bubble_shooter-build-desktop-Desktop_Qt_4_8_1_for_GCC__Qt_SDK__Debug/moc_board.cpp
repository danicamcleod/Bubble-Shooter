/****************************************************************************
** Meta object code from reading C++ file 'board.h'
**
** Created: Tue Dec 4 09:54:12 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bubble_shooter/board.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'board.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Board[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      21,    6,    6,    6, 0x0a,
      30,    6,    6,    6, 0x0a,
      37,    6,    6,    6, 0x0a,
      46,    6,    6,    6, 0x0a,
      55,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Board[] = {
    "Board\0\0restartGame()\0topTen()\0help()\0"
    "novice()\0expert()\0master()\0"
};

void Board::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Board *_t = static_cast<Board *>(_o);
        switch (_id) {
        case 0: _t->restartGame(); break;
        case 1: _t->topTen(); break;
        case 2: _t->help(); break;
        case 3: _t->novice(); break;
        case 4: _t->expert(); break;
        case 5: _t->master(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Board::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Board::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Board,
      qt_meta_data_Board, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Board::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Board::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Board::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Board))
        return static_cast<void*>(const_cast< Board*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Board*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Board*>(this));
    return QObject::qt_metacast(_clname);
}

int Board::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
