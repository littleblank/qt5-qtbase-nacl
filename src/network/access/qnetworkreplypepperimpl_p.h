/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QNETWORKREPLYPEPPERIMPL_H
#define QNETWORKREPLYPEPPERIMPL_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "qnetworkaccessmanager.h"
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/qloggingcategory.h>

#include <qplatformdefs.h>

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/cpp/url_response_info.h"
#include "ppapi/utility/completion_callback_factory.h"

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_PLATFORM_PEPPER_NETWORK)

class QNetworkReplyPepperImplPrivate;
class QNetworkReplyPepperImpl: public QNetworkReply
{
    Q_OBJECT
public:
    QNetworkReplyPepperImpl(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op);
    ~QNetworkReplyPepperImpl();

    // pepper url loader data callback and driver functions
    void onOpen(int32_t result);
    void read();
    void onRead(int32_t result);
    void appendData(int32_t size);
    void fail();

    virtual void abort();

    // reimplemented from QNetworkReply
    virtual void close();
    virtual qint64 bytesAvailable() const;
    virtual bool isSequential () const;
    qint64 size() const;

    virtual qint64 readData(char *data, qint64 maxlen);

    Q_DECLARE_PRIVATE(QNetworkReplyPepperImpl)
private:
    pp::Instance* m_instance;
    pp::URLRequestInfo m_urlRequest;
    pp::URLLoader m_urlLoader;
    pp::CompletionCallbackFactory<QNetworkReplyPepperImpl> callbackFactory;
};

class QNetworkReplyPepperImplPrivate: public QNetworkReplyPrivate
{
public:
    QNetworkReplyPepperImplPrivate(QNetworkReplyPepperImpl * q);

    QByteArray buffer;
    quint64 readOffset;
    quint64 writeOffset;
    int state; // 0:(PP_OK), PP_ERROR_FILENOTFOUND

    Q_DECLARE_PUBLIC(QNetworkReplyPepperImpl)
};

QT_END_NAMESPACE

#endif // QNETWORKREPLYPEPPERIMPL_H
