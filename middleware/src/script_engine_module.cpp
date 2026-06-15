#include "takt/modules/script_engine_module.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "ScriptEngine";
}

bool ScriptEngineModule::init() {
    TAKT_LOGI(TAG, "Script engine init");
    return true;
}

void ScriptEngineModule::enqueueInstruction(uint32_t opcode, uint32_t operand) {
    if (queueCount_ >= kQueueSize) return;
    queue_[queueTail_] = {opcode, operand};
    queueTail_ = (queueTail_ + 1) % kQueueSize;
    ++queueCount_;
    workPending_ = true;
}

bool ScriptEngineModule::executeNextInstruction() {
    if (queueCount_ == 0) return false;
    const auto& instr = queue_[queueHead_];
    TAKT_LOGD(TAG, "Execute opcode=%u operand=%u", instr.opcode, instr.operand);
    queueHead_ = (queueHead_ + 1) % kQueueSize;
    --queueCount_;
    if (queueCount_ == 0) workPending_ = false;
    return true;
}

void ScriptEngineModule::tick() {
    while (hasWork()) {
        if (!executeNextInstruction()) break;
    }
}

void ScriptEngineModule::shutdown() {
    queueCount_ = 0;
    workPending_ = false;
    TAKT_LOGI(TAG, "Script engine shutdown");
}

} // namespace takt::modules
