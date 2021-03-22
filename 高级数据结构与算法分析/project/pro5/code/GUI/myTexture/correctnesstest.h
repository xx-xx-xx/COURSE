#ifndef CORRECTNESSTEST_H
#define CORRECTNESSTEST_H
#include<bits/stdc++.h>

class CorrectnessTest
{
public:
    CorrectnessTest();
    ~CorrectnessTest();
    bool testTexture(int,int,int,int);
    int tot_width,tot_height;
    bool testTot();
    struct Texture
    {
        int w, h, posw, posh, left, right, top, bottom;
    };
    std::vector<struct Texture>Texturelst;
private:
    bool isInclude(Texture &r1, Texture &r2);
    bool isRectOverlap(Texture &r1, Texture &r2);
};

#endif // CORRECTNESSTEST_H
