#ifndef TELEGRAMDOWNLOADHANDLER_H
#define TELEGRAMDOWNLOADHANDLER_H

#include "telegramqml_macros.h"
#include "telegramqml_global.h"
#include "tqobject.h"

class PhotoObject;
class PhotoSizeObject;
class DocumentObject;
class FileLocationObject;
class TelegramFileLocation;
class TelegramEngine;
class TelegramTypeQObject;
class TelegramDownloadHandlerPrivate;
class TELEGRAMQMLSHARED_EXPORT TelegramDownloadHandler : public TqObject
{
    Q_OBJECT
    Q_PROPERTY(TelegramTypeQObject* source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(TelegramFileLocation* target READ target NOTIFY targetChanged)
    Q_PROPERTY(int targetType READ targetType NOTIFY targetTypeChanged)
    Q_PROPERTY(qint32 size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qint32 dowloadedSize READ dowloadedSize NOTIFY dowloadedSizeChanged)
    Q_PROPERTY(qint32 downloadTotal READ downloadTotal NOTIFY downloadTotalChanged)
    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
    Q_PROPERTY(QString destination READ destination NOTIFY destinationChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)

public:
    enum ObjectType {
        TypeObjectEmpty,
        TypeObjectMessage,
        TypeObjectPeer,
        TypeObjectDialog,
        TypeObjectUser,
        TypeObjectChat,
        TypeObjectFileLocation,
        TypeObjectMessageAction,
        TypeObjectMessageMedia,
        TypeObjectDocument,
        TypeObjectPhoto,
        TypeObjectPhotoSize,
        TypeObjectUserProfilePhoto,
        TypeObjectChatPhoto
    };

    enum TargetType {
        TypeTargetUnknown,
        TypeTargetMediaPhoto,
        TypeTargetMediaAudio,
        TypeTargetMediaVideo,
        TypeTargetMediaDocument,
        TypeTargetChatPhoto,
        TypeTargetUserPhoto,
        TypeTargetActionChatPhoto
    };

    TelegramDownloadHandler(QObject *parent = 0);
    ~TelegramDownloadHandler();

    void setSource(TelegramTypeQObject *source);
    TelegramTypeQObject *source() const;

    TelegramEngine *engine() const;
    void setEngine(TelegramEngine *engine);

    TelegramFileLocation *target() const;
    int targetType() const;

    qint32 size() const;
    qint32 dowloadedSize() const;
    qint32 downloadTotal() const;
    bool downloading() const;
    QString destination() const;
    QString thumbnail() const;

public Q_SLOTS:
    bool download();
    bool check();

Q_SIGNALS:
    void sourceChanged();
    void targetChanged();
    void engineChanged();
    void targetTypeChanged();
    void sizeChanged();
    void dowloadedSizeChanged();
    void downloadTotalChanged();
    void downloadingChanged();
    void destinationChanged();
    void thumbnailChanged();

private:
    TelegramFileLocation *findTarget(QObject *source, int *targetType = 0, QObject **targetPointer = 0);
    ObjectType findObjectType(QObject *object);

    TelegramFileLocation *locationOf(FileLocationObject *obj);
    TelegramFileLocation *locationOf(DocumentObject *obj);
    TelegramFileLocation *locationOf(PhotoObject *obj, bool thumbnail = false);
    TelegramFileLocation *locationOf(PhotoSizeObject *obj);

protected:
    void registerLocation(TelegramFileLocation *loc, const QByteArray &hash);
    void retry();

private:
    TelegramDownloadHandlerPrivate *p;
};

#endif // TELEGRAMDOWNLOADHANDLER_H
