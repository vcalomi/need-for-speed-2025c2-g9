class Explosion {
public:
    float x, y;
    float size;
    float frameTime = 0.03f;
    float elapsed = 0.0f;
    int frame = 0;
    bool finished = false;

    Explosion(float x, float y, float size): x(x), y(y), size(size) {}
};
