cmake_minimum_required(VERSION 3.8)
project(Skyline LANGUAGES CXX ASM VERSION 0.3)

set(BUILD_TESTS OFF)
set(BUILD_TESTING OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Shared Libraries" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(source_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto=full -Wno-unused-command-line-argument")
if (uppercase_CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_compile_definitions(NDEBUG)
endif()

set(CMAKE_POLICY_DEFAULT_CMP0048 OLD)
add_subdirectory("libraries/tinyxml2")
add_subdirectory("libraries/fmt")
add_subdirectory("libraries/oboe")
include_directories("libraries/oboe/include")
include_directories("libraries/vkhpp/include")
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

include_directories(${source_DIR}/skyline)

add_library(skyline SHARED
        ${source_DIR}/main.cpp
        ${source_DIR}/skyline/common.cpp
        ${source_DIR}/skyline/nce/guest.S
        ${source_DIR}/skyline/nce/guest.cpp
        ${source_DIR}/skyline/nce.cpp
        ${source_DIR}/skyline/jvm.cpp
        ${source_DIR}/skyline/audio.cpp
        ${source_DIR}/skyline/audio/track.cpp
        ${source_DIR}/skyline/audio/resampler.cpp
        ${source_DIR}/skyline/gpu.cpp
        ${source_DIR}/skyline/gpu/texture.cpp
        ${source_DIR}/skyline/os.cpp
        ${source_DIR}/skyline/loader/nro.cpp
        ${source_DIR}/skyline/kernel/memory.cpp
        ${source_DIR}/skyline/kernel/ipc.cpp
        ${source_DIR}/skyline/kernel/svc.cpp
        ${source_DIR}/skyline/kernel/types/KProcess.cpp
        ${source_DIR}/skyline/kernel/types/KThread.cpp
        ${source_DIR}/skyline/kernel/types/KSharedMemory.cpp
        ${source_DIR}/skyline/kernel/types/KTransferMemory.cpp
        ${source_DIR}/skyline/kernel/types/KPrivateMemory.cpp
        ${source_DIR}/skyline/services/serviceman.cpp
        ${source_DIR}/skyline/services/common/parcel.cpp
        ${source_DIR}/skyline/services/sm/IUserInterface.cpp
        ${source_DIR}/skyline/services/fatalsrv/IService.cpp
        ${source_DIR}/skyline/services/audio/IAudioOutManager.cpp
        ${source_DIR}/skyline/services/audio/IAudioOut.cpp
        ${source_DIR}/skyline/services/audio/IAudioRendererManager.cpp
        ${source_DIR}/skyline/services/audio/IAudioRenderer/IAudioRenderer.cpp
        ${source_DIR}/skyline/services/audio/IAudioRenderer/voice.cpp
        ${source_DIR}/skyline/services/audio/IAudioRenderer/memory_pool.cpp
        ${source_DIR}/skyline/services/settings/ISystemSettingsServer.cpp
        ${source_DIR}/skyline/services/apm/IManager.cpp
        ${source_DIR}/skyline/services/apm/ISession.cpp
        ${source_DIR}/skyline/services/am/IAllSystemAppletProxiesService.cpp
        ${source_DIR}/skyline/services/am/IApplicationProxyService.cpp
        ${source_DIR}/skyline/services/am/proxy/base_proxy.cpp
        ${source_DIR}/skyline/services/am/proxy/IApplicationProxy.cpp
        ${source_DIR}/skyline/services/am/proxy/ILibraryAppletProxy.cpp
        ${source_DIR}/skyline/services/am/proxy/IOverlayAppletProxy.cpp
        ${source_DIR}/skyline/services/am/proxy/ISystemAppletProxy.cpp
        ${source_DIR}/skyline/services/am/controller/IAppletCommonFunctions.cpp
        ${source_DIR}/skyline/services/am/controller/IApplicationFunctions.cpp
        ${source_DIR}/skyline/services/am/controller/IAudioController.cpp
        ${source_DIR}/skyline/services/am/controller/ICommonStateGetter.cpp
        ${source_DIR}/skyline/services/am/controller/IDebugFunctions.cpp
        ${source_DIR}/skyline/services/am/controller/IDisplayController.cpp
        ${source_DIR}/skyline/services/am/controller/ILibraryAppletCreator.cpp
        ${source_DIR}/skyline/services/am/controller/ISelfController.cpp
        ${source_DIR}/skyline/services/am/controller/IWindowController.cpp
        ${source_DIR}/skyline/services/hid/IHidServer.cpp
        ${source_DIR}/skyline/services/hid/IAppletResource.cpp
        ${source_DIR}/skyline/services/timesrv/IStaticService.cpp
        ${source_DIR}/skyline/services/timesrv/ISystemClock.cpp
        ${source_DIR}/skyline/services/timesrv/ITimeZoneService.cpp
        ${source_DIR}/skyline/services/fssrv/IFileSystemProxy.cpp
        ${source_DIR}/skyline/services/fssrv/IFileSystem.cpp
        ${source_DIR}/skyline/services/nvdrv/INvDrvServices.cpp
        ${source_DIR}/skyline/services/nvdrv/devices/nvmap.cpp
        ${source_DIR}/skyline/services/nvdrv/devices/nvhost_ctrl_gpu.cpp
        ${source_DIR}/skyline/services/nvdrv/devices/nvhost_ctrl.cpp
        ${source_DIR}/skyline/services/nvdrv/devices/nvhost_channel.cpp
        ${source_DIR}/skyline/services/nvdrv/devices/nvhost_as_gpu.cpp
        ${source_DIR}/skyline/services/hosbinder/IHOSBinderDriver.cpp
        ${source_DIR}/skyline/services/visrv/IDisplayService.cpp
        ${source_DIR}/skyline/services/visrv/IApplicationDisplayService.cpp
        ${source_DIR}/skyline/services/visrv/IManagerDisplayService.cpp
        ${source_DIR}/skyline/services/visrv/IManagerRootService.cpp
        ${source_DIR}/skyline/services/visrv/ISystemDisplayService.cpp
        )

target_link_libraries(skyline vulkan android fmt tinyxml2 oboe)
target_compile_options(skyline PRIVATE -Wno-c++17-extensions)
