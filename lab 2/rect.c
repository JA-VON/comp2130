#include <stdio.h>

struct point {
int x,y;
};
struct rect {
struct point ne,se,sw,nw;
};

int rectArea(struct rect mRect)
{
	int length = mRect.ne.x-mRect.nw.x;
	int width = mRect.ne.y-mRect.se.y;
	return length*width;
}

main()
{
	struct rect rect1;
	rect1.ne.x=34;
	rect1.ne.y=26;
	rect1.se.x=34;
	rect1.se.y=-12;
	rect1.nw.x=10;
	rect1.nw.y=26;
	rect1.sw.x=10;
	rect1.sw.y=-12;
	int area = rectArea(rect1);
	printf("The area is %d\n",area);
}
