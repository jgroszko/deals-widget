/*
 *   Copyright 2008 John Groszko <grosjoh1@iit.edu>
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

#ifndef DEALENGINE_H
#define DEALENGINE_H

#include "plasma/dataengine.h"

#include <KIO/Job>

/**
 * This class evaluates the basic expressions given in the interface.
 */
class DealEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        DealEngine(QObject* parent, const QVariantList& args);
        ~DealEngine();

	virtual QStringList sources() const;

    protected:
        bool sourceRequestEvent(const QString &source);
        bool updateSourceEvent(const QString &source);

	QMap<QString, QString> m_sources;

	QMap<QString, QString> m_cachedThumbnailURLs;
};

#endif
