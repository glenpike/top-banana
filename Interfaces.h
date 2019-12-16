#ifndef INTERFACES_H
#define INTERFACES_H

enum Animation {
    OFF,
    ON,
    FLASH,
    RAINBOW_CYCLE,
    THEATER_CHASE,
    COLOR_WIPE,
    SCANNER,
    FADE
};
enum Direction { FORWARD, REVERSE };

struct AnimationConfig {
    Animation animation;
    uint32_t updateInterval;
    uint16_t steps;
    Direction direction;
    uint32_t color1;
    uint32_t color2;
};

struct CallBackHandler {
    virtual void OnComplete(void *p);
};

struct AbstractPlayer {
  virtual bool isRunning();
  virtual bool isAnimationComplete();
  virtual void Update() = 0;
  virtual void Start();
  virtual void Pause();
  virtual void Reset();
  virtual void SetCallback(CallBackHandler *handler);
};

struct AbstractAnimateable : AbstractPlayer  {
  virtual void SetPattern(AnimationConfig *config);
};

class CallbackWrapper : public CallBackHandler {
  public:
    void (*OnCompleteHandler)(); // Callback on completion of pattern

    CallbackWrapper(void (*callback)()) {
        OnCompleteHandler = callback;
    }

    void OnComplete(void *p) {
      if (OnCompleteHandler != NULL) {
          OnCompleteHandler();
      }
    }
};

#endif
