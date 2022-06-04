#include "intagli/intagli.h"
#include "intagli/intagli_types.h"
#include "intagli/intagli_wayland.h"

void iglInitComponentFlags(iglComponentFlags *flags)
{
	flags->isEditable = false;
}

void iglInitComponentCallbacks(iglComponentCallbacks *callbacks)
{
	callbacks->onClick = NULL;
	callbacks->onLoad = NULL;
	callbacks->onMouseOver = NULL;
}

void iglInitComponentStyles(iglComponentStyles *styles)
{
	styles->backgroundColor = IGL_TRANSPARENT;
	styles->textColor = IGL_BLACK;
	styles->width = -1.0f;
	styles->height = -1.0f;
	styles->opacity = 1.0f;
	styles->padding = 0.0f;
	styles->paddingBottom = 0.0f;
	styles->paddingLeft = 0.0f;
	styles->paddingRight = 0.0f;
	styles->paddingTop = 0.0f;
}

void iglInitComponent(iglComponent *component)
{
	iglComponentCallbacks callbacks;
	iglComponentFlags flags;
	iglComponentStyles styles;

	iglInitComponentFlags(&flags);
	iglInitComponentCallbacks(&callbacks);
	iglInitComponentStyles(&styles);

	component->flags = flags;
	component->callbacks = callbacks;
	component->styles = styles;
}

iglComponent iglCreateComponent()
{
	iglComponent component;
	iglInitComponent(&component);
	return component;
}
