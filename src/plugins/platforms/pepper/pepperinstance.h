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

#ifndef QT_PEPPER_INSTANCE_H
#define QT_PEPPER_INSTANCE_H

#include "peppereventtranslator.h"
#include "qpepperhelpers.h"

#include <ppapi/cpp/instance.h>
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"
#include "ppapi/cpp/graphics_2d.h"
#include "ppapi/cpp/image_data.h"
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/utility/completion_callback_factory.h>

class QPepperIntegration;
class QPepperInstance : public pp::Instance
{
public:
    QPepperInstance(PP_Instance instance);
    virtual ~QPepperInstance();

    // pp::Instance virtuals:
    virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
    virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip);
    virtual void DidChangeFocus(bool has_focus);
    virtual bool HandleInputEvent(const pp::InputEvent& event);
    virtual bool HandleDocumentLoad(const pp::URLLoader& url_loader);
    virtual void HandleMessage(const pp::Var& message);

    // Interface for QPepperCompositor:
    void flush();
    void waitForFlushed();

    // privates:
    void flushFunction(int32_t);
    void flushCompletedCallback(int32_t);
    void setupTestGraphics(pp::Size newSize);

    QPepperIntegration *m_pepperIntegraton;

    pp::Var m_console;
    pp::Rect m_currentGeometry;

    pp::Graphics2D *m_context2D;
    pp::ImageData *m_imageData2D;
    QImage *m_frameBuffer;
    bool m_inFlush;
    qint32 m_windowId;
    pp::CompletionCallbackFactory<QPepperInstance, ThreadSafeRefCount> m_callbackFactory;
};

#endif
