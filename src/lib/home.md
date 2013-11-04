# Piano API #

## Overview ##

[Visualizer](../../classes/Visualizer.html)s are defined as CoffeeScript
classes and are instantiated when needed.

### Environment ###

All of the library classes are preloaded into the environment and ready for use;
there should be no need to add `require` calls.

### Instantiation ###

Visualizer implementations are instantiated by the framework code passing two
parameters into the constructor:

1. a [LightStrip](../../classes/LightStrip.html) object representing all of the
   physical LEDs on the piano
2. a [PianoKeys](../../classes/PianoKeys.html) object which provides access to
   reading the state of the piano keys.

### Statefulness ###

Visualizers are expected to be stateful, and can be `reset` at any time.  When
switching from one visualizer to the next the
[Visualizer](../../classes/Visualizer.html) object will be discarded along with
any state contained within.  However, it is not possible to discard or undo
changes made to global objects (including library objects) so don't do that.

### Piano Key Reading Accuracy ###

The piano hasn't had its wiring fixed up in quite some time, so some of the keys
don't register properly or even at all.  This isn't super critical to achieve
the overall effect, but does restrict the scope of what we can do with
fine-grained interactivity at the moment.  It's not actually that difficult to
do; if you're handy with a soldering iron and want to help out, let Tim or David
know.

### Magic ###

For the most part this is straightforward CoffeeScript code, though there is a
small amount of magic involved to help simplify writing visualizers and reduce
boilerplate code.  If something seems screwy around compilation or parsing your
code, that might be the culprit &mdash; let Tim know.
