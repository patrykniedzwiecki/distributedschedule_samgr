/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_ISYSTEM_ABILITY_LOAD_CALLBACK_H
#define SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_ISYSTEM_ABILITY_LOAD_CALLBACK_H

#include "iremote_broker.h"
#include "iremote_object.h"
#include "iremote_proxy.h"

namespace OHOS {
class ISystemAbilityLoadCallback : public IRemoteBroker {
public:
    virtual ~ISystemAbilityLoadCallback() = default;
    virtual void OnLoadSystemAbilitySuccess(int32_t systemAbilityId, const sptr<IRemoteObject>& remoteObject) = 0;
    virtual void OnLoadSystemAbilityFail(int32_t systemAbilityId) = 0;
    DECLARE_INTERFACE_DESCRIPTOR(u"OHOS.ISystemAbilityLoadCallback");
protected:
    enum {
        ON_LOAD_SYSTEM_ABILITY_SUCCESS = 1,
        ON_LOAD_SYSTEM_ABILITY_FAIL = 2,
    };
};
}
#endif /* SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_ISYSTEM_ABILITY_LOAD_CALLBACK_H */
