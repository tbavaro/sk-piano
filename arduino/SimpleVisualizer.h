#include "LPD8806.h"
#include "Visualizer.h"

class SimpleVisualizer : public Visualizer {
  public:
    SimpleVisualizer(LPD8806* strip);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onPassFinished(bool something_changed);

  private:
    LPD8806* strip;
};
