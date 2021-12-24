/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_DISTRIBUTED_SCHED_SERVICE_H
#define OHOS_DISTRIBUTED_SCHED_SERVICE_H

#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>

#include "distributed_sched_stub.h"
#include "distributed_sched_continuation.h"
#include "iremote_object.h"
#include "iremote_proxy.h"
#include "mission/mission_info.h"
#include "nocopyable.h"
#include "single_instance.h"
#include "system_ability.h"

namespace OHOS {
namespace DistributedSchedule {
class ConnectAbilitySession;

enum class ServiceRunningState {
    STATE_NO_START,
    STATE_RUNNING
};

enum class TargetComponent {
    HARMONY_COMPONENT,
};

struct ConnectInfo {
    CallerInfo callerInfo;
    sptr<IRemoteObject> callbackWrapper;
};

class DistributedSchedService : public SystemAbility, public DistributedSchedStub {
DECLARE_SYSTEM_ABILITY(DistributedSchedService);

DECLARE_SINGLE_INSTANCE_BASE(DistributedSchedService);

public:
    ~DistributedSchedService() = default;
    void OnStart() override;
    void OnStop() override;
    int32_t StartRemoteAbility(const OHOS::AAFwk::Want& want, int32_t callerUid, int32_t requestCode) override;
    int32_t StartAbilityFromRemote(const OHOS::AAFwk::Want& want,
        const OHOS::AppExecFwk::AbilityInfo& abilityInfo, int32_t requestCode, const CallerInfo& callerInfo,
        const AccountInfo& accountInfo) override;
    int32_t StartContinuation(const OHOS::AAFwk::Want& want, const sptr<IRemoteObject>& abilityToken,
        int32_t callerUid) override;
    void NotifyCompleteContinuation(const std::u16string& devId, int32_t sessionId, bool isSuccess) override;
    int32_t NotifyContinuationResultFromRemote(int32_t sessionId, bool isSuccess) override;
    int32_t ConnectRemoteAbility(const OHOS::AAFwk::Want& want,
        const sptr<IRemoteObject>& connect, int32_t callerUid, int32_t callerPid) override;
    int32_t DisconnectRemoteAbility(const sptr<IRemoteObject>& connect) override;
    int32_t ConnectAbilityFromRemote(const OHOS::AAFwk::Want& want, const AppExecFwk::AbilityInfo& abilityInfo,
        const sptr<IRemoteObject>& connect, const CallerInfo& callerInfo, const AccountInfo& accountInfo) override;
    int32_t DisconnectAbilityFromRemote(const sptr<IRemoteObject>& connect,
        int32_t uid, const std::string& sourceDeviceId) override;
    int32_t NotifyProcessDiedFromRemote(const CallerInfo& callerInfo) override;
    int32_t GetMissionInfos(const std::string& deviceId, int32_t numMissions,
        std::vector<MissionInfo>& missionInfos) override;
    int32_t StoreSnapshotInfo(const std::string& deviceId, int32_t missionId,
        const uint8_t* byteStream, size_t len) override;
    int32_t RemoveSnapshotInfo(const std::string& deviceId, int32_t missionId) override;
    int32_t NotifyMissionsChangedFromRemote(const std::vector<MissionInfo>& missionInfos,
        const CallerInfo& callerInfo) override;
    void ProcessConnectDied(const sptr<IRemoteObject>& connect);
    void ProcessDeviceOffline(const std::string& deviceId);
    int32_t Dump(int32_t fd, const std::vector<std::u16string>& args) override;
    void DumpConnectInfo(std::string& info);
    void DumpSessionsLocked(const std::list<ConnectAbilitySession>& sessionsList, std::string& info);
    void DumpElementLocked(const std::list<AppExecFwk::ElementName>& elementsList, std::string& info);
    int32_t CheckSupportOsd(const std::string& deviceId) override;
    void GetCachedOsdSwitch(std::vector<std::u16string>& deviceIds, std::vector<int32_t>& values) override;
    int32_t GetOsdSwitchValueFromRemote() override;
    int32_t UpdateOsdSwitchValueFromRemote(int32_t switchVal, const std::string& sourceDeviceId) override;
    std::unique_ptr<Snapshot> GetRemoteSnapshotInfo(const std::u16string& deviceId, int32_t missionId) override;
    int32_t RegisterRemoteMissionListener(const std::u16string& devId, const sptr<IRemoteObject>& obj) override;
    int32_t UnRegisterRemoteMissionListener(const std::u16string& devId, const sptr<IRemoteObject>& obj) override;
    int32_t PrepareAndSyncMissionsFromRemote(const CallerInfo& callerInfo,
        std::vector<MissionInfo>& missionInfos) override;
    int32_t UnRegisterMissionListenerFromRemote(const CallerInfo& callerInfo) override;
    int32_t PrepareAndSyncMissions(const std::u16string& devId, bool fixConflict, int64_t tag) override;
private:
    DistributedSchedService();
    bool Init();
    void NotifyContinuationCallbackResult(const sptr<IRemoteObject>& abilityToken, int32_t isSuccess);
    void RemoteConnectAbilityMappingLocked(const sptr<IRemoteObject>& connect, const std::string& localDeviceId,
        const std::string& remoteDeviceId, const AppExecFwk::ElementName& element, const CallerInfo& callerInfo,
        TargetComponent targetComponent);
    int32_t DisconnectEachRemoteAbilityLocked(const std::string& localDeviceId,
        const std::string& remoteDeviceId, const sptr<IRemoteObject>& connect);
    sptr<IDistributedSched> GetRemoteDms(const std::string& remoteDeviceId);
    static bool GetLocalDeviceId(std::string& localDeviceId);
    bool CheckDeviceId(const std::string& localDeviceId, const std::string& remoteDeviceId);
    bool CheckDeviceIdFromRemote(const std::string& localDeviceId,
        const std::string& destinationDeviceId, const std::string& sourceDeviceId);
    void NotifyDeviceOfflineToAppLocked(const sptr<IRemoteObject>& connect, const ConnectAbilitySession& session);
    int32_t NotifyApp(const sptr<IRemoteObject>& connect, const AppExecFwk::ElementName& element, int32_t errCode);
    void NotifyProcessDiedLocked(const std::string& remoteDeviceId, const CallerInfo& callerInfo,
        TargetComponent targetComponent);
    int32_t CheckDistributedConnectLocked(const CallerInfo& callerInfo) const;
    void DecreaseConnectLocked(int32_t uid);
    static int32_t GetUidLocked(const std::list<ConnectAbilitySession>& sessionList);
    int32_t TryConnectRemoteAbility(const OHOS::AAFwk::Want& want,
        const sptr<IRemoteObject>& connect, const CallerInfo& callerInfo);

    std::shared_ptr<DSchedContinuation> dschedContinuation_;
    std::map<sptr<IRemoteObject>, std::list<ConnectAbilitySession>> distributedConnectAbilityMap_;
    std::map<sptr<IRemoteObject>, ConnectInfo> connectAbilityMap_;
    std::unordered_map<int32_t, uint32_t> trackingUidMap_;
    std::mutex distributedLock_;
    std::mutex connectLock_;
    sptr<IRemoteObject::DeathRecipient> connectDeathRecipient_;
};

class ConnectAbilitySession {
public:
    ConnectAbilitySession(const std::string& sourceDeviceId, const std::string& destinationDeviceId,
        const CallerInfo& callerInfo, TargetComponent targetComponent = TargetComponent::HARMONY_COMPONENT);
    ~ConnectAbilitySession() = default;

    const std::string& GetSourceDeviceId() const
    {
        return sourceDeviceId_;
    }

    const std::string& GetDestinationDeviceId() const
    {
        return destinationDeviceId_;
    }

    std::list<AppExecFwk::ElementName> GetElementsList() const
    {
        return elementsList_;
    }

    CallerInfo GetCallerInfo() const
    {
        return callerInfo_;
    }

    TargetComponent GetTargetComponent() const
    {
        return targetComponent_;
    }

    bool IsSameCaller(const CallerInfo& callerInfo);
    void AddElement(const AppExecFwk::ElementName& element);

private:
    std::string sourceDeviceId_;
    std::string destinationDeviceId_;
    std::list<AppExecFwk::ElementName> elementsList_;
    CallerInfo callerInfo_;
    TargetComponent targetComponent_;
};
} // namespace DistributedSchedule
} // namespace OHOS
#endif // OHOS_DISTRIBUTED_SCHED_SERVICE_H