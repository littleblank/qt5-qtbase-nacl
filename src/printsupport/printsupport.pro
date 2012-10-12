load(qt_build_config)

TARGET     = QtPrintSupport
QT = core-private gui-private widgets-private

DEFINES   += QT_NO_USING_NAMESPACE

QMAKE_DOCS = $$PWD/doc/qtprintsupport.qdocconf

load(qt_module)

QMAKE_LIBS += $$QMAKE_LIBS_PRINTSUPPORT

!nacl {
    include(kernel/kernel.pri)
    include(widgets/widgets.pri)
    include(dialogs/dialogs.pri)
}
