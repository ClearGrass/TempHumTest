#include "networkManage.h"

NetworkManage::NetworkManage()
{
    // 设置为十秒
    durationRequest = 2000;
    readySize = 0;
}

/*******************************************************************************
* Function Name  :  get_request
* Description    :  GET请求
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManage::get_request(QNetworkRequest request)
{

    QTimer *timer = new QTimer();
    timer->setInterval(durationRequest);
    timer->setSingleShot(true);
    QEventLoop loop;
    QString replyData;
    QNetworkReply *reply;
    QByteArray data;
    int num = 3;

    connect(timer,SIGNAL(timeout()), &loop, SLOT(quit()));
    while(num -- > 0)
    {
        reply = this->get(request);
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer->start();

        // 事件循环
        loop.exec();
        if (timer->isActive())
        {
            timer->stop();
            if (reply->error() != QNetworkReply::NoError)
            {
                // 错误处理
                qDebug() << "Error String : " << reply->errorString();
            }
            else
            {
                QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();
                // 根据状态码做进一步数据处理
                if(nStatusCode == 200)
                {
                    data = reply->readAll();
                    replyData.prepend(data);

                    // 发送成功信号
                    emit signal_request_success(replyData);
                    return;
                }
            }
        }
        else
        {
            // 处理超时
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            reply->abort();
            reply->deleteLater();
        }
    }
    emit signal_request_timeout();
}

/*******************************************************************************
* Function Name  :  post_request
* Description    :  POST 请求
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManage::post_request(QNetworkRequest request, QByteArray postData)
{
    QTimer *timer = new QTimer();
    timer->setInterval(durationRequest);
    timer->setSingleShot(true);

    QEventLoop loop;
    QString replyData;
    QByteArray data;
    QNetworkReply *reply;
    int num = 3;
    connect(timer,SIGNAL(timeout()), &loop, SLOT(quit()));

    while(num-- > 0)
    {
        reply = this->post(request,postData);
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer->start();

        // 事件循环
        loop.exec();

        if (timer->isActive())
        {
            timer->stop();
            if (reply->error() != QNetworkReply::NoError)
            {
                // 错误处理
                qDebug() << "Error String : " << reply->errorString();
            }
            else
            {

                QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();

                // 根据状态码做进一步数据处理
                if(nStatusCode == 200)
                {
                    data = reply->readAll();
                    replyData.prepend(data);
                    qDebug()<<replyData;

                    // 发送成功信号
                    emit signal_request_success(replyData);
                    return;
                }
            }
        }
        else
        {  // 处理超时
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            reply->abort();
            reply->deleteLater();
        }
    }
    // 发送超时信号
    emit signal_request_timeout();
}

/*******************************************************************************
* Function Name  :  download_file
* Description    :  下载文件
* Input          :  QString filePath 文件路径
*                   QString fileMD5 文件md5值，用于鉴定，文件完整性
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManage::download_file(File newFile)
{
    qDebug()<<newFile.name<<"!!!!!!!!!!!!!!!!!!!!"<<newFile.url<<newFile.md5;
    QUrl urlDownload = QUrl::fromEncoded(newFile.url.toAscii());
    QNetworkRequest request;
    timerDownload = new QTimer();
    QEventLoop loop;
    file = new QFile(newFile.name);

    // 若此文件已经存在
    if(file->exists())
    {
        if(!file->open(QIODevice::ReadWrite))
        {
            qDebug()<<"Open file failed!!!";
            emit signal_download_failed();
            return;
        }
        readySize = file->size();

        // 若文件大小相同
        if(readySize == newFile.size)
        {
            if(get_fileMD5(newFile.name) == newFile.md5)
            {
                emit signal_download_process(readySize, readySize);
                emit signal_download_success(get_fileMD5(newFile.name));
                return;
            }
            else
            {
                system(qPrintable(QString("rm %1").arg(newFile.name)));
                readySize = 0;
            }
        }
        file->seek(readySize);
        qDebug()<<"$$$$$$$"<<readySize;
    }
    else
    {
        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug()<<"Create file failed!!!";
            emit signal_download_failed();
            return;
        }
        readySize = 0;
    }

    request.setUrl(urlDownload);
    qDebug()<<"%%%%%%%%%%%%%%%%%%%"<<readySize;
    request.setRawHeader("Range", tr("bytes=%1-").arg(readySize).toUtf8());
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    replyDownload = this->get(request);

    timerDownload->setInterval(20000);
    timerDownload->setSingleShot(true);

    connect(timerDownload,SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(replyDownload, SIGNAL(finished()), &loop, SLOT(quit()));

    connect(replyDownload, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    connect(replyDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slot_download_process(qint64,qint64)));
    timerDownload->start();

    // 事件循环
    loop.exec();
    if (timerDownload->isActive())
    {
        timerDownload->stop();

        if (replyDownload->error() != QNetworkReply::NoError)
        {
            QVariant variant = replyDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();
            qDebug()<<replyDownload->error();
            qDebug()<<"Error Code:"<<nStatusCode;

            // 错误处理
            qDebug() << "Error String : " << replyDownload->errorString();
            emit signal_download_failed();
        }
        else
        {
            QVariant variant = replyDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();

            qDebug()<<"@@@@@@@@@@@@@@"<<nStatusCode;

            // 根据状态码做进一步数据处理
            if((nStatusCode == 200) || (206 == nStatusCode))
            {
                file->close();
                emit signal_download_success(get_fileMD5(newFile.name));
            }
            else
            {
                emit signal_download_failed();
            }
        }
    }
    else
    {  // 处理超时
        disconnect(replyDownload, SIGNAL(finished()), &loop, SLOT(quit()));
        replyDownload->abort();

        // 发送超时信号
        emit signal_download_timeout();
        qDebug()<<"DOWNLOAD TIMEOUT!!!!!!!!!!!!!!!!!!!!";

    }
}

/*******************************************************************************
* Function Name  :  slot_readyRead
* Description    :  读取文件信息槽函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManage::slot_readyRead()
{
    if(timerDownload->isActive())
    {
         timerDownload->stop();
         timerDownload->start();
    }
    if(file)
    {
        file->write(replyDownload->readAll());
    }
}

/*******************************************************************************
* Function Name  :  slot_download_process
* Description    :  下载进度
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void NetworkManage::slot_download_process(qint64 ready, qint64 total)
{
   qDebug()<<"PROCESS"<<ready<<total;
   emit signal_download_process((readySize + ready), total);
}

/*******************************************************************************
* Function Name  :  analyse_reply_download
* Description    :  分析下载后的文件，md5加密，看是否匹配
* Input          :  QString filePath 文件路径
* Output         :  None
* Return         :  None
*******************************************************************************/
QString NetworkManage::get_fileMD5(QString filePath)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return " ";
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();
    QString strMD5;
    strMD5.prepend(md5.toHex());

    return strMD5;
}
