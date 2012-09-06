/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QPEPPERCOMPOSITOR_H
#define QPEPPERCOMPOSITOR_H

#include <QtGui>
#include <QtCore>
#include "pepperinstance.h"

class QPepperCompositedWindow
{
public:
    QPepperCompositedWindow();

    QPlatformWindow *window;
    QImage *frameBuffer;
    QPlatformWindow *parentWindow;
    QRect damage;
    bool flushPending;
    bool visible;
    QList<QWindow *> childWindows;
};

class QPepperCompositor : public QObject
{
Q_OBJECT
public:
    QPepperCompositor();
// Client API
    void addRasterWindow(QPlatformWindow *window, QPlatformWindow *parentWindow = 0);
    void removeWindow(QPlatformWindow *window);

    void setVisible(QPlatformWindow *window, bool visible);
    void raise(QPlatformWindow *window);
    void lower(QPlatformWindow *window);

    void setFrameBuffer(QPlatformWindow *window, QImage *frameBuffer);
    void flush(QPlatformWindow *surface);
    void waitForFlushed(QPlatformWindow *surface);

// Server API
    void setRasterFrameBuffer(QImage *frameBuffer); // call when the frame buffer geometry changes
Q_SIGNALS:
    void flush();                                   // emitted when the server should flush the frame buffer
public Q_SLOTS:
    void flushCompleted();                          // call when the frame buffer flush has completed.

// Misc API
    QWindow *windowAt(QPoint p);
    QWindow *keyWindow();
    void maybeComposit();
    void composit();
private:
    QHash<QWindow *, QPepperCompositedWindow> m_compositedWindows;
    QList<QWindow *> m_windowStack;
    QImage *m_frameBuffer;
    bool m_needComposit;
};

#endif
