#pragma once

#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    static NetworkAccessManager& instance();

private:
    explicit NetworkAccessManager(QObject *parent = nullptr);

protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest &request,
                                 QIODevice *outgoingData) override;
};
