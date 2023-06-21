#include "GUIDefs.h"

namespace MYENGINE
{

	GUIAnchor::GUIAnchor()
	{
		x = GUIAnchorValue::LEFT;
		y = GUIAnchorValue::BOTTOM;
	}

	GUIAnchor::GUIAnchor(GUIAnchorValue _x, GUIAnchorValue _y)
	{
		x = _x;
		y = _y;
	}

	GUIAnchor::GUIAnchor(glm::vec2 _xy)
	{
		x = (GUIAnchorValue)_xy.x;
		y = (GUIAnchorValue)_xy.y;
	}

} // namespace MYENGINE
