#ifndef _intagli_intagli_h_
#define _intagli_intagli_h_

#include "intagli/internal.h"
#include "intagli/intagli_types.h"
#include "intagli/intagli_definitions.h"
#include <stdbool.h>

typedef struct iglComponentStyles
{
	float width;
	float height;
	float padding;
	float paddingTop;
	float paddingBottom;
	float paddingLeft;
	float paddingRight;
	vec4 textColor;
	vec4 backgroundColor;
	float opacity;
} iglComponentStyles;

typedef struct iglComponentFlags
{
	bool isEditable;
} iglComponentFlags;

typedef void (*iglComponentCallbackFunc)();

typedef struct iglComponentCallbacks
{
	iglComponentCallbackFunc onClick;
	iglComponentCallbackFunc onLoad;
	iglComponentCallbackFunc onMouseOver;
} iglComponentCallbacks;

typedef struct iglComponent
{
	iglComponentStyles styles;
	iglComponentFlags flags;
	iglComponentCallbacks callbacks;
} iglComponent;

void iglInitComponentCallbacks(iglComponentCallbacks* callbacks);
void iglInitComponentFlags(iglComponentFlags* flags);
void iglInitComponentStyles(iglComponentStyles* styles);
void iglInitComponent(iglComponent* component);

iglComponent iglCreateComponent();

#endif
