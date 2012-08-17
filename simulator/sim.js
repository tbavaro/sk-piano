var WEBSOCKETS_URL = "ws://localhost:8001/piano";

var console_div;
var ws;
var keys = [];

var lowest_mapped_key = 39; //C3
var key_mapping_reverse_dvorak = [186,79,81,69,74,75,73,88,68,66,72,77,87,78,86,83,90,222,50,188,51,190,52,80,89,54,70,55,71,67,57,82,48,76,219,191,187];
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
})(key_mapping_reverse_dvorak);

var log = function(text) {
  console_div.appendChild(document.createTextNode(text));
  console_div.appendChild(document.createElement("BR"));
  console_div.scrollTop = console_div.scrollHeight;
};

var onLoad = function() {
  Canvas.init();
  console_div = document.getElementById("console");

  log("Connecting to " + WEBSOCKETS_URL + " ...");
  Canvas.clear();
  Canvas.drawBoxes([ "red", "green", "blue" ]);

  ws = new WebSocket(WEBSOCKETS_URL);
     ws.onopen = function()
     {
     };
     ws.onmessage = function (evt) 
     { 
        var msg = evt.data;

        if (msg.indexOf("SHOW:") === 0) {
          Canvas.drawBoxes(eval(msg.substr(5)));
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
};

var noteFromKeyCode = function(key_code) {
  if (key_code >= 0 && key_code < key_mapping.length) {
    return key_mapping[key_code];
  } else {
    return null;
  }
};

var onKeyDown = function(event) {
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

var Canvas = {
  _canvas: null,

  init: function() {
    Canvas._canvas = document.getElementById("canvas");
  },

  draw: function(func) {
    var ctx = Canvas._canvas.getContext("2d");
    func(ctx);
  },

  clear: function(color_opt) {
    Canvas.draw(function(ctx) {
      ctx.fillStyle = color_opt || "#000000";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    });
  },

  drawBoxes: function(colors) {
    var n = colors.length;
    var box_width = Canvas._canvas.width / n;
    var box_height = Canvas._canvas.height;
    Canvas.draw(function(ctx) {
      ctx.lineWidth = 0;
      for (var i = 0; i < n; ++i) {
        ctx.fillStyle = colors[i];
        ctx.fillRect(i * box_width, 0, box_width + 1, box_height);
      }
    });
  }
};
