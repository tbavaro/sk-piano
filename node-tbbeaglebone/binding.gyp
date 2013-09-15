{
  "targets": [
    {
      "target_name": "tbbeaglebone",
      "sources": [
        "src/Pin.cpp",
        "src/PinScanner.cpp",
        "src/RuntimeException.cpp",
        "src/SlowPin.cpp",
        "src/Spi.cpp",
        "src/TBBeagleBone.cpp",
        "src/WrappedPin.cpp",
        "src/WrappedPinScanner.cpp",
        "src/WrappedSpi.cpp",
        "src/WrapUtils.cpp"
      ],
      "cflags": [ "-fexceptions" ],
      "cflags_cc": [ "-fexceptions" ],
      "conditions": [
        [
          "OS=='mac'", {
            "defines": [ "IS_SIMULATOR" ],
            "include_dirs": [ "./stubs" ],
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            }
          }
        ]
      ]
    }
  ]
}
