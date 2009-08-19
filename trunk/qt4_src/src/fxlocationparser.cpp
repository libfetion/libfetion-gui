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
#include <QtDebug>
#include <QFile>

#include "fxlocationparser.h"
#include "fxResource.h"

FxLocationParser::FxLocationParser()
{
    QFile file(FX_LOCATION_DOM_FILE);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_query.setFocus(&file);

    file.close();
}

QString
FxLocationParser::getProvinceByAlias(QString provinceAlias)
{
    if ("" == provinceAlias)
    {
        return QString::fromUtf8("其它");
    }

    return getValueByTagName("provList", provinceAlias, "");
}

QString
FxLocationParser::getCityByCode(quint32 cityCode)
{
    if (99 == cityCode)
    {
        return QString::fromUtf8("其它");
    }

    return getValueByTagName("cityList", QString::number(cityCode), "");
}

/**
 * Important!!
 *    Following xml parser tight close to identical DOM hierarchy.
 *    XML must ensure the unique key
 */
QString
FxLocationParser::getValueByTagName(QString tag,
                                    QString item,
                                    QString defValue)
{
    QString results;

    QString xpath =
            QString("/document/" + tag + "/item" + "[@key=\"" + item + "\"]") ;

    m_query.setQuery(xpath, QUrl(FX_LOCATION_DOM_FILE));

    if ( ! m_query.isValid())
        return NULL;

    m_query.evaluateTo(&results);
    results.remove(QChar('\n'), Qt::CaseInsensitive);

    return results;
}

FxLocationParser::~FxLocationParser()
{
}
/**  Example location xml data format:
    <document>
    <cityList>
        <item key="10">北京</item>
        <item key="20">广州</item>
        <item key="21">上海</item>
        <item key="22">天津</item>
    </cityList>
    <provList>
        <item key="bj">北京市 </item>
        <item key="sh">上海市 </item>
        <item key="tj">天津市 </item>
        <item key="cq">重庆市 </item>
    </provList>
    <document>
*/
