#include <Geode/Geode.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

void nopWindows(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x25, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} at {}", funcName, address);
    }
}

void nopWindowsMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopWindows(address, "nopWindowsMulti");
    }
}

void nopWindows2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x81, 0xe1, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} at {}", funcName, address);
    }
}

void nopMac(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x81, 0xe1, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} at {}", funcName, address);
    }
}

void nopMacMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopMac(address, "nopMacMulti");
    }
}

void nopMac2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x41, 0x81, 0xe6, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {}", funcName);
    }
}

void nopMacArm(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x29, 0x4d, 0x40, 0x92},
        {0x1F, 0x20, 0x03, 0xD5}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} at {}", funcName, address);
    }
}

void nopMacArmMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopMacArm(address, "nopMacArmMulti");
    }
}

void nopMacArm2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0xc8, 0x4e, 0x40, 0x92},
        {0x1F, 0x20, 0x03, 0xD5}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {}", funcName);
    }
}

$execute {
    // This patch is intended to remove the AND operation that limits the timer's range.
    // The timer trim problem does not occur on iOS, so we leave that branch empty.
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopWindowsMulti({0x3760bc, 0x085a06, 0x03cf16, 0x38c659, 0x38c799, 0x2d041f,
                           0x0ce160, 0x147be5, 0x1666d0, 0x167038, 0x16708c, 0x3b6506,
                           0x147d6e, 0x27b975, 0x147eb2});
        // Optionally include:
        // nopWindows2(0x00000, "GetGJDailyLevelState finished; its the callback in handleIt");
        
    #elif defined(GEODE_IS_INTEL_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacMulti({0x009748, 0x3f1439, 0x04f7ff, 0x05082b, 0x051a01, 0x071d95,
                     0x072951, 0x08c038, 0x0ac00e, 0x0aca99, 0x0bb276, 0x3b5592,
                     0x0bbcaf, 0x0bf30b, 0x0bf69c, 0x522b89, 0x5222bc, 0x0ddbfc,
                     0x3f1131, 0x3f13d6, 0x522599, 0x20960e, 0x21f5ac, 0x2202bd,
                     0x254f86, 0x254ff1, 0x2556c7, 0x2555d4, 0x257905, 0x257a69,
                     0x257ba4, 0x257c11, 0x38a71a, 0x36d0d2, 0x36d142, 0x36d1b2,
                     0x36c9fd, 0x36ca7d, 0x36cabd, 0x36ccaf, 0x36d222, 0x36d292,
                     0x36d302, 0x378663, 0x38a9aa, 0x393dcc, 0x3941df, 0x3b4edd,
                     0x48116d, 0x54383c, 0x54388a, 0x5438d5, 0x6438f9, 0x6464f9,
                     0x646565});
        nopMac2(0x21edf6, "DailyLevelPage::init");
        
    #elif defined(GEODE_IS_ARM_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacArmMulti({0x00ad9c, 0x09d88c, 0x047268, 0x048e88, 0x0657e0, 0x07fc84,
                        0x09cf28, 0x0aa31c, 0x340b8c, 0x0aafdc, 0x0add00, 0x371efc,
                        0x37215c, 0x47e1bc, 0x47e4bc, 0x1be304, 0x47eb1c, 0x1d24e4,
                        0x202484, 0x20257c, 0x2044c4, 0x204614, 0x204798, 0x319ca4,
                        0x2fe740, 0x2fe7c0, 0x2fe840, 0x2fe018, 0x2fe084, 0x2fe0cc,
                        0x2fe2dc, 0x2fe8c0, 0x2fe940, 0x2fe9c0, 0x308b7c, 0x32266c,
                        0x3404f4, 0x0ae054, 0x56ec38, 0x56ec98, 0x066398, 0x49c828,
                        0x49c880, 0x49c8d8, 0x0c5480, 0x201e9c, 0x201ef8, 0x319f68,
                        0x56c32c, 0x048120, 0x1d31c0, 0x204710, 0x3229f8, 0x3ef0a0});
        nopMacArm2(0x1d1b48, "DailyLevelPage::init");
        
    #elif defined(GEODE_IS_IOS)
        // The timer trim issue does not occur on iOS.
        // Log a message to indicate the patch is not applied.
        log::debug("Timer fix: iOS detected – timer trim patch not necessary.");
        
    #elif defined(GEODE_IS_ANDROID)
        // Android does not have the timer trim issue.
        log::debug("Timer fix: Android detected – timer trim patch is not needed.");
        
    #else
        static_assert(false, "Unsupported platform");
    #endif
}