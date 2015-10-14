/*
    Copyright (C) 2015 Canonical

    This project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "telegramthumbnailercore.h"

#ifdef UBUNTU_PHONE
#include <stdexcept>
#include <QDebug>
#include <QSize>
#include <QSharedPointer>
#include "thumbnailer-qt.h"
#endif

const int THUMB_SIZE = 128;

TelegramThumbnailerCore::TelegramThumbnailerCore(QObject *parent) : QObject(parent) {
}

TelegramThumbnailerCore::~TelegramThumbnailerCore() {
}

void TelegramThumbnailerCore::createThumbnail(QString source, QString dest) {
#ifdef UBUNTU_PHONE

    try {
        QSize size(THUMB_SIZE, THUMB_SIZE);
        unity::thumbnailer::qt::Thumbnailer thumbnailer;

        QSharedPointer<unity::thumbnailer::qt::Request> request =
                thumbnailer.getThumbnail(source, size);
        // We're on a generic thumbnailer thread, safe to call blocking method.
        request->waitForFinished();
        if (request->isValid()) {
            QImage image = request->image();
            image.save(dest, "JPEG");
        }
        Q_EMIT thumbnailCreated(source);
        return;
    } catch (const std::runtime_error &e) {
        qCritical() << "Failed to generate thumbnail!" << e.what();
    } catch (...) {
        qCritical() << "Failed to generate thumbnail!";
    }
    // Failed to create thumbnail, empty thumb to avoid calling back in here.
    QImage image;
    image.save(dest, "JPEG");
    Q_EMIT thumbnailCreated(source);

#else

    QString ffmpegPath;
#ifdef Q_OS_WIN
    ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg.exe";
#else
#ifdef Q_OS_MAC
    ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg";
#else
    if (!QFileInfo::exists(ffmpegPath) {
        if(QFileInfo::exists("/usr/bin/avconv"))
            ffmpegPath = "/usr/bin/avconv";
        else
            ffmpegPath = "ffmpeg";
    }
#endif
#endif

    QStringList args;
    args << "-itsoffset" << "-4";
    args << "-i" << video;
    args << "-vcodec" << "mjpeg";
    args << "-vframes" << "1";
    args << "-an";
    args << "-f" << "rawvideo";
    args << output;
    args << "-y";

    QProcess prc;
    prc.start(ffmpegPath, args);
    prc.waitForStarted();
    prc.waitForFinished();

    if (prc.exitCode() == 0) {
        Q_EMIT thumbnailCreated(source);
    } else {
        QImage image;
        image.save(dest, "JPEG");
        Q_EMIT thumbnailCreated(source);
    }

#endif
}