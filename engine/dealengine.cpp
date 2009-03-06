/*
 *   Copyright 2007 Aaron Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "dealengine.h"

#include <QDate>
#include <QStringList>
#include <QTime>
#include <QTimer>
#include <QXmlQuery>

#include <KDebug>
#include <KLocale>
#include <KSystemTimeZones>
#include <KDateTime>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <KUrl>
#include <ktemporaryfile.h>

#include "plasma/datacontainer.h"

DealEngine::DealEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    m_sources["TramDock"] = "http://rss.tramdock.com/docs/tramdock/rss.xml";
    m_sources["WhiskeyMilitia"] = "http://rss.whiskeymilitia.com/docs/wm/rss.xml";
    m_sources["SteepAndCheap"] = "http://rss.steepandcheap.com/docs/steepcheap/rss.xml";
    m_sources["ChainLove"] = "http://rss.chainlove.com/docs/chainlove/rss.xml";
    m_sources["BonkTown"] = "http://rss.bonktown.com/docs/bonktown/rss.xml";
    m_sources["Brociety"] = "http://rss.brociety.com/docs/brociety/rss.xml";
}

DealEngine::~DealEngine()
{
}

QStringList DealEngine::sources() const
{
     return m_sources.keys();
}

bool DealEngine::sourceRequestEvent(const QString &source)
{
     kDebug() << source;
    return updateSourceEvent(source);
}

bool DealEngine::updateSourceEvent(const QString &source)
{
     kDebug() << source;
     
     QXmlQuery query;
    
     QStringList results;

     query.setQuery(QString("doc('%1')//item[position()=1]/*/string()")
		    .arg(m_sources[source]));
     query.evaluateTo(&results);

     if(results.count() < 13)
	  return false;

     Data data;

     data["name"] = results[0];
     data["description"] = results[1];
     data["link"] = results[2];
     data["pubDate"] = results[3];
     data["listDescription"] = results[4];
     data["price"] = results[5];
     data["priceCurrent"] = results[6];
     data["priceRegular"] = results[7];
     data["tinyImage"] = results[8];
     data["image"] = results[9];
     data["thumbnail"] = results[10];
     data["detailImage"] = results[11];
     data["availability"] = results[12];

     if(m_cachedThumbnailURLs[source] != data["thumbnail"])
     {
	  /* Request new thumbnail */
	  KIO::StoredTransferJob* job = (KIO::StoredTransferJob*)KIO::storedGet(data["thumbnail"].toString(), KIO::Reload, KIO::HideProgressInfo);
	  job->exec();
	 
	  data["pixmap"] = job->data();

	  job->deleteLater();
     }

     setData(source, data);

     m_cachedThumbnailURLs[source] = data["thumbnail"].toString();

     return true;
}

K_EXPORT_PLASMA_DATAENGINE(deal, DealEngine)

#include "dealengine.moc"
