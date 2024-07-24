/****************************************************************************
** Meta object code from reading C++ file 'ModifyElementDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ModifyElementDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModifyElementDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModifyElementDialog_t {
    QByteArrayData data[10];
    char stringdata0[250];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModifyElementDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModifyElementDialog_t qt_meta_stringdata_ModifyElementDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ModifyElementDialog"
QT_MOC_LITERAL(1, 20, 26), // "on_pushButton_Save_clicked"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 27), // "on_pushButton_Reset_clicked"
QT_MOC_LITERAL(4, 76, 33), // "on_pushButton_ChangeColor_cli..."
QT_MOC_LITERAL(5, 110, 51), // "on_comboBox_AllSelectedElemen..."
QT_MOC_LITERAL(6, 162, 5), // "index"
QT_MOC_LITERAL(7, 168, 31), // "on_pushButton_ModifyAll_clicked"
QT_MOC_LITERAL(8, 200, 35), // "change_pushButton_ChangeColor..."
QT_MOC_LITERAL(9, 236, 13) // "current_color"

    },
    "ModifyElementDialog\0on_pushButton_Save_clicked\0"
    "\0on_pushButton_Reset_clicked\0"
    "on_pushButton_ChangeColor_clicked\0"
    "on_comboBox_AllSelectedElements_currentIndexChanged\0"
    "index\0on_pushButton_ModifyAll_clicked\0"
    "change_pushButton_ChangeColor_color\0"
    "current_color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModifyElementDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    1,   47,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       8,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    9,

       0        // eod
};

void ModifyElementDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModifyElementDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_Save_clicked(); break;
        case 1: _t->on_pushButton_Reset_clicked(); break;
        case 2: _t->on_pushButton_ChangeColor_clicked(); break;
        case 3: _t->on_comboBox_AllSelectedElements_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_pushButton_ModifyAll_clicked(); break;
        case 5: _t->change_pushButton_ChangeColor_color((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModifyElementDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ModifyElementDialog.data,
    qt_meta_data_ModifyElementDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModifyElementDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModifyElementDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModifyElementDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ModifyElementDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
