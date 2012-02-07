/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

import QtQuick 1.0

ListModel {
    id: rssFeeds

    ListElement { name: "Top Stories"; feed: "rss.news.yahoo.com/rss/topstories" }
    ListElement { name: "World"; feed: "rss.news.yahoo.com/rss/world" }
    ListElement { name: "Europe"; feed: "rss.news.yahoo.com/rss/europe" }
    ListElement { name: "Oceania"; feed: "rss.news.yahoo.com/rss/oceania" }
    ListElement { name: "U.S. National"; feed: "rss.news.yahoo.com/rss/us" }
    ListElement { name: "Politics"; feed: "rss.news.yahoo.com/rss/politics" }
    ListElement { name: "Business"; feed: "rss.news.yahoo.com/rss/business" }
    ListElement { name: "Technology"; feed: "rss.news.yahoo.com/rss/tech" }
    ListElement { name: "Entertainment"; feed: "rss.news.yahoo.com/rss/entertainment" }
    ListElement { name: "Health"; feed: "rss.news.yahoo.com/rss/health" }
    ListElement { name: "Science"; feed: "rss.news.yahoo.com/rss/science" }
    ListElement { name: "Sports"; feed: "rss.news.yahoo.com/rss/sports" }
}
