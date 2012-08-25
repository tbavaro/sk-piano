#include "JackieVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <math.h>
#include <string.h>

JackieVisualizer::JackieVisualizer(LightStrip& strip) : LightStripVisualizer(strip) {
	keys_down = new bool[88];
	keys_age = new float[88];
	keys_hue = new int[88];
	this->reset();
}

JackieVisualizer::~JackieVisualizer() {
	delete[] keys_down;
	delete[] keys_age;
	delete[] keys_hue;
}

// Returns the center key of the block
int JackieVisualizer::pixelForKey(Key key) {
	int offset = strip.numPixels()/(88 * 2);
	return ((int)key / 88.0 * strip.numPixels()) + offset;
}

// When a key is pressed: mark it, set the age to 0, pick a random hue
void JackieVisualizer::onKeyDown(Key key) {
	keys_down[key] = true;
	keys_age[key] = 0;
	keys_hue[key] = Util::random(360);
}

// When a key is released: unmark it
void JackieVisualizer::onKeyUp(Key key) {
	keys_down[key] = false;
}

void JackieVisualizer::onPassFinished(bool something_changed) {
	// increase the age of all the pressed down keys, decrease for raised
	for (int i = 0; i < 88; ++i) {
		if (keys_down[i]) {
			++keys_age[i];
		} else if (keys_age[i] > 0) {
			keys_age[i] = keys_age[i] * 0.9 - 0.5;	//decrease faster than increase
			keys_age[i] = ((keys_age[i] > .8) ? keys_age[i] : 0);
		}
	}
	
	strip.reset();
	
	// Draw the appropriate sized bar for each key
	for (int i =0; i < 88; ++i) {
		// Any key not at 0 should be drawn
		if (keys_age[i] > 0) {
			int center_pixel = pixelForKey(i);
			// Color the center pixel
			strip.addPixel(center_pixel, Colors::hsv(keys_hue[i], 1.0, Util::min(1.0f,keys_age[i])));
			
			// For each age, add a bar left and right
			float radius = pow(keys_age[i]*1.5, .9) ;
			for (int j = 1; j < radius; ++j) {
				if (center_pixel - j >= 0) {
					strip.addPixel(center_pixel - j, Colors::hsv(keys_hue[i], 1.0, pow(1-j/radius,0.9)));
				}
				if (center_pixel + j < strip.numPixels()) {
					strip.addPixel(center_pixel	+j, Colors::hsv(keys_hue[i], 1.0, pow(1-j/radius,0.9)));
				}
			}
		}
	}
}

void JackieVisualizer::reset() {
	LightStripVisualizer::reset();
	bzero(keys_down, 88 * sizeof(bool));
	bzero(keys_age, 88 * sizeof(float));
	bzero(keys_hue, 88 * sizeof(int));
}

