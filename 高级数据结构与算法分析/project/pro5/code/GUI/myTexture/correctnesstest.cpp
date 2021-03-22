#include "correctnesstest.h"

CorrectnessTest::CorrectnessTest()
{
    Texturelst.clear();
}
CorrectnessTest::~CorrectnessTest(){
    Texturelst.clear();
}

bool CorrectnessTest::testTexture(int tw, int th, int tposw, int tposh){
    struct Texture tmpT;
    tmpT.w=tw;
    tmpT.h=th;
    tmpT.posw=tposw;
    tmpT.posh=tposh;
    tmpT.left=tposw;
    tmpT.right=tposw+tw;
    tmpT.top=tposh;
    tmpT.bottom=tposh+th;
    Texturelst.push_back(tmpT);
    if(tmpT.left<0||tmpT.top<0||tmpT.right>tot_width||tmpT.bottom>tot_height){
        return false;
    }
    return true;
}

//check whether r1, r2 includes
bool CorrectnessTest::isInclude(Texture &r1, Texture &r2)
{
    return (r1.posw < r2.posw && r1.posh < r2.posh && r2.right <= r1.right && r2.bottom <= r1.bottom) ||
           (r2.posw < r1.posw && r2.posh < r1.posh && r1.right <= r2.right && r1.bottom <= r2.bottom);
}
//check whether r1, r2  overlaps
bool CorrectnessTest::isRectOverlap(Texture &r1, Texture &r2)
{
    return !(((r1.right <= r2.left) || (r1.bottom >= r2.top)) ||
             ((r2.right <= r1.left) || (r2.bottom >= r1.top)));
}

bool CorrectnessTest::testTot(){
    for (int i = 0; i < Texturelst.size(); i++)
        for (int j = 0; j < i; j++)
            if (isRectOverlap(Texturelst[i], Texturelst[j]) || isInclude(Texturelst[i], Texturelst[j]))
            {
                return false;
            }

    return true;
}
