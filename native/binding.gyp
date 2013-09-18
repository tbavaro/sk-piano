{
  "targets": [
    {
      "target_name": "piano_native",
      "sources": [
        "src/RuntimeException.cpp",
        "src/Spi.cpp",
        "src/NodeModule.cpp",
        "src/WrappedMMap.cpp",
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
