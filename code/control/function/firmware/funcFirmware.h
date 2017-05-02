#ifndef FUNCFIRMWARE_H
#define FUNCFIRMWARE_H
#include <QThread>
#include <QTime>
#include <QDir>
#include <QTimer>
#include "globalStruct.h"
#include "server/sysData/serverFirmware.h"
#include "file/config/fileConfig.h"

#define FIRMWARE_VERSION    "4.8.9"                                       // 固件版本

#define CHECK_UPDATE_INTERVAL      (60*60*1000)
#define RETRY_CHECK_UPDATE_INTERVAL     6000                            // 重试检查版本信息
#define REBOOT_INTERVAL     3000
#define SYSTEM_VERSION_FILE "./IMG_Version"

#define SYSTEM          "system"
#define FIRMWARE        "firmware"

enum DownloadType
{
    DOWNLOADED = 0,
    DOWNLOADING = 1,
    DISDOWNLOAD = 2
};

class FuncFirmware : public QThread
{
    Q_OBJECT
public:
    static FuncFirmware *getInstance();                                  // 获取单例
    QString get_version_firmware();
    QString get_version_system();

    InfoVersion get_version();

signals:
    void signal_check_update(QString,QString,QString version = "release");
    void signal_download_file(InfoFirmware);

    void signal_checkFirmware_success(QString version,QString desc ="");                      // 更新版本信息
    void signal_checkFirmware_success();

    void signal_download_process(qint64,qint64);                        // 更新进度条
    void signal_download_success();                                     // 下载成功信号
    void signal_install_failed();

    void signal_net_abnormal();                                         // 网络异常
    void signal_net_disconnect();                                       // 网络断开

    void signal_device_reboot();


protected slots:
    void slot_net_connected();
    void slot_net_disconnected();

    void slot_check_success(InfoFirmware);                           // 有版本更新信息，返回固件信息

    void slot_check_failed();                                    // 检查过程中出现错误

    void slot_download_process(qint64,qint64);                          // 更新下载进度
    void slot_download_success();                                       // 下载成功
    void slot_download_failed();                                        // 下载失败

    void slot_download_manual();                                        // 手动下载
    void slot_download_auto();                                          // 自动下载

    void slot_checkRelease_manual();                                    // 手动检测发布版
    void slot_checkDebug_manual();                                      // 手动检测测试版
    void slot_check_auto();                                             // 自动检测


private:
    explicit FuncFirmware();
    void connect_init();
    void system_reboot();                                                 // 系统重启
    void data_init();

private:
    FileConfig *fileConfig;

    QTimer *timerCheck;
    QTimer *timerRetry;
    QTimer *timerDownload;

    InfoFirmware newFirmware;
    DownloadType status;

    ServerFirmware *serverFirmware;

    bool isConnected;                                                   // 是否连接
    bool isManual;

    qint64 readySize;
    qint64 totalSize;

private:
    static FuncFirmware *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FuncFirmware::instance)
                delete FuncFirmware::instance;
        }
    };
    static FreeInstance free;
};

#endif // FUNCFIRMWARE_H
