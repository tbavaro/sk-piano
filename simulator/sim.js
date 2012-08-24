var WEBSOCKETS_URL = "ws://localhost:8001/piano";

var console_div;
var ws;
var keys = [];

var lowest_mapped_key = 39; //C3
var key_mapping_reverse_dvorak = [186,79,81,69,74,75,73,88,68,66,72,77,87,78,86,83,90,222,50,188,51,190,52,80,89,54,70,55,71,67,57,82,48,76,219,191,187];
var key_mapping_reverse_qwerty = [90,83,88,68,67,86,71,66,72,78,74,77,188,76,190,186,191,81,50,50,87,51,69,52,82,84,54,89,55,85,73,57,79,48,80,189,219,221];
var key_mapping = (function(key_mapping_reverse) {
  var map = [];
  var i;
  for (i = 0; i < 256; ++i) {
    map.push(null);
  }
  for (i = 0; i < key_mapping_reverse.length; ++i) {
    map[key_mapping_reverse[i]] = i + lowest_mapped_key;
  }

  // others
  map[13] = 0;

  return map;
//})(key_mapping_reverse_qwerty);
})(key_mapping_reverse_dvorak);

Offsets = {
  TOP: [ 1, 2 ],
  SECOND_ROW: [ 0.9, 1.9 ],
  THIRD_ROW: [ 0.5, 1.5 ],
  BOTTOM_ROW: [ 0.3, 1.3 ],
  LEGS: [ 0.3, 1.3 ]
};

Shapes = {
  parts: [
    {
      // LEDS 0-79, top behind hinges
      num_leds: 80,
      offsets: Offsets.TOP,
      points: [
        [ 0,   1.5 ],
        [ 0,   4 ],
        [ 0.5, 5 ],
        [ 1.5, 5.75 ],
        [ 2.5, 6 ],
        [ 3.5, 5.75 ],
        [ 4.5, 5 ],
        [ 5,   4 ],
        [ 5.5, 3 ],
        [ 6.5, 2 ],
        [ 7,   1.5 ]
      ]
    },
    {
      // LEDS 80-91, top right in front of hinge
      num_leds: 12,
      offsets: Offsets.TOP,
      points: [
        [ 7, 1.5 ],
        [ 7, 0 ]
      ]
    },
    {
      // LEDS 92-135, top front row
      num_leds: 44,
      offsets: Offsets.TOP,
      points: [
        [ 7, 0 ],
        [ 0, 0 ]
      ]
    },
    {
      // LEDS 136-147, top left in front of hinge
      num_leds: 12,
      offsets: Offsets.TOP,
      points: [
        [ 0, 0 ],
        [ 0, 1.5 ]
      ]
    },
    {
      // LEDS 148-162, second row left side up to keys
      num_leds: 15,
      offsets: Offsets.SECOND_ROW,
      points: [
        [ 0, 1.5 ],
        [ 0, 0 ],
        [ 0.2, 0 ],
        [ 0.2, 0.2 ]
      ]
    },
    {
      // LEDS 163-203, second row above keys
      num_leds: 41,
      offsets: Offsets.SECOND_ROW,
      points: [
        [ 0.2, 0.2 ],
        [ 6.8, 0.2 ]
      ]
    },
    {
      // LEDS 204-218, second row right side keys to hinge
      num_leds: 15,
      offsets: Offsets.SECOND_ROW,
      points: [
        [ 6.8, 0.2 ],
        [ 6.8, 0 ],
        [ 7,   0 ],
        [ 7,   1.5 ]
      ]
    },
    {
      // LEDS 219-297, second row right hinge around to left hinge
      num_leds: 79,
      offsets: Offsets.SECOND_ROW,
      points: [
        [ 7,   1.5 ],
        [ 6.5, 2 ],
        [ 5.5, 3 ],
        [ 5,   4 ],
        [ 4.5, 5 ],
        [ 3.5, 5.75 ],
        [ 2.5, 6 ],
        [ 1.5, 5.75 ],
        [ 0.5, 5 ],
        [ 0,   4 ],
        [ 0,   1.5 ]
      ]
    },
    {
      // LEDS 298-319, third row left up to keys
      num_leds: 22,
      offsets: Offsets.THIRD_ROW,
      points: [
        [ 0,   2 ],
        [ 0,   -1 ]
      ]
    },
    {
      // LEDS 320-363, third row across the front
      num_leds: 44,
      offsets: Offsets.THIRD_ROW,
      points: [
        [ 0,   -1 ],
        [ 7,   -1 ]
      ]
    },
    {
      // LEDS 364-457, third row front right all the way around to left hinge
      num_leds: 94,
      offsets: Offsets.THIRD_ROW,
      points: [
        [ 7,   -1 ],
        [ 7,   1.5 ],
        [ 6.5, 2 ],
        [ 5.5, 3 ],
        [ 5,   4 ],
        [ 4.5, 5 ],
        [ 3.5, 5.75 ],
        [ 2.5, 6 ],
        [ 1.5, 5.75 ],
        [ 0.5, 5 ],
        [ 0,   4 ],
        [ 0,   2 ]
      ]
    },
    {
      // LEDS 458-477, bottom row left up to keys
      num_leds: 20,
      offsets: Offsets.BOTTOM_ROW,
      points: [
        [ 0,   2 ],
        [ 0,   -1 ]
      ]
    },
    {
      // LEDS 478-521, bottom row across the front
      num_leds: 44,
      offsets: Offsets.BOTTOM_ROW,
      points: [
        [ 0,   -1 ],
        [ 7,   -1 ]
      ]
    },
    {
      // LEDS 522-541, third row front right all the way around to left hinge
      num_leds: 20,
      offsets: Offsets.BOTTOM_ROW,
      points: [
        [ 7,   -1 ],
        [ 7,   2 ]
      ]
    },
    {
      // LEDS 542-559, up the back of the right leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 6,   1.1 ],
        [ 7,   2.1 ]
      ]
    },
    {
      // LEDS 560-577, down the front of the right leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 7,   1.9 ],
        [ 6,   0.9 ]
      ]
    },
    {
      // LEDS 578-595, down the front right of the back leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 2.75,   5.8 ],
        [ 1.75,   4.8 ]
      ]
    },
    {
      // LEDS 596-613, up the rear right of the back leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 1.75,   5 ],
        [ 2.75,   6 ]
      ]
    },
    {
      // LEDS 614-631, down the rear left of the back leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 2.25,   6 ],
        [ 1.25,   5 ]
      ]
    },
    {
      // LEDS 632-649, up the front left of the back leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 1.25,   4.8 ],
        [ 2.25,   5.8 ]
      ]
    },
    {
      // LEDS 650-667, up the back of the right leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ -1,   1.1 ],
        [ 0,   2.1 ]
      ]
    },
    {
      // LEDS 668-685, down the front of the right leg
      num_leds: 18,
      offsets: Offsets.LEGS,
      points: [
        [ 0,   1.9 ],
        [ -1,   0.9 ]
      ]
    }
  ],

  segmentLength: function(point_a, point_b) {
    var dx = point_a[0] - point_b[0];
    var dy = point_a[1] - point_b[1];
    return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
  },

  length: function(shape) {
    var accum = 0;
    var prev_point = null;
    shape.forEach(function(point) {
      if (prev_point) {
        accum += Shapes.segmentLength(prev_point, point);
      }
      prev_point = point;
    });
    return accum;
  },

  evenlyDistributeAlongShape: function(shape, num_points) {
    var spacing = Shapes.length(shape) / num_points;
    var points = [];
    var cur_segment_idx = 0;
    var cur_segment = null;
    var cur_segment_length = 0;
    var dist_along_cur_segment = 0;
    while (points.length < num_points) {
      while (!cur_segment || cur_segment_length < dist_along_cur_segment) {
        dist_along_cur_segment -= cur_segment_length;
        ++cur_segment_idx;
        cur_segment = [shape[cur_segment_idx - 1], shape[cur_segment_idx]];
        cur_segment_length = 
            Shapes.segmentLength(cur_segment[0], cur_segment[1]);
      }

      var pct_along_segment = dist_along_cur_segment / cur_segment_length;
      var x = cur_segment[0][0] + (cur_segment[1][0] - cur_segment[0][0]) * pct_along_segment;
      var y = cur_segment[0][1] + (cur_segment[1][1] - cur_segment[0][1]) * pct_along_segment;
      points.push([x, y]);
      dist_along_cur_segment += spacing;
    }
    return points;
  }
};

LEDs = (function() {
  var all_leds = [];
  Shapes.parts.forEach(function(part) {
    var offsets = part.offsets;
    var points = part.points.map(function(point) {
      return [point[0] + offsets[0], point[1] + offsets[1]];
    });
    all_leds = all_leds.concat(Shapes.evenlyDistributeAlongShape(points, part.num_leds));
  });
  return all_leds;
})();

var log = function(text) {
  console_div.appendChild(document.createTextNode(text));
  console_div.appendChild(document.createElement("BR"));
  console_div.scrollTop = console_div.scrollHeight;
};

var onLoad = function() {
  WholeStripCanvas.init();
  console_div = document.getElementById("console");

  log("Connecting to " + WEBSOCKETS_URL + " ...");
  WholeStripCanvas.clear();
  WholeStripCanvas.drawBoxes([ "red", "green", "blue" ]);

  ws = new WebSocket(WEBSOCKETS_URL);
     ws.onopen = function()
     {
        log("Connected!");
     };
     ws.onmessage = function (evt) 
     { 
        var msg = evt.data;

        if (msg.indexOf("SHOW:") === 0) {
          var colors = eval(msg.substr(5));
          WholeStripCanvas.drawBoxes(colors);
          PianoCanvas.clear();
          PianoCanvas.drawLights(LEDs, colors);
        } else {
          log(msg);
        }
     };
     ws.onclose = function()
     { 
        // websocket is closed.
        log("Connection is closed...");
        ws = null;
     };

   PianoCanvas.init();
//   PianoCanvas.drawShape(Shapes.piano_top, "#335544");
};

var kc_history = [];

var noteFromKeyCode = function(key_code) {
  if (key_code >= 0 && key_code < key_mapping.length) {
    return key_mapping[key_code];
  } else {
    return null;
  }
};

var onKeyDown = function(event) {
  kc_history.push(event.keyCode);
//  console.log(JSON.stringify(kc_history));

  var note = noteFromKeyCode(event.keyCode);
  if (note !== null && ws !== null && !keys[note]) {
    ws.send("KEY_DOWN:" + note);
    keys[note] = 1;
  }
};

var onKeyUp = function(event) {
  var note = noteFromKeyCode(event.keyCode);
  if (note !== null && ws !== null && keys[note]) {
    ws.send("KEY_UP:" + note);
    keys[note] = 0;
  }
};

var WholeStripCanvas = {
  _canvas: null,

  init: function() {
    WholeStripCanvas._canvas = document.getElementById("whole_strip_canvas");
  },

  draw: function(func) {
    var ctx = WholeStripCanvas._canvas.getContext("2d");
    func(ctx);
  },

  clear: function(color_opt) {
    WholeStripCanvas.draw(function(ctx) {
      var canvas = WholeStripCanvas._canvas;
      ctx.fillStyle = color_opt || "#000000";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    });
  },

  drawBoxes: function(colors) {
    var n = colors.length;
    var box_width = WholeStripCanvas._canvas.width / n;
    var box_height = WholeStripCanvas._canvas.height;
    WholeStripCanvas.draw(function(ctx) {
      ctx.lineWidth = 0;
      for (var i = 0; i < n; ++i) {
        ctx.fillStyle = colors[i];
        ctx.fillRect(i * box_width, 0, box_width + 1, box_height);
      }
    });
  }
};

var PianoCanvas = {
  _canvas: null,

  transformPoint: null, // set in constructor

  init: function() {
    PianoCanvas._canvas = document.getElementById("piano_canvas");
    var canvas = PianoCanvas._canvas;
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    var min = function(a, b) {
     return (a < b) ? a : b;
    };
    var max_xy = 8;
    var square_size = 
       min(canvas.width, canvas.height) * 0.95;
    var multiplier = square_size / max_xy;
    PianoCanvas.multiplier = multiplier;
    console.log("mult: " + multiplier);
    var offset_x = (canvas.width - square_size) / 2;
    var offset_y = (canvas.height - square_size) / 2;
    PianoCanvas.transformPoint = function(point) {
      return [ point[0] * multiplier + offset_x,
               (max_xy - point[1]) * multiplier + offset_y ];
    };
  },

  drawShape: function(shape, stroke_style) {
    var canvas = PianoCanvas._canvas;
    var prev_point = null;
    var ctx = canvas.getContext("2d");
    ctx.beginPath();
    ctx.lineWidth = 1.0;
    ctx.strokeStyle = stroke_style;
    shape.forEach(function(point) {
     point = PianoCanvas.transformPoint(point);
     var x = point[0];
     var y = point[1];
     if (!prev_point) {
       ctx.moveTo(x, y);
     } else {
       ctx.lineTo(x, y);
     }
     prev_point = point;
    });
    ctx.stroke();
  },

  clear: function() {
    var canvas = PianoCanvas._canvas;
    var ctx = canvas.getContext("2d");
    ctx.fillStyle = "#001100";
    ctx.fillRect(0, 0, canvas.width, canvas.height);
  },

  drawLights: function(shape, colors) {
    var canvas = PianoCanvas._canvas;
    var ctx = canvas.getContext("2d");
    var radius = PianoCanvas.multiplier * 0.02;
    var i = 0;
    for (i = shape.length - 1; i >= 0; --i) {
      var point = PianoCanvas.transformPoint(shape[i]);
      if (i < colors.length) {
        ctx.fillStyle = colors[i];
      } else {
        ctx.fillStyle = "#000000";
      }
      var x = point[0];
      var y = point[1];

      ctx.fillRect(x - radius, y - radius, radius * 2, radius * 2);
//      ctx.beginPath();
//      ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
//      ctx.fill();
    }
  }
};
