/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qservicepackage_p.h"
#include <QDataStream>
#include <QDebug>

QT_BEGIN_NAMESPACE

QServicePackage::QServicePackage()
    : d(0)
{

}

QServicePackage::QServicePackage(const QServicePackage& other)
    : d(other.d)
{
}

QServicePackage& QServicePackage::operator=(const QServicePackage& other)
{
    d = other.d;
    return *this;
}

QServicePackage::~QServicePackage()
{
}

bool QServicePackage::isValid() const
{
    return d;
}

QServicePackage QServicePackage::createResponse() const
{
    Q_ASSERT(d->responseType == QServicePackage::NotAResponse);
    QServicePackage response;
    response.d = new QServicePackagePrivate();
    response.d->packageType = d->packageType;
    response.d->messageId = d->messageId;
    response.d->instanceId = d->instanceId;
    response.d->responseType = QServicePackage::Failed;

    return response;
}

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const QServicePackage& package)
{
    const quint32 magicNumber = 0x78AFAFB;
    out.setVersion(QDataStream::Qt_4_6);
    out << magicNumber;

    const qint8 valid = package.d ? 1 : 0;
    out << (qint8) valid;
    if (valid) {
        out << (qint8) package.d->packageType;
        out << (qint8) package.d->responseType;
        out << package.d->messageId;
        out << package.d->instanceId;
        out << package.d->entry;
        out << package.d->payload;
    }

    return out;
}

QDataStream &operator>>(QDataStream &in, QServicePackage& package)
{
    const quint32 magicNumber = 0x78AFAFB;
    in.setVersion(QDataStream::Qt_4_6);

    quint32 storedMagicNumber;
    in >> storedMagicNumber;
    if (storedMagicNumber != magicNumber) {
        qWarning() << Q_FUNC_INFO << "Datastream doesn't provide serialized QServiceFilter";
        return in;
    }

    qint8 valid;
    in >> valid;
    if (valid) {
        if (!package.d) {
            QServicePackagePrivate* priv = new QServicePackagePrivate();
            package.d = priv;
        } else {
            package.d.detach();
            package.d->clean();
        }
        qint8 data;
        in >> data;
        package.d->packageType = (QServicePackage::Type) data;
        in >> data;
        package.d->responseType = (QServicePackage::ResponseType) data;
        in >> package.d->messageId;
        in >> package.d->instanceId;
        in >> package.d->entry;
        in >> package.d->payload;
    } else {
        if (package.d)
            package.d.reset();
    }

    return in;
}
#endif


#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QServicePackage& p)
{
    if (p.isValid()) {
        QString type;
        switch (p.d->packageType) {
            case QServicePackage::ObjectCreation:
                type = QLatin1String("ObjectCreation");
                break;
            case QServicePackage::MethodCall:
                type = QLatin1String("MethodCall");
                break;
            case QServicePackage::PropertyCall:
                type = QLatin1String("PropertyCall");
                break;
            default:
                break;
        }
        dbg.nospace() << "QServicePackage ";
        dbg.nospace() << type << ' ' << p.d->responseType ; dbg.space();
        dbg.nospace() << p.d->messageId.toString(); dbg.space();
        dbg.nospace() << p.d->entry;dbg.space();
    } else {
        dbg.nospace() << "QServicePackage(invalid)";
    }
    return dbg.space();
}
#endif



QT_END_NAMESPACE
