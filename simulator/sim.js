var WEBSOCKETS_URL = "ws://localhost:8001/piano";

var console;

var log = function(text) {
  console.appendChild(document.createTextNode(text));
  console.appendChild(document.createElement("BR"));
  console.scrollTop = console.scrollHeight;
};

var onLoad = function() {
  Canvas.init();
  console = document.getElementById("console");

  log("Connecting to " + WEBSOCKETS_URL + " ...");
  Canvas.clear();
  Canvas.drawBoxes([ "red", "green", "blue" ]);

  var ws = new WebSocket(WEBSOCKETS_URL);
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
     };
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
