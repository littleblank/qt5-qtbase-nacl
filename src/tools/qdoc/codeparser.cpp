/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
  codeparser.cpp
*/

#include "codeparser.h"
#include "node.h"
#include "tree.h"
#include "config.h"
#include "generator.h"
#include "qdocdatabase.h"
#include <qdebug.h>

QT_BEGIN_NAMESPACE

#define COMMAND_COMPAT                  Doc::alias(QLatin1String("compat"))
#define COMMAND_DEPRECATED              Doc::alias(QLatin1String("deprecated")) // ### don't document
#define COMMAND_INGROUP                 Doc::alias(QLatin1String("ingroup"))
#define COMMAND_INMODULE                Doc::alias(QLatin1String("inmodule"))  // ### don't document
#define COMMAND_INQMLMODULE             Doc::alias(QLatin1String("inqmlmodule"))
#define COMMAND_INTERNAL                Doc::alias(QLatin1String("internal"))
#define COMMAND_MAINCLASS               Doc::alias(QLatin1String("mainclass"))
#define COMMAND_NONREENTRANT            Doc::alias(QLatin1String("nonreentrant"))
#define COMMAND_OBSOLETE                Doc::alias(QLatin1String("obsolete"))
#define COMMAND_PAGEKEYWORDS            Doc::alias(QLatin1String("pagekeywords"))
#define COMMAND_PRELIMINARY             Doc::alias(QLatin1String("preliminary"))
#define COMMAND_INPUBLICGROUP           Doc::alias(QLatin1String("inpublicgroup"))
#define COMMAND_REENTRANT               Doc::alias(QLatin1String("reentrant"))
#define COMMAND_SINCE                   Doc::alias(QLatin1String("since"))
#define COMMAND_SUBTITLE                Doc::alias(QLatin1String("subtitle"))
#define COMMAND_THREADSAFE              Doc::alias(QLatin1String("threadsafe"))
#define COMMAND_TITLE                   Doc::alias(QLatin1String("title"))

QString CodeParser::currentSubDir_;
QList<CodeParser *> CodeParser::parsers;
bool CodeParser::showInternal = false;
QMap<QString,QString> CodeParser::nameToTitle;

/*!
  The constructor adds this code parser to the static
  list of code parsers.
 */
CodeParser::CodeParser()
{
    qdb_ = QDocDatabase::qdocDB();
    parsers.prepend(this);
}

/*!
  The destructor removes this code parser from the static
  list of code parsers.
 */
CodeParser::~CodeParser()
{
    parsers.removeAll(this);
}

/*!
  Initialize the code parser base class.
 */
void CodeParser::initializeParser(const Config& config)
{
    showInternal = config.getBool(QLatin1String(CONFIG_SHOWINTERNAL));
}

/*!
  Terminating a code parser is trivial.
 */
void CodeParser::terminateParser()
{
    // nothing.
}

QStringList CodeParser::headerFileNameFilter()
{
    return sourceFileNameFilter();
}

void CodeParser::parseHeaderFile(const Location& location, const QString& filePath)
{
    parseSourceFile(location, filePath);
}

void CodeParser::doneParsingHeaderFiles()
{
    doneParsingSourceFiles();
}

/*!
  All the code parsers in the static list are initialized here,
  after the qdoc configuration variables have been set.
 */
void CodeParser::initialize(const Config& config)
{
    QList<CodeParser *>::ConstIterator p = parsers.constBegin();
    while (p != parsers.constEnd()) {
        (*p)->initializeParser(config);
        ++p;
    }
}

/*!
  All the code parsers in the static list are terminated here.
 */
void CodeParser::terminate()
{
    QList<CodeParser *>::ConstIterator p = parsers.constBegin();
    while (p != parsers.constEnd()) {
        (*p)->terminateParser();
        ++p;
    }
}

CodeParser *CodeParser::parserForLanguage(const QString& language)
{
    QList<CodeParser *>::ConstIterator p = parsers.constBegin();
    while (p != parsers.constEnd()) {
        if ((*p)->language() == language)
            return *p;
        ++p;
    }
    return 0;
}

CodeParser *CodeParser::parserForHeaderFile(const QString &filePath)
{
    QString fileName = QFileInfo(filePath).fileName();

    QList<CodeParser *>::ConstIterator p = parsers.constBegin();
    while (p != parsers.constEnd()) {

        QStringList headerPatterns = (*p)->headerFileNameFilter();
        foreach (const QString &pattern, headerPatterns) {
            QRegExp re(pattern, Qt::CaseInsensitive, QRegExp::Wildcard);
            if (re.exactMatch(fileName))
                return *p;
        }
        ++p;
    }
    return 0;
}

CodeParser *CodeParser::parserForSourceFile(const QString &filePath)
{
    QString fileName = QFileInfo(filePath).fileName();

    QList<CodeParser *>::ConstIterator p = parsers.constBegin();
    while (p != parsers.constEnd()) {

        QStringList sourcePatterns = (*p)->sourceFileNameFilter();
        foreach (const QString &pattern, sourcePatterns) {
            QRegExp re(pattern, Qt::CaseInsensitive, QRegExp::Wildcard);
            if (re.exactMatch(fileName))
                return *p;
        }
        ++p;
    }
    return 0;
}

/*!
  Returns the set of strings representing the common metacommands.
 */
QSet<QString> CodeParser::commonMetaCommands()
{
    return QSet<QString>() << COMMAND_COMPAT
                           << COMMAND_DEPRECATED
                           << COMMAND_INGROUP
                           << COMMAND_INMODULE
                           << COMMAND_INQMLMODULE
                           << COMMAND_INTERNAL
                           << COMMAND_MAINCLASS
                           << COMMAND_NONREENTRANT
                           << COMMAND_OBSOLETE
                           << COMMAND_PAGEKEYWORDS
                           << COMMAND_PRELIMINARY
                           << COMMAND_INPUBLICGROUP
                           << COMMAND_REENTRANT
                           << COMMAND_SINCE
                           << COMMAND_SUBTITLE
                           << COMMAND_THREADSAFE
                           << COMMAND_TITLE;
}

/*!
  The topic command has been processed. Now process the other
  metacommands that were found. These are not the text markup
  commands.
 */
void CodeParser::processCommonMetaCommand(const Location& location,
                                          const QString& command,
                                          const ArgLocPair& arg,
                                          Node* node)
{
    if (command == COMMAND_COMPAT) {
        location.warning(tr("\\compat command used, but Qt3 compatibility is no longer supported"));
        node->setStatus(Node::Compat);
    }
    else if (command == COMMAND_DEPRECATED) {
        node->setStatus(Node::Deprecated);
    }
    else if (command == COMMAND_INGROUP) {
        qdb_->addToGroup(node, arg.first);
    }
    else if (command == COMMAND_INPUBLICGROUP) {
        qdb_->addToPublicGroup(node, arg.first);
    }
    else if (command == COMMAND_INMODULE) {
        qdb_->addToModule(arg.first,node);
    }
    else if (command == COMMAND_INQMLMODULE) {
        qdb_->addToQmlModule(arg.first,node);
    }
    else if (command == COMMAND_MAINCLASS) {
        node->setStatus(Node::Main);
    }
    else if (command == COMMAND_OBSOLETE) {
        if (node->status() != Node::Compat)
            node->setStatus(Node::Obsolete);
    }
    else if (command == COMMAND_NONREENTRANT) {
        node->setThreadSafeness(Node::NonReentrant);
    }
    else if (command == COMMAND_PRELIMINARY) {
        node->setStatus(Node::Preliminary);
    }
    else if (command == COMMAND_INTERNAL) {
        if (!showInternal) {
            node->setAccess(Node::Private);
            node->setStatus(Node::Internal);
            if (node->subType() == Node::QmlPropertyGroup) {
                const QmlPropGroupNode* qpgn = static_cast<const QmlPropGroupNode*>(node);
                NodeList::ConstIterator p = qpgn->childNodes().constBegin();
                while (p != qpgn->childNodes().constEnd()) {
                    if ((*p)->type() == Node::QmlProperty) {
                        (*p)->setAccess(Node::Private);
                        (*p)->setStatus(Node::Internal);
                    }
                    ++p;
                }
            }
        }
    }
    else if (command == COMMAND_REENTRANT) {
        node->setThreadSafeness(Node::Reentrant);
    }
    else if (command == COMMAND_SINCE) {
        node->setSince(arg.first);
    }
    else if (command == COMMAND_PAGEKEYWORDS) {
        node->addPageKeywords(arg.first);
    }
    else if (command == COMMAND_SUBTITLE) {
        if (node->type() == Node::Document) {
            DocNode *dn = static_cast<DocNode *>(node);
            dn->setSubTitle(arg.first);
        }
        else
            location.warning(tr("Ignored '\\%1'").arg(COMMAND_SUBTITLE));
    }
    else if (command == COMMAND_THREADSAFE) {
        node->setThreadSafeness(Node::ThreadSafe);
    }
    else if (command == COMMAND_TITLE) {
        if (node->type() == Node::Document) {
            DocNode *dn = static_cast<DocNode *>(node);
            dn->setTitle(arg.first);
            if (dn->subType() == Node::Example) {
                ExampleNode::exampleNodeMap.insert(dn->title(),static_cast<ExampleNode*>(dn));
            }
            nameToTitle.insert(dn->name(),arg.first);
        }
        else
            location.warning(tr("Ignored '\\%1'").arg(COMMAND_TITLE));
    }
}

/*!
  Find the page title given the page \a name and return it.
 */
const QString CodeParser::titleFromName(const QString& name)
{
    const QString t = nameToTitle.value(name);
    return t;
}

/*!
  \internal
 */
void CodeParser::extractPageLinkAndDesc(const QString& arg,
                                        QString* link,
                                        QString* desc)
{
    QRegExp bracedRegExp(QLatin1String("\\{([^{}]*)\\}(?:\\{([^{}]*)\\})?"));

    if (bracedRegExp.exactMatch(arg)) {
        *link = bracedRegExp.cap(1);
        *desc = bracedRegExp.cap(2);
        if (desc->isEmpty())
            *desc = *link;
    }
    else {
        int spaceAt = arg.indexOf(QLatin1Char(' '));
        if (arg.contains(QLatin1String(".html")) && spaceAt != -1) {
            *link = arg.left(spaceAt).trimmed();
            *desc = arg.mid(spaceAt).trimmed();
        }
        else {
            *link = arg;
            *desc = arg;
        }
    }
}

/*!
  \internal
 */
void CodeParser::setLink(Node* node, Node::LinkType linkType, const QString& arg)
{
    QString link;
    QString desc;
    extractPageLinkAndDesc(arg, &link, &desc);
    node->setLink(linkType, link, desc);
}

/*!
  Returns true if the file being parsed is a .h file.
 */
bool CodeParser::isParsingH() const
{
    return currentFile_.endsWith(".h");
}

/*!
  Returns true if the file being parsed is a .cpp file.
 */
bool CodeParser::isParsingCpp() const
{
    return currentFile_.endsWith(".cpp");
}

/*!
  Returns true if the file being parsed is a .qdoc file.
 */
bool CodeParser::isParsingQdoc() const
{
    return currentFile_.endsWith(".qdoc");
}

QT_END_NAMESPACE
