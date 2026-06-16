# TAKT OS — UML Component Diagram

```mermaid
graph TB
    subgraph "Application Layer"
        DemoApp[Demo Controller]
        Gateway[IoT Gateway]
        Telemetry[Telemetry Node]
    end

    subgraph "Middleware"
        UART[UartModule]
        Sensor[SensorModule]
        Script[ScriptEngineModule]
        WiFi[WiFiModule]
        MQTT[MqttModule]
        BLE[BleModule]
    end

    subgraph "Services"
        OTA[OtaService]
    end

    subgraph "Kernel"
        Kernel[Kernel]
        Scheduler[Scheduler]
        EventBus[EventBus]
        TimerMgr[TimerManager]
        Logger[Logger]
        Diag[Diagnostics]
        Storage[StorageManager]
        Cache[CacheManager]
        FwCache[FirmwareCache]
        NVS[NvsManager]
    end

    subgraph "Recovery"
        Recovery[RecoveryManager]
    end

    subgraph "Bootloader"
        Boot[Bootloader]
    end

    subgraph "Drivers"
        Platform[Platform]
        GPIO[GPIO]
        HW_UART[HW UART]
        ADC[ADC]
    end

    DemoApp --> UART
    DemoApp --> Sensor
    DemoApp --> WiFi
    DemoApp --> MQTT
    DemoApp --> OTA

    UART --> Scheduler
    Sensor --> Scheduler
    WiFi --> Scheduler
    MQTT --> Scheduler
    BLE --> Scheduler
    OTA --> Scheduler

    Kernel --> Scheduler
    Kernel --> EventBus
    Kernel --> TimerMgr
    Kernel --> Diag

    Scheduler --> EventBus
    Scheduler --> TimerMgr

    OTA --> FwCache
    Recovery --> FwCache
    FwCache --> Storage
    Cache --> Storage

    Boot --> Recovery
    Boot --> FwCache

    UART --> HW_UART
    Sensor --> ADC
    WiFi --> Platform
    Platform --> GPIO
```

# TAKT OS — UML Class Diagram (Modules)

```mermaid
classDiagram
    class IModule {
        <<interface>>
        +init() bool
        +tick() void
        +shutdown() void
        +name() const char*
        +type() ModuleType
        +budgetUs() uint32_t
        +hasWork() bool
    }

    class UartModule {
        -int port_
        -size_t bytesPerTick_
        +init() bool
        +tick() void
        +type() Static
        +budgetUs() 500
    }

    class SensorModule {
        -float lastValue_
        +tick() void
        +type() Static
        +budgetUs() 2000
    }

    class ScriptEngineModule {
        -Instruction[] queue_
        -bool workPending_
        +tick() void
        +type() Dynamic
        +hasWork() bool
    }

    class WiFiModule {
        -bool connected_
        -Timer reconnectTimer_
        +tick() void
        +type() Background
        +hasWork() bool
    }

    class BleModule {
        -bool eventPending_
        +tick() void
        +type() Background
        +hasWork() bool
    }

    class MqttModule {
        -bool msgPending_
        +publish(topic, payload) bool
        +type() Background
    }

    class OtaService {
        -bool active_
        +startUpdate(transport, size, ver) bool
        +rollback() bool
        +type() Background
    }

    IModule <|.. UartModule
    IModule <|.. SensorModule
    IModule <|.. ScriptEngineModule
    IModule <|.. WiFiModule
    IModule <|.. BleModule
    IModule <|.. MqttModule
    IModule <|.. OtaService
```

# TAKT OS — UML Sequence: Full Takt Cycle

```mermaid
sequenceDiagram
    participant K as Kernel
    participant S as Scheduler
    participant TM as TimerManager
    participant EB as EventBus
    participant UART as UartModule
    participant SENS as SensorModule
    participant WIFI as WiFiModule

    K->>S: runTakt()
    S->>TM: tick(1ms)
    S->>EB: dispatchQueued()
    S->>UART: tick()
    UART-->>S: 45us
    S->>SENS: tick()
    SENS->>EB: enqueue(SensorDataReady)
    SENS-->>S: 120us
    S->>WIFI: hasWork()?
    alt reconnect pending
        WIFI->>WIFI: tick()
        WIFI->>EB: publish(WiFiConnected)
        WIFI-->>S: 200us
    else idle
        S->>S: skip (skipCount++)
    end
    S->>S: update stats, check overrun
    S-->>K: takt complete
```

# TAKT OS — UML State: Module Lifecycle

```mermaid
stateDiagram-v2
    [*] --> Unregistered
    Unregistered --> Registered: registerModule()
    Registered --> Initializing: initAll()
    Initializing --> Ready: init() OK
    Initializing --> Error: init() FAIL
    Ready --> Running: tick()
    Running --> Ready: tick() done
    Ready --> Suspended: (future)
    Ready --> Shutdown: shutdownAll()
    Error --> Shutdown: shutdownAll()
    Shutdown --> [*]
```

# TAKT OS — UML Deployment

```mermaid
graph LR
    subgraph "ESP32 Flash"
        BL[Bootloader 28KB]
        PT[Partition Table 4KB]
        REC[Recovery 256KB]
        APP_A[App Slot A 1.5MB]
        APP_B[App Slot B 1.5MB]
        NVS_P[NVS 64KB]
        STOR[Storage 640KB]
    end

    subgraph "ESP32 RAM"
        HEAP[Heap]
        STACK[Main Stack]
        CACHE_RAM[Cache Pool]
    end

    BL --> REC
    BL --> APP_A
    BL --> APP_B
    APP_A --> HEAP
    APP_A --> CACHE_RAM
```

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
