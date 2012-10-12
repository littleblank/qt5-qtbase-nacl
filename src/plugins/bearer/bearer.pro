TEMPLATE = subdirs

linux*:contains(QT_CONFIG, dbus) {
    SUBDIRS += generic
    SUBDIRS += connman networkmanager
}

#win32:SUBDIRS += nla
win32:SUBDIRS += generic
blackberry:SUBDIRS += blackberry
win32:!wince*:SUBDIRS += nativewifi
macx:contains(QT_CONFIG, corewlan):SUBDIRS += corewlan
macx:SUBDIRS += generic

!nacl: isEmpty(SUBDIRS):SUBDIRS = generic
