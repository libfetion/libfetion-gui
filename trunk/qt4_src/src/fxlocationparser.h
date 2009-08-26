/***************************************************************************
 *   Copyright (C) 2008-2009 by DDD                                        *
 *   dedodong@163.com                                                      *
 *                                                                         *
 *      Contact: alsor.zhou@gmail.com                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FXLOCATIONPARSER_H
#define FXLOCATIONPARSER_H

#include "appconfig.h"

#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 0)
    #undef FX_XML_USE_DOM
#else
    #define FX_XML_USE_DOM
#endif

#ifdef  FX_XML_USE_DOM
    #include <QDomDocument>

    #define FX_LOCATION_DOM_NAME     "document"
    #define FX_LOCATION_DOM_KEY_NAME "key"
#else
    #include <QXmlQuery>
#endif

#include "fxutil.h"

#define FX_LOCATION_DATA_PATH    defaultDataPath() + "/Location.xml"

class FxLocationParser : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxLocationParser();
        ~FxLocationParser();
        QString getProvinceByAlias(QString provinceAlias);
        QString getCityByCode(int cityCode);

    private:
        QString getValueByTagName(QString tag,
                                  QString item);

    private:
#ifdef FX_XML_USE_DOM
        QDomDocument m_dom;
#else
        QXmlQuery    m_query;
#endif
};

#endif // FXLOCATIONPARSER_H
