//modify the code in line 57 to change the algorithm
#include <bits/stdc++.h>
using namespace std;
#define N 10005 //maximum number of textures
#define MIN 0x3f3f3f3f
//the structure to save a rectangle
struct Texture
{
	int w;	  //width
	int h;	  //height
	int posw; //the width axis
	int posh; //the height axis
};
//the structure to save a level
struct level
{
	int h;	//height of the level
	int rem_w;	//remaining width
	int tot_h;	//total height
};
//compare function
bool cmp(Texture a, Texture b){
    return a.h>b.h;
}
//First-Fit Decreasing Height algorithm
int FFDH(int width, int n);
//Next-Fit Decreasing Height algorithm
int NFDH(int width, int n);
//Best-Fit Decreasing Height algorithm
int BFDH(int width, int n);
vector<Texture> texture(N);
int main()
{
	int width, n;
	int i, j;

	//read in n and width
	freopen("data.in", "r", stdin);
	cin >> n >> width;
	START:
	for (i = 0; i < n; i++)
	{
		//input each texture
		cin >> texture[i].w >> texture[i].h;
		//if some rectangle's width is longer than the strip
		if (texture[i].w > width)
		{
			cout << "Error!";
			//reinput
			goto START;
		}
	}

	//sort the height in non-increasing order
	sort(texture.begin(), texture.end(), cmp);
	
	int min_height = BFDH(width, n);
	//output the minimum approximation height
	printf("minimum height=%d\n", min_height);

	//output the result of the packing
	for (i = 0; i < n; i++)
	{

		printf("%d\nwidth=%d\nheight=%d\npos.w=%d\npos.h=%d\n", i, texture[i].w, texture[i].h, texture[i].posw, texture[i].posh);
	}

	return 0;
}

//First-Fit Decreasing Height algorithm
int FFDH(int width, int n)
{
	int i, j;
	vector<level *> l;	//pointers to levels

	for (i = 0; i < n; i++)
	{
		int flag = 0;	//flag=1 if the texture should be added
		//check each level
		for (j = 0; j < l.size(); j++) 
		{
			//if the texture can be put into level j
			if (l[j]->rem_w >= texture[i].w) 
			{
				flag = 1;
				//calculate the position of texture in the strip
				texture[i].posw = width - l[j]->rem_w; 
				texture[i].posh = l[j]->tot_h;
				//update the remaining width
				l[j]->rem_w -= texture[i].w; 
				goto NEXT;
			}
		}
	NEXT:
		//create a new level
		if (!flag)
		{ 
			level *p = new level;
			//the new level's height=current texture's height
			p->h = texture[i].h;	
			//calculate the total height of the new level				  
			p->tot_h = (l.size() == 0) ? 0 : l[l.size() - 1]->tot_h + l[l.size() - 1]->h; 
			//update the remaining width
			p->rem_w = width - texture[i].w;
			//add p to l								
			l.push_back(p);													
			//calculate the position of new texture in the strip		 
			texture[i].posw = 0;										  
			texture[i].posh = p->tot_h;
		}
	}
	return ((l.size()==0)?0:l[l.size()-1]->tot_h+l[l.size()-1]->h);
}
//Next-Fit Decreasing Height algorithm
int NFDH(int width, int n)
{
	int i, j;
	vector<level *> l;	//pointers to levels

	for (i = 0; i < n; i++)
	{
		int flag = 0;	//flag=1 if the texture should be added
		//check the last level
		for (j=l.size()-1; j < l.size(); j++) 
		{
			//if the texture can be put into level j
			if (l[j]->rem_w >= texture[i].w) 
			{
				flag = 1;
				//calculate the position of texture in the strip
				texture[i].posw = width - l[j]->rem_w; 
				texture[i].posh = l[j]->tot_h;
				//update the remaining width
				l[j]->rem_w -= texture[i].w; 
				goto NEXT;
			}
		}
	NEXT:
		//create a new level
		if (!flag)
		{ 
			level *p = new level;
			//the new level's height=current texture's height
			p->h = texture[i].h;	
			//calculate the total height of the new level				  
			p->tot_h = (l.size() == 0) ? 0 : l[l.size() - 1]->tot_h + l[l.size() - 1]->h; 
			//update the remaining width
			p->rem_w = width - texture[i].w;
			//add p to l								
			l.push_back(p);													//calculate the position of new texture in the strip		 
			texture[i].posw = 0;										  
			texture[i].posh = p->tot_h;
		}
	}
	return ((l.size()==0)?0:l[l.size()-1]->tot_h+l[l.size()-1]->h);
}
//Best-Fit Decreasing Height algorithm
int BFDH(int width, int n)
{
	int i, j;
	vector<level *> l;	//pointers to levels
    //find the best fit level whose residual width is minimum
	for (i = 0; i < n; i++)
	{
		int min_num = -1;	//num of min level
		int min_w=MIN;	//min width
		//check each level
		for (j = 0; j < l.size(); j++) 
		{
			//if the texture can be put into level j
			if (l[j]->rem_w >= texture[i].w && (min_w>l[j]->rem_w - texture[i].w)) 
			{
				//update the min num and width
				min_num = j;
				min_w=l[j]->rem_w - texture[i].w; 
			}
		}
		//if there exists a best level
		if (min_num>=0)
		{ 
			//calculate the position of new texture in the strip		 
			texture[i].posw = width-l[min_num]->rem_w;
			texture[i].posh = l[min_num]->tot_h;
			//update the remaining width
			l[min_num]->rem_w-=texture[i].w;
		}
		//else create a new level
		else{
			level *p = new level;
			//the new level's height=current texture's height
			p->h = texture[i].h;	
			//calculate the total height of the new level				  
			p->tot_h = (l.size() == 0) ? 0 : l[l.size() - 1]->tot_h + l[l.size() - 1]->h; 
			//update the remaining width
			p->rem_w = width - texture[i].w;
			//add p to l								
			l.push_back(p);													//calculate the position of new texture in the strip		 
			texture[i].posw = 0;										  
			texture[i].posh = p->tot_h;
		}
	}
	return ((l.size()==0)?0:l[l.size()-1]->tot_h+l[l.size()-1]->h);
}
