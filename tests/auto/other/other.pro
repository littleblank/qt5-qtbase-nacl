TEMPLATE=subdirs
SUBDIRS=\
   # atwrapper \ # QTBUG-19452
   baselineexample \
   collections \
   compiler \
   exceptionsafety \
   # exceptionsafety_objects \    # QObjectPrivate is not safe
   gestures \
   headersclean \
   lancelot \
   languagechange \
   macgui \
   macnativeevents \
   macplist \
   modeltest \
   networkselftest \
   qaccessibility \
   qaccessibilitylinux \
   qcomplextext \
   qfocusevent \
   qnetworkaccessmanager_and_qprogressdialog \
   qobjectperformance \
   qobjectrace \
   qsharedpointer_and_qwidget \
   qtokenautomaton \
   windowsmobile \

contains(QT_CONFIG, no-widgets): SUBDIRS -= \
   baselineexample \
   gestures \
   headersclean \
   lancelot \
   languagechange \
   modeltest \
   qaccessibility \
   qcomplextext \
   qfocusevent \
   qnetworkaccessmanager_and_qprogressdialog \
   qsharedpointer_and_qwidget \
   windowsmobile \

testcocoon: SUBDIRS -= headersclean

cross_compile: SUBDIRS -= \
   atwrapper \
   compiler

wince*|!contains(QT_CONFIG, accessibility): SUBDIRS -= qaccessibility

!contains(QT_CONFIG, accessibility)|!contains(QT_CONFIG, xcb): SUBDIRS -= qaccessibilitylinux

!mac: SUBDIRS -= \
           macgui \
           macnativeevents \
           macplist

!embedded|wince*: SUBDIRS -= \
           qdirectpainter

!linux*-g++*:SUBDIRS -= exceptionsafety_objects
