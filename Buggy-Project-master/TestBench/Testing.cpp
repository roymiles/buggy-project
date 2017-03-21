#include "Testing.h"

Testing::Testing()
{
	pf = new PathFinding(MAZE_X_MAX, MAZE_X_MAX, "*#*#*#*#A0507A0604D0206B0001D1302D1000A1307*0003*0303*0405*0705*1107*1104*0600*1317*#", Vector(0, 0));
}


Testing::~Testing()
{
}

int main() {
	Testing *test = new Testing();
}
