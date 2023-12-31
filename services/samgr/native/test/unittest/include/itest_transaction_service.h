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
#ifndef SAMGR_TEST_UNITTEST_INCLUDE_ITEST_TRANSACTION_SERVICE_H
#define SAMGR_TEST_UNITTEST_INCLUDE_ITEST_TRANSACTION_SERVICE_H

#include "iremote_broker.h"
#include "iremote_object.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"
#include "refbase.h"
#include "sam_log.h"

namespace OHOS {
class ITestTransactionService : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"test.ITestTransactionService");
    virtual int32_t ReverseInt(int32_t data, int32_t& rep)
    {
        HILOGI("ITestTransactionService ReverseInt called!");
        return 0;
    }
};

class TestTransactionServiceProxy : public IRemoteProxy<ITestTransactionService> {
public:
    explicit TestTransactionServiceProxy(const sptr<IRemoteObject> &impl)
        : IRemoteProxy<ITestTransactionService>(impl)
    {
    }
    ~TestTransactionServiceProxy() = default;
    int32_t ReverseInt(int32_t data, int32_t& rep) override;
};

class TestTransactionServiceStub : public IRemoteStub<ITestTransactionService> {
public:
    int32_t OnRemoteRequest(uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option) override;
};

class TestTransactionService : public TestTransactionServiceStub {
public:
    TestTransactionService() = default;
    virtual ~TestTransactionService() = default;
    int32_t ReverseInt(int32_t data, int32_t& rep) override;
};
} // namespace OHOS
#endif /* SAMGR_TEST_UNITTEST_INCLUDE_ITEST_TRANSACTION_SERVICE_H */
