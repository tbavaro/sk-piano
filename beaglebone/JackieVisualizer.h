#ifndef __INCLUDED_JACKIE_VISUALIZER_H
#define __INCLUDED_JACKIE_VISUALIZER_H


#include "LightStrip.h"
#include "LightStripVisualizer.h"

class JackieVisualizer : public LightStripVisualizer {
public:
    JackieVisualizer(LightStrip& strip);
    ~JackieVisualizer();
	
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
    virtual void reset();
	
private:
    int pixelForKey(Key key);
	
    int* keys_hue;
    bool* keys_down;
	float* keys_age;
};

#endif
