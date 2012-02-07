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

#ifndef QDECLARATIVEBINDINGOPTIMIZATIONS_P_H
#define QDECLARATIVEBINDINGOPTIMIZATIONS_P_H

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

#include "private/qdeclarativeexpression_p.h"
#include "private/qdeclarativebinding_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

struct QDeclarativeBindingCompilerPrivate;
class QDeclarativeBindingCompiler
{
public:
    QDeclarativeBindingCompiler();
    ~QDeclarativeBindingCompiler();

    // Returns true if bindings were compiled
    bool isValid() const;

    struct Expression
    {
        QDeclarativeParser::Object *component;
        QDeclarativeParser::Object *context;
        QDeclarativeParser::Property *property;
        QDeclarativeParser::Variant expression;
        QHash<QString, QDeclarativeParser::Object *> ids;
        QDeclarativeImports imports;
    };

    // -1 on failure, otherwise the binding index to use
    int compile(const Expression &, QDeclarativeEnginePrivate *);

    // Returns the compiled program
    QByteArray program() const;

    static void dump(const QByteArray &);
private:
    QDeclarativeBindingCompilerPrivate *d;
};

class QDeclarativeCompiledBindingsPrivate;
class QDeclarativeCompiledBindings : public QObject, public QDeclarativeAbstractExpression, public QDeclarativeRefCount
{
public:
    QDeclarativeCompiledBindings(const char *program, QDeclarativeContextData *context, QDeclarativeRefCount *);
    virtual ~QDeclarativeCompiledBindings();

    QDeclarativeAbstractBinding *configBinding(int index, QObject *target, QObject *scope, int property);

protected:
    int qt_metacall(QMetaObject::Call, int, void **);

private:
    Q_DISABLE_COPY(QDeclarativeCompiledBindings)
    Q_DECLARE_PRIVATE(QDeclarativeCompiledBindings)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEBINDINGOPTIMIZATIONS_P_H

