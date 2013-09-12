{
  "targets": [
    {
      "target_name": "tbbeaglebone",
      "sources": [
        "src/runtime_exception.cpp",
        "src/pin.cpp",
        "src/spi.cpp",
        "src/tbbeaglebone.cpp",
        "src/wrapped_spi.cpp"
      ],
      "cflags": [ "-fexceptions" ],
      "cflags_cc": [ "-fexceptions" ],
      "conditions": [
        [
          "OS=='mac'", {
            "defines": [ "NOT_BEAGLEBONE" ],
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
