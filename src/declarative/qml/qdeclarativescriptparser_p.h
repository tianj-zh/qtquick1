/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQuick1 module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QDECLARATIVESCRIPTPARSER_P_H
#define QDECLARATIVESCRIPTPARSER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarativeerror.h"
#include "private/qdeclarativeparser_p.h"

#include <QtCore/QList>
#include <QtCore/QUrl>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

class QByteArray;

class QDeclarativeScriptParserJsASTData;
class QDeclarativeScriptParser
{
public:
    class Import
    {
    public:
        Import() : type(Library) {}

        enum Type { Library, File, Script };
        Type type;

        QString uri;
        QString qualifier;
        QString version;

        QDeclarativeParser::LocationSpan location;
    };

    class TypeReference
    {
    public:
        TypeReference(int typeId, const QString &typeName) : id(typeId), name(typeName) {}

        int id;
        // type as it has been referenced in Qml
        QString name;
        // objects in parse tree referencing the type
        QList<QDeclarativeParser::Object*> refObjects;
    };

    QDeclarativeScriptParser();
    ~QDeclarativeScriptParser();

    bool parse(const QByteArray &data, const QUrl &url = QUrl());

    QList<TypeReference*> referencedTypes() const;

    QDeclarativeParser::Object *tree() const;
    QList<Import> imports() const;

    void clear();

    QList<QDeclarativeError> errors() const;

    class JavaScriptMetaData {
    public:
        JavaScriptMetaData() 
        : pragmas(QDeclarativeParser::Object::ScriptBlock::None) {}

        QDeclarativeParser::Object::ScriptBlock::Pragmas pragmas;
        QList<Import> imports;
    };

    static QDeclarativeParser::Object::ScriptBlock::Pragmas extractPragmas(QString &);
    static JavaScriptMetaData extractMetaData(QString &);


// ### private:
    TypeReference *findOrCreateType(const QString &name);
    void setTree(QDeclarativeParser::Object *tree);

    void setScriptFile(const QString &filename) {_scriptFile = filename; }
    QString scriptFile() const { return _scriptFile; }

// ### private:
    QList<QDeclarativeError> _errors;

    QDeclarativeParser::Object *root;
    QList<Import> _imports;
    QList<TypeReference*> _refTypes;
    QString _scriptFile;
    QDeclarativeScriptParserJsASTData *data;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVESCRIPTPARSER_P_H
