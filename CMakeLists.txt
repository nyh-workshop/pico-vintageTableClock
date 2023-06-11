# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -Wl,--gc-sections")

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "E:/pico-sdk")

set(PICO_EXTRAS_PATH "E:/pico-extras")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico-vintageTableClock C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(dht)

# Add executable. Default name is the project name, version 0.1

add_executable(pico-vintageTableClock main.cpp
st7920_spi_u8g2_hal.cpp
digitalClockDisplay.cpp
RotaryEncoder/rotaryEncoder.cpp
helperRTC/helperRTC.cpp
audio_i2s/audio_i2s.c
driver/fmSynth_picoI2sAudioDriver.cpp
fmSynth/fmSynth.cpp
fmSynth/envelope.cpp
fmSynth/oscillator.cpp
fmSynth/fmChannel.cpp
fmSynth/patch.cpp
miditones/miditones.cpp
miditones/midiFreqTable.c
miditonesFile/sampleMidi.c)

pico_generate_pio_header(pico-vintageTableClock ${CMAKE_CURRENT_LIST_DIR}/audio_i2s/audio_i2s.pio)

pico_set_program_name(pico-vintageTableClock "pico-vintageTableClock")
pico_set_program_version(pico-vintageTableClock "0.9")

pico_enable_stdio_uart(pico-vintageTableClock 1)
pico_enable_stdio_usb(pico-vintageTableClock 0)

# Add the standard library to the build
target_link_libraries(pico-vintageTableClock
        pico_audio
        pico_multicore
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pico-vintageTableClock PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  u8g2/csrc
  u8g2/cppsrc
  )

file(GLOB U8G2_SRC u8g2/csrc/*.c)
add_library(u8g2 ${U8G2_SRC})

target_compile_definitions(pico-vintageTableClock PRIVATE PICO_AUDIO_I2S_MONO_INPUT)

target_link_options(pico-vintageTableClock PRIVATE -Xlinker --print-memory-usage)

# Add any user requested libraries
target_link_libraries(pico-vintageTableClock
        hardware_spi
        hardware_i2c
        hardware_pio
        hardware_rtc
        hardware_dma
        hardware_interp
        u8g2
        dht
        )

pico_add_extra_outputs(pico-vintageTableClock)

