#
# Simple visualizer to demonstrate setting colors and depending
# upon time.  The colors cycle endlessly; Notes make the colors spin faster
#


ASANA_BLUE = Colors.hsb(204,0.85,0.3)
ASANA_BRIGHT_BLUE = Colors.hsb(204,0.85,1)
ASANA_GREEN = Colors.hsb(102,1.0,1)


class AsanaWorms extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    @dots = [{"pos":0.0, "speed":-5}, {"pos":0.0, "speed":10}]
    @counter = 0.0

  render: (secondsSinceLastFrame) ->
  
    for i in [0...@dots.length]
      @dots[i]["pos"]+= secondsSinceLastFrame * @dots[i]["speed"]
      if ( @dots[i]["pos"] > @strip.numPixels )
        @dots[i]["pos"] = 0
      if ( @dots[i]["pos"] < 0 )
        @dots[i]["pos"] = @strip.numPixels
      
    if (@pianoKeys.pressedSinceLastFrame.length > 2)
      new_worm = {"pos":(Math.floor(Math.random()*@strip.numPixels)), "speed":(Math.floor(Math.random()*30)-15)}
      @dots.push(new_worm)
  

    for i in [0...@strip.numPixels]
      if (Math.random() > 0.999)
        @strip.setPixel(i, ASANA_BRIGHT_BLUE)
      else
        @strip.setPixel(i, ASANA_BLUE)
      
    for i in [0...@strip.numPixels]
      for j in [0...@dots.length]
        if (Math.abs(@dots[j]["pos"] - i) < 2*Math.abs(Math.sin(@dots[j]["pos"]/2)))
          @strip.setPixel(i, ASANA_GREEN)
