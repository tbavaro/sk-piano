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
