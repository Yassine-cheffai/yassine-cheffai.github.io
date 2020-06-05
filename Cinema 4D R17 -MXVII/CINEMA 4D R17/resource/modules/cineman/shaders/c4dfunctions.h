/***************************************************************************
 *  Copyright (C) 2008 MAXON Computer GmbH                                 *
 *                                                                         *
 *  These coded instructions, statements, and computer programs contain    *
 *  unpublished proprietary information of MAXON Computer GmbH,            *
 *  and are protected by copyright law. They  may  not be disclosed        *
 *  to third parties or copied or duplicated in any form, in whole or      *
 *  in part, without the prior written consent of MAXON Computer GmbH.     *
 *                                                                         *
 ***************************************************************************/

#ifndef C4DFUNCTIONS_H
#define C4DFUNCTIONS_H

#ifndef RI_INFINITY
#define RI_INFINITY 1.0e38
#endif

#ifndef RI_EPSILON
#define RI_EPSILON 1.0e-6
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef boxstep
#define boxstep(a,b,x) (clamp(((x)-(a))/((b)-(a)), 0, 1))
#endif


/* added for R12, this function converts a color in sRGB space to linear space */
color sRgbToLinear( color c; )
{
		return color ( ( comp( c, 0 ) <= 0.04045 ? ( comp( c, 0 ) / 12.92 ) : pow ( ( comp( c, 0 ) + 0.055 ) / ( 1.0 + 0.055 ), 2.4 ) ),
									 ( comp( c, 1 ) <= 0.04045 ? ( comp( c, 1 ) / 12.92 ) : pow ( ( comp( c, 1 ) + 0.055 ) / ( 1.0 + 0.055 ), 2.4 ) ),
									 ( comp( c, 2 ) <= 0.04045 ? ( comp( c, 2 ) / 12.92 ) : pow ( ( comp( c, 2 ) + 0.055 ) / ( 1.0 + 0.055 ), 2.4 ) ) );
}

/* added for R12, this function converts a color in linear space to sRGB space */
color linearToSrgb( color c; )
{
		return color ( ( comp( c, 0 ) <= 0.0031308 ? ( comp( c, 0 ) * 12.92 ) : ( 1.055 * pow( comp( c, 0 ), 1.0 / 2.4 ) - 0.055 ) ),
									 ( comp( c, 1 ) <= 0.0031308 ? ( comp( c, 1 ) * 12.92 ) : ( 1.055 * pow( comp( c, 1 ), 1.0 / 2.4 ) - 0.055 ) ),
									 ( comp( c, 2 ) <= 0.0031308 ? ( comp( c, 2 ) * 12.92 ) : ( 1.055 * pow( comp( c, 2 ), 1.0 / 2.4 ) - 0.055 ) ) );
}

point gradientspace( float space; point p; string textureSpace; )
{
		point pnoise;
		if ( space >= 6 ) { /* Raster Space */
				pnoise = transform( "NDC", p );
		} else if ( space >= 5) { /* Screen Space */
				pnoise = transform( "NDC", p );
		} else if ( space >= 4) { /* Camera Space */
				pnoise = transform( "camera", p );
		} else if ( space >= 3) { /* World Space */
				pnoise = transform( "world", p );
		} else if ( space >= 2) { /* Object Space */
				/*pnoise = transform( "object", p );*/
				pnoise = transform( "shader", p );
		} else { /* Texture Space */
				pnoise = transform( textureSpace, p ) * 100;
		}
		return pnoise;
}

float sqr( float f )
{
		return f * f;
}

/* Return the average value of the color components */
float avg( color c )
{
		return ( comp( c, 0 ) + comp( c, 1 ) + comp( c, 2 ) ) / 3;
}

/* Return the color component with minimum value */
float minComp( color c )
{
		return min( comp( c, 0 ), comp( c, 1 ), comp( c, 2 ) );
}

/* Return the color component with maximum value */
float maxComp( color c )
{
		return max( comp( c, 0 ), comp( c, 1 ), comp( c, 2 ) );
}

/* do c4d style genlocking */
float genlock( color sample, col, delta )
{
		float result = 1;
		color upper_bound = col + delta;
		color lower_bound = col - delta;
		if ( ( comp( sample, 0 ) < comp( upper_bound, 0 ) && comp( sample, 0 ) > comp( lower_bound, 0 ) ) ||
				 ( comp( sample, 1 ) < comp( upper_bound, 1 ) && comp( sample, 1 ) > comp( lower_bound, 1 ) ) ||
				 ( comp( sample, 2 ) < comp( upper_bound, 2 ) && comp( sample, 2 ) > comp( lower_bound, 2 ) ) ) {
				result = 0;
		}
		return result;
}

color lowclip( color c )
{
	color value = c;
	if ( comp (c, 0) < 0) { setcomp( value, 0, 0 ); }
	if ( comp (c, 1) < 0) { setcomp( value, 1, 0 ); }
	if ( comp (c, 2) < 0) { setcomp( value, 2, 0 ); }
	return value;
}

color c4dfilter( color col; float hue; float saturation; float lightness; float colorize; float brightness; float contrast_in; float lowclip; float highclip; float clipping; float gamma_in )
{
	color ret = ctransform( "rgb", "hsv", col );
	float icomp;
	float gamma = gamma_in;
	float contrast = contrast_in;
	extern uniform float ncomps;

		if ( colorize == 0 ) {
				setcomp( ret, 0, mod( comp( ret, 0 ) + hue / ( PI * 2 ), 1.0 ) );
				setcomp( ret, 1, clamp( comp( ret, 1 ) + saturation, 0, 1 ) );
#pragma nolint
				ret = ctransform( "hsv", "rgb", ret );
	} else {
				setcomp( ret, 0, hue / ( PI * 2 ) );
				setcomp( ret, 1, saturation );
#pragma nolint
		}

		if ( lightness >= 0 ) {
				ret = mix( ret, color( 1 ), lightness );
	} else {
				ret = mix( ret, color( 0 ), -lightness );
	}

		/* Change Brightness Contrast */

		if ( clipping != 0 ) {
				ret = clamp( ret, color( 0 ), color( 1 ) );
				setcomp( ret, 0, boxstep( lowclip, highclip, comp( ret, 0 ) ) );
				setcomp( ret, 1, boxstep( lowclip, highclip, comp( ret, 1 ) ) );
				setcomp( ret, 2, boxstep( lowclip, highclip, comp( ret, 2 ) ) );
		}

		if ( gamma != 1.0 ) {
				gamma = 1.0 / gamma;
				setcomp( ret, 0, pow( comp( ret, 0 ), gamma ) );
				setcomp( ret, 1, pow( comp( ret, 1 ), gamma ) );
				setcomp( ret, 2, pow( comp( ret, 2 ), gamma ) );
		}

		ret += color( brightness );
		if ( contrast == 1 ) {
				contrast = .9999;
		}

		for ( icomp = 0; icomp < ncomps; icomp += 1 ) {
				setcomp( ret, icomp, ( comp( ret, icomp ) - .5 ) / ( 1 - contrast ) + .5 );
				if ( comp( ret, icomp ) < 0.0 ) setcomp( ret, icomp, 0 );
		}

		if ( clipping != 0 ) {
				ret = clamp( ret, color( 0 ), color( 1 ) );
		}

		return ret;
}



#endif
