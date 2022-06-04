#include "intagli/intagli.h"
#include "intagli/intagli_wayland.h"

int main()
{
	struct iglWaylandWindow window = {0};
	iglWaylandInitWindow(&window);

	iglWaylandRender(&window);	

	iglWaylandDestroyWindow(&window);

	return 0;
}
