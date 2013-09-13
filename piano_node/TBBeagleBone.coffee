TBBeagleBone = require("../node-tbbeaglebone/build/Release/tbbeaglebone")

TBBeagleBone.Pin.Modes =
  INPUT: 0
  OUTPUT: 1
  
HEADERS =
  8: [
    null, # there's never a pin 0

    # 1:
    null,
    null,
    new TBBeagleBone.Pin("gpmc_ad6", 38),
    new TBBeagleBone.Pin("gpmc_ad7", 39),
    new TBBeagleBone.Pin("gpmc_ad2", 34),

    # 6:
    null, #  new TBBeagleBone.Pin("gpmc_ad3", 35), # something's wrong here, maybe reserved?
    new TBBeagleBone.Pin("gpmc_advn_ale", 66),
    new TBBeagleBone.Pin("gpmc_oen_ren", 67),
    new TBBeagleBone.Pin("gpmc_ben0_cle", 69),
    new TBBeagleBone.Pin("gpmc_wen", 68),

    # 11:
    new TBBeagleBone.Pin("gpmc_ad13", 45),
    new TBBeagleBone.Pin("gpmc_ad12", 44),
    new TBBeagleBone.Pin("gpmc_ad9", 23),
    new TBBeagleBone.Pin("gpmc_ad10", 26),
    new TBBeagleBone.Pin("gpmc_ad15", 47),

    # 16:
    new TBBeagleBone.Pin("gpmc_ad14", 46),
    new TBBeagleBone.Pin("gpmc_ad11", 27),
    new TBBeagleBone.Pin("gpmc_clk", 65),
    new TBBeagleBone.Pin("gpmc_ad8", 22),
    new TBBeagleBone.Pin("gpmc_csn2", 63),

    # 21:
    new TBBeagleBone.Pin("gpmc_csn1", 62),
    new TBBeagleBone.Pin("gpmc_ad5", 37),
    new TBBeagleBone.Pin("gpmc_ad4", 36),
    new TBBeagleBone.Pin("gpmc_ad1", 33),
    new TBBeagleBone.Pin("gpmc_ad0", 32),

    # 26:
    new TBBeagleBone.Pin("gpmc_csn0", 61)
  ]

TBBeagleBone.Pins =
  pin: (headerNumber, pinNumber) ->
    header = HEADERS[headerNumber]
    throw ("unsupported header: " + headerNumber) if !header

    pin = if pinNumber >= 1 && pinNumber < header.length then header[pinNumber]
    throw ("unsupported header/pin: " + headerNumber + "/" + pinNumber) if !pin

    pin

module.exports = TBBeagleBone
