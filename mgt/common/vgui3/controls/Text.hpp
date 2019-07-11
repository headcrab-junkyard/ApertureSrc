/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1999-2005 Id Software, Inc.
 * Copyright (C) 2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief text widget

#pragma once

struct menutext_s : public MenuElement
{
public:
	Text() : MenuElement(Type::Text){}
	
	void Init();
	
	void Draw() override;

	char*			string;
	int				style;
	float*			color;
};

// proportional text widget
struct ProportionalText : public menutext_s
{
public:
	ProportionalText() : Text(){}
	
	void Init();
	
	void Draw() override;
};

/*
=================
PText_Init
=================
*/
void ProportionalText::Init( menutext_s *t )
{
	int	x;
	int	y;
	int	w;
	int	h;
	float	sizeScale;

	sizeScale = UI_ProportionalSizeScale( t->style );

	x = t->generic.x;
	y = t->generic.y;
	w = UI_ProportionalStringWidth( t->string ) * sizeScale;
	h =	PROP_HEIGHT * sizeScale;

	if( t->generic.flags & QMF_RIGHT_JUSTIFY ) {
		x -= w;
	}
	else if( t->generic.flags & QMF_CENTER_JUSTIFY ) {
		x -= w / 2;
	}

	t->generic.left   = x - PROP_GAP_WIDTH * sizeScale;
	t->generic.right  = x + w + PROP_GAP_WIDTH * sizeScale;
	t->generic.top    = y;
	t->generic.bottom = y + h;
};

/*
=================
PText_Draw
=================
*/
void ProportionalText::Draw( menutext_s *t )
{
	int		x;
	int		y;
	float *	color;
	int		style;

	x = t->generic.x;
	y = t->generic.y;

	if (t->generic.flags & QMF_GRAYED)
		color = text_color_disabled;
	else
		color = t->color;

	style = t->style;
	if( t->generic.flags & QMF_PULSEIFFOCUS ) {
		if( Menu_ItemAtCursor( t->generic.parent ) == t ) {
			style |= UI_PULSE;
		}
		else {
			style |= UI_INVERSE;
		}
	}

	UI_DrawProportionalString( x, y, t->string, style, color );
};

// proportional banner text widget
struct ProportionalBannerText : public menutext_s
{
	void Init();
	
	void Draw() override;
};

/*
=================
BText_Init
=================
*/
void ProportionalBannerText::Init()
{
	self->flags |= QMF_INACTIVE;
};

/*
=================
BText_Draw
=================
*/
void ProportionalBannerText::Draw( menutext_s *t )
{
	int		x;
	int		y;
	float*	color;

	x = t->generic.x;
	y = t->generic.y;

	if (t->generic.flags & QMF_GRAYED)
		color = text_color_disabled;
	else
		color = t->color;

	UI_DrawBannerString( x, y, t->string, t->style, color );
};