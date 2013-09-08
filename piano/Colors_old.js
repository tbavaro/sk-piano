Util = {
  now: function() {
    return (new Date()).getTime();
  }
};

Colors = (function() {
  var _bracket01 = function(v) {
    return (v < 0) ? 0 : ((v > 1) ? 1 : v);
  }

  var _rgb_unchecked = function(r, g, b) {
    return (g << 16) | (r << 8) | b;
  };

  var _hsv_unchecked = function(h, s, v) {
    var r, g, b;
    if (s === 0) {
      // achromatic (grey)
      r = g = b = v;
    } else {
      h /= 60;                      // sector 0 to 5
      var i = Math.floor(h);
      var f = h - i;                      // factorial part of h
      var p = v * (1 - s);
      var q = v * (1 - s * f);
      var t = v * (1 - s * (1 - f));
      switch(i) {
        case 0:
          r = v;
          g = t;
          b = p;
          break;
        case 1:
          r = q;
          g = v;
          b = p;
          break;
        case 2:
          r = p;
          g = v;
          b = t;
          break;
        case 3:
          r = p;
          g = q;
          b = v;
          break;
        case 4:
          r = t;
          g = p;
          b = v;
          break;
        default:                // case 5:
          r = v;
          g = p;
          b = q;
          break;
      }
    }
    return _rgb_unchecked(r * 127, g * 127, b * 127);
  }

  return {
    rgb: function(r, g, b) {
      return _rgb_unchecked(r, g, b);
    },

    hsv: function(h, s, v) {
      return _hsv_unchecked(h % 360.0, _bracket01(s), _bracket01(v));
    },

    perfTest: function() {
      var iterations = 100000;
      var n = iterations;
      var start_time = Util.now();
      var x = 0;
      while (n > 0) {
        var h = Math.random() * 360.0
        var s = Math.random();
        var v = Math.random();
        var c = Colors.hsv(h, s, v);
        if (c > x ) {
          x = c;
        }
        n -= 1;
      }
      var end_time = Util.now();
      var duration_ms = (end_time - start_time);
      console.log("total time: " + duration_ms + "ms");
      console.log(
          "time per iteration: " + (duration_ms / iterations).toFixed(6) + "ms");
      console.log(x);
    }
  };
})();

Colors.perfTest();
/*


require "inline"

class Colors
@@compile_start_time = Time.now.to_f

inline do |builder|
    builder.include '"math.h"'
builder.include '"unistd.h"'

builder.add_compile_flags "-x c++"

builder.prefix "
static inline uint32_t _rgb_unchecked(uint32_t r, uint32_t g, uint32_t b) {
    return (g << 16) | (r << 8) | b;
}

#define COLOR_PART(c, shift) ((uint8_t)(0x7f & ((c) >> (shift))))

static inline uint8_t _red(uint32_t c) {
    return COLOR_PART(c, 8);
}

static inline uint8_t _green(uint32_t c) {
    return COLOR_PART(c, 16);
}

static inline uint8_t _blue(uint32_t c) {
    return COLOR_PART(c, 0);
}

#define BRACKET(v, min, max) \
(((v) <= (min)) ? (min) : (((v) >= (max)) ? (max) : (v)))

static inline double bracket(double v) {
    return BRACKET(v, 0.0, 1.0);
}

static inline uint8_t _add_and_bracket127(uint16_t a, uint16_t b) {
    uint16_t c = a + b;
    return BRACKET(c, 0, 127);
}

static uint32_t _hsv_unchecked(double h, double s, double v) {
    float r, g, b;
    if( s == 0 ) {
        // achromatic (grey)
        r = g = b = v;
    } else {
        h /= 60;                      // sector 0 to 5
        int i = floor(h);
        float f = h - i;                      // factorial part of h
        float p = v * (1 - s);
        float q = v * (1 - s * f);
        float t = v * (1 - s * (1 - f));
        switch(i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:                // case 5:
                r = v;
                g = p;
                b = q;
                break;
        }
    }
    return _rgb_unchecked(r * 127, g * 127, b * 127);
}
"

builder.c_singleton "
unsigned int rgb(double r, double g, double b) {
    return _rgb_unchecked(bracket(r) * 127, bracket(g) * 127, bracket(b) * 127);
}
"

builder.c_singleton "
unsigned int hsv(double h, double s, double v) {
    return _hsv_unchecked(fmod(h, 360.0), bracket(s), bracket(v));
}
"

builder.c_singleton "
unsigned int add(unsigned int a, unsigned int b) {
    return _rgb_unchecked(
        _add_and_bracket127(_red(a), _red(b)),
        _add_and_bracket127(_green(a), _green(b)),
        _add_and_bracket127(_blue(a), _blue(b)));
}
"

builder.c_singleton "
double red(unsigned int c) {
    return _red(c) / 127.0;
}
"

builder.c_singleton "
double green(unsigned int c) {
    return _green(c) / 127.0;
}
"

builder.c_singleton "
double blue(unsigned int c) {
    return _blue(c) / 127.0;
}
"
end

STDERR.puts "> Colors.rb native code compiled: %.2fms" %
((Time.now.to_f - @@compile_start_time) * 1000)

def self.BLACK
0
end

def self.WHITE
0x7f7f7f
end

def self.perf_test
iterations = 100000
n = iterations
start_time = Time.now.to_f
while n > 0 do
    h = rand() * 360.0
    s = rand()
v = rand()
hsv(h, s, v)
n -= 1
end
end_time = Time.now.to_f
duration_ms = (end_time - start_time) * 1000.0
puts "total time: %.2fms" % (duration_ms)
puts "time per iteration: %.6fms" % (duration_ms / iterations)
end
end

    */