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

#include "system_ability_load_callback_proxy.h"

#include "ipc_types.h"
#include "parcel.h"
#include "sam_log.h"

namespace OHOS {
void SystemAbilityLoadCallbackProxy::OnLoadSystemAbilitySuccess(int32_t systemAbilityId,
    const sptr<IRemoteObject>& remoteObject)
{
    if (systemAbilityId <= 0) {
        HILOGE("OnLoadSystemAbilitySuccess systemAbilityId:%{public}d invalid!", systemAbilityId);
        return;
    }

    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        HILOGE("OnLoadSystemAbilitySuccess Remote() return null!");
        return;
    }
    MessageParcel data;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("OnLoadSystemAbilitySuccess write interface token failed!");
        return;
    }
    bool ret = data.WriteInt32(systemAbilityId);
    if (!ret) {
        HILOGE("OnLoadSystemAbilitySuccess write systemAbilityId failed!");
        return;
    }
    if (remoteObject == nullptr) {
        HILOGE("OnLoadSystemAbilitySuccess IRemoteObject null!");
        return;
    }
    ret = data.WriteRemoteObject(remoteObject);
    if (!ret) {
        HILOGE("OnLoadSystemAbilitySuccess write IRemoteObject failed!");
        return;
    }

    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    int32_t status = remote->SendRequest(ON_LOAD_SYSTEM_ABILITY_SUCCESS, data, reply, option);
    if (status != NO_ERROR) {
        HILOGE("OnLoadSystemAbilitySuccess SendRequest failed, return value:%{public}d !", status);
        return;
    }
}
}
