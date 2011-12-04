/****************************************************************************
** Meta object code from reading C++ file 'camerawidget.h'
**
** Created: Mon Dec 5 01:09:02 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../camerawidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerawidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CameraWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x09,
      29,   13,   13,   13, 0x09,
      45,   13,   13,   13, 0x09,
      67,   13,   13,   13, 0x09,
      94,   80,   13,   13, 0x09,
     159,   13,   13,   13, 0x09,
     168,   13,   13,   13, 0x09,
     184,  182,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CameraWidget[] = {
    "CameraWidget\0\0enableCamera()\0"
    "searchAndLock()\0error(QCamera::Error)\0"
    "lockFailed()\0status,reason\0"
    "lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)\0"
    "locked()\0updateVideo()\0,\0"
    "imageCaptured(int,QImage)\0"
};

const QMetaObject CameraWidget::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CameraWidget,
      qt_meta_data_CameraWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CameraWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CameraWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CameraWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CameraWidget))
        return static_cast<void*>(const_cast< CameraWidget*>(this));
    if (!strcmp(_clname, "VideoIF"))
        return static_cast< VideoIF*>(const_cast< CameraWidget*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CameraWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: enableCamera(); break;
        case 1: searchAndLock(); break;
        case 2: error((*reinterpret_cast< QCamera::Error(*)>(_a[1]))); break;
        case 3: lockFailed(); break;
        case 4: lockStatusChanged((*reinterpret_cast< QCamera::LockStatus(*)>(_a[1])),(*reinterpret_cast< QCamera::LockChangeReason(*)>(_a[2]))); break;
        case 5: locked(); break;
        case 6: updateVideo(); break;
        case 7: imageCaptured((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
