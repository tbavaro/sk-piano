#ifndef __INCLUDED_IDLE_VISUALIZER_WRAPPER_H
#define __INCLUDED_IDLE_VISUALIZER_WRAPPER_H

#include "Visualizer.h"

class IdleVisualizerWrapper : public Visualizer {
  public:
    IdleVisualizerWrapper(
        Visualizer& delegate,
        float idle_timeout,
        float random_key_rate);

    virtual void reset();
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    Visualizer& delegate;
    uint32_t prev_key_time;
    uint32_t idle_timeout_ms;
    float random_key_rate;
};

#endif

