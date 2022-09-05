#include "NetworkAccessManager.h"

#include <QCoreApplication>
#include <QLocale>
#include <QNetworkReply>


static NetworkAccessManager *namInstance = nullptr;

void cleanupNetworkAccessManager()
{
    delete namInstance;
    namInstance = nullptr;
}

NetworkAccessManager& NetworkAccessManager::instance()
{
    static NetworkAccessManager self;
    return self;
}

NetworkAccessManager::NetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{

}

QNetworkReply* NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString agentStr = QString::fromLatin1("%1/%2 (QNetworkAccessManager %3; %4; %5; %6 bit)")
                    .arg(QCoreApplication::applicationName(),
                         QCoreApplication::applicationVersion(),
                         QLatin1String(qVersion()),
                         QSysInfo::prettyProductName(),
                         QLocale::system().name())
                    .arg(QSysInfo::WordSize);
    QNetworkRequest req(request);
    req.setRawHeader("User-Agent", agentStr.toLatin1());
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}

