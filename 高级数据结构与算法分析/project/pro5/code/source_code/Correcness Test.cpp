#include <bits/stdc++.h>
using namespace std;
#define N 10005
struct Texture
{
    int w, h, posw, posh, left, right, top, bottom;
} texture[N];
//check whether r1, r2 includes
bool isInclude(Texture &r1, Texture &r2)
{
    return (r1.posw < r2.posw && r1.posh < r2.posh && r2.right <= r1.right && r2.bottom <= r1.bottom) ||
           (r2.posw < r1.posw && r2.posh < r1.posh && r1.right <= r2.right && r1.bottom <= r2.bottom);
}
//check whether r1, r2  overlaps
bool isRectOverlap(Texture &r1, Texture &r2)
{
    return !(((r1.right <= r2.left) || (r1.bottom >= r2.top)) ||
             ((r2.right <= r1.left) || (r2.bottom >= r1.top)));
}

int main()
{
    int i,j;
    int width, height, n;
    freopen("in.txt", "r", stdin);
    //input
    cin >> n;  
    cin >> width >> height;          
    for (i = 0; i < n; i++)
    {
        //input the position and size of each texture
        cin >> texture[i].w >> texture[i].h >> texture[i].posw >> texture[i].posh; 
        texture[i].left = texture[i].posw;
        texture[i].right = texture[i].posw + texture[i].w;
        texture[i].top = texture[i].posh;
        texture[i].bottom = texture[i].posh + texture[i].h;

        //check whether the texture cross the boarder
        if (texture[i].left < 0 || texture[i].top < 0 || texture[i].right > width || texture[i].bottom > height)
        {
            cout << "Error!";
            return 0;
        }
    }
    //check whether there exists a pair of texture that overlaps or include one another
    for (i = 0; i < n; i++)
        for (j = 0; j < i; j++)
            if (isRectOverlap(texture[i], texture[j]) || isInclude(texture[i], texture[j]))
            {
                cout << "Error!!";
                return 0;
            }

    //true answer if not return 0
    cout << "True!!!";

    return 0;
}