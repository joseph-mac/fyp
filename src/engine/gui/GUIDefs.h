#ifndef _MYENGINE_GUIDEFS_H_
#define _MYENGINE_GUIDEFS_H_

#include <glm/vec2.hpp>

#include <vector>

namespace MYENGINE
{

	enum class GUIAnchorValue
	{
		BOTTOM = 0,
		MIDDLE = 1,
		TOP = 2,

		LEFT = 0,
		CENTRE = 1,
		RIGHT = 2,
	};

	struct GUIAnchor
	{
		GUIAnchor();
		GUIAnchor(GUIAnchorValue _x, GUIAnchorValue _y);
		GUIAnchor(glm::vec2 _xy);

		GUIAnchorValue x = GUIAnchorValue::LEFT;
		GUIAnchorValue y = GUIAnchorValue::TOP;
	};

	const std::vector<float> GUIAnchorValues = { 0.0f, 0.5f, 1.0f };

} // namespace MYENGINE

#endif
