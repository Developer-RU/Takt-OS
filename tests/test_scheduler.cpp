#include <gtest/gtest.h>
#include "takt/scheduler.hpp"
#include "takt/imodule.hpp"

namespace {

class MockModule : public takt::IModule {
public:
    MockModule(const char* n, takt::ModuleType t) : name_(n), type_(t) {}
    bool init() override { initCalled = true; return initResult; }
    void tick() override { ++tickCount; }
    void shutdown() override { shutdownCalled = true; }
    const char* name() const override { return name_; }
    takt::ModuleType type() const override { return type_; }
    bool hasWork() const override { return hasWork_; }

    bool initCalled = false;
    bool shutdownCalled = false;
    bool initResult = true;
    bool hasWork_ = true;
    int tickCount = 0;
    const char* name_;
    takt::ModuleType type_;
};

TEST(SchedulerTest, RegisterAndInit) {
    takt::Scheduler scheduler;
    MockModule mod("test", takt::ModuleType::Static);
    auto id = scheduler.registerModule(&mod);
    EXPECT_NE(id, takt::kInvalidModuleId);
    EXPECT_TRUE(scheduler.initAll());
    EXPECT_TRUE(mod.initCalled);
}

TEST(SchedulerTest, RunTaktCallsTick) {
    takt::Scheduler scheduler;
    MockModule mod("test", takt::ModuleType::Static);
    scheduler.registerModule(&mod);
    scheduler.initAll();
    scheduler.runTakt();
    EXPECT_EQ(mod.tickCount, 1);
}

TEST(SchedulerTest, BackgroundModuleSkippedWhenIdle) {
    takt::Scheduler scheduler;
    MockModule mod("bg", takt::ModuleType::Background);
    mod.hasWork_ = false;
    scheduler.registerModule(&mod);
    scheduler.initAll();
    scheduler.runTakt();
    EXPECT_EQ(mod.tickCount, 0);
}

} // namespace
