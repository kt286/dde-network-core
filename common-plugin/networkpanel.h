/*
 * Copyright (C) 2011 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     donghualin <donghualin@uniontech.com>
 *
 * Maintainer: donghualin <donghualin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORKPANEL_H
#define NETWORKPANEL_H

#include "item/devicestatushandler.h"

#include <DGuiApplicationHelper>

#include <NetworkManagerQt/Device>

#include <com_deepin_daemon_network.h>

DGUI_USE_NAMESPACE

namespace Dock {
class TipsWidget;
}

namespace dde {
namespace network {
enum class DeviceType;
class NetworkDeviceBase;
} // namespace network
} // namespace dde

class NetworkDialog;
class QTimer;
class NetItem;

using namespace dde::network;
using DbusNetwork = com::deepin::daemon::Network;

class NetworkPanel : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void sendIpConflictDect(int);
    void iconChange();
    void addDevice(const QString &devicePath);

public:
    explicit NetworkPanel(QObject *parent = Q_NULLPTR);
    ~NetworkPanel();

    void invokeMenuItem(const QString &menuId);
    bool needShowControlCenter();
    const QString contextMenu(bool hasSetting) const;
    QWidget *itemTips();
    bool hasDevice();
    void setGreeterStyle(bool greeterStyle);

    void refreshIcon();
    QPixmap icon();

private:
    void initUi();
    void initConnection();
    void getPluginState();
    void updateTooltips(); // 更新提示的内容
    bool deviceEnabled(const DeviceType &deviceType) const;
    void setDeviceEnabled(const DeviceType &deviceType, bool enabeld);

    int getStrongestAp();
    int deviceCount(const DeviceType &devType) const;
    QStringList ipTipsMessage(const DeviceType &devType);

    QStringList getIPList(const DeviceType &deviceType) const;
    QStringList getActiveWiredList() const;
    QStringList getActiveWirelessList() const;
    QStringList currentIpList() const;
    QString getStrengthStateString(int strength);

private Q_SLOTS:
    void onDeviceAdded(QList<NetworkDeviceBase *> devices);
    void onDeviceStatusChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);
    void onUpdatePlugView();

    void onIPConfllict(const QString &ip, const QString &mac);
    void onSendIpConflictDect(int index = 0);
    void onDetectConflict();

private:
    PluginState m_pluginState;

    QTimer *m_refreshIconTimer;
    QTimer *m_switchWireTimer;
    QTimer *m_wirelessScanTimer;

    Dock::TipsWidget *m_tipsWidget;
    bool m_switchWire;
    QPixmap m_iconPixmap;

    // 判断定时的时间是否到,否则不重置计时器
    bool m_timeOut;

    DbusNetwork *m_networkInter;
    QStringList m_disconflictList;        // 解除冲突数据列表
    QMap<QString, QString> m_conflictMap; // 缓存有线和无线冲突的ip列表
    QTimer *m_detectConflictTimer;        // 定时器自检,当其他主机主动解除ip冲突，我方需要更新网络状态
    bool m_ipConflict;                    // ip冲突的标识
    bool m_ipConflictChecking;            // 标记是否正在检测中
    bool m_greeterStyle;                  // 登录界面样式

    QSet<QString> m_devicePaths; // 记录无线设备Path,防止信号重复连接
    QString m_lastActiveWirelessDevicePath;
};

#endif // NETWORKPANEL_H