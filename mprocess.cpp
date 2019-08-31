// MProcess class - Installer-specific extensions to QProcess.
//
//   Copyright (C) 2019 by AK-47
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#include <QDebug>
#include <QEventLoop>
#include <QTimer>

#include "mprocess.h"

MProcess::MProcess(QObject *parent)
    : QProcess(parent)
{
}

bool MProcess::exec(const QString &cmd, const bool rawexec, const QByteArray *input, bool needRead)
{
    if (halting) return false;
    ++execount;
    qDebug().nospace() << "Exec #" << execount << ": " << cmd;
    QEventLoop eloop;
    connect(this, static_cast<void (QProcess::*)(int)>(&QProcess::finished), &eloop, &QEventLoop::quit);
    if (rawexec) start(cmd);
    else start("/bin/bash", QStringList() << "-c" << cmd);
    if (!debugUnusedOutput) {
        if (!needRead) closeReadChannel(QProcess::StandardOutput);
        closeReadChannel(QProcess::StandardError);
    }
    if (input && !(input->isEmpty())) write(*input);
    closeWriteChannel();
    eloop.exec();
    disconnect(this, static_cast<void (QProcess::*)(int)>(&QProcess::finished), 0, 0);
    if (debugUnusedOutput) {
        if (!needRead) {
            const QByteArray &StdOut = readAllStandardOutput();
            if (!StdOut.isEmpty()) qDebug().nospace() << "SOut #" << execount << ": " << StdOut;
        }
        const QByteArray &StdErr = readAllStandardError();
        if (!StdErr.isEmpty()) qDebug().nospace() << "SErr #" << execount << ": " << StdErr;
    }
    qDebug().nospace() << "Exit #" << execount << ": " << exitCode() << " " << exitStatus();
    return (exitStatus() == QProcess::NormalExit && exitCode() == 0);
}

QString MProcess::execOut(const QString &cmd, bool everything)
{
    exec(cmd, false, nullptr, true);
    QString strout(readAllStandardOutput().trimmed());
    if (everything) return strout;
    return strout.section("\n", 0, 0);
}

QStringList MProcess::execOutLines(const QString &cmd)
{
    exec(cmd, false, nullptr, true);
    return QString(readAllStandardOutput().trimmed()).split('\n');
}

void MProcess::halt()
{
    halting = true;
    terminate();
    QTimer::singleShot(5000, this, &QProcess::kill);
}

void MProcess::unhalt()
{
    QEventLoop eloop;
    connect(this, static_cast<void (QProcess::*)(int)>(&QProcess::finished), &eloop, &QEventLoop::quit);
    if (state() != QProcess::NotRunning) {
        closeReadChannel(QProcess::StandardOutput);
        closeReadChannel(QProcess::StandardError);
        closeWriteChannel();
        eloop.exec();
    }
    disconnect(this, static_cast<void (QProcess::*)(int)>(&QProcess::finished), 0, 0);
    halting = false;
}
