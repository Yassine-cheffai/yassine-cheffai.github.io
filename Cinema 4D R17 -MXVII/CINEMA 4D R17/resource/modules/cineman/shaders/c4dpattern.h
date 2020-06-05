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

#ifndef C4DPATTERN_H
#define C4DPATTERN_H

#include "c4dfunctions.h"
#include "c4dnoise.h"

/*
** WOOD
**
** Recreates the pattern of the CINEMA 4D Wood Shader. It takes the
** Wood Parameters and the position of the lookup as input, the output
** is a value between 0 and 1 as a lookup into the color Gradient.
*/
float patternWood( point p_in; vector scale; float turbulence )
{
	#define DIST_THRESH 0.15

	float samplerad, octaves, len;
	point p = p_in * scale;

	len = sqrt( comp(p, 0)*comp(p, 0) + comp(p, 1)*comp(p, 1) );
	if (len > 0) {
		float fak, xf, ll;

		samplerad = 0;/*d*data->scale;*/
		if (samplerad != 0.0) { octaves = DIST_THRESH / samplerad; } else { octaves = 100.0; }
		if (octaves > 5.0) { octaves = 5.0; }

				ll = c4dturbulence3d( point( acos( comp( p, 0 ) / len ) * 2, comp( p, 2 ) * 0.25 + 0.5, 0.0 ), 665, octaves, FALSE );
		len *= 15.0 + ll * 2.25 * turbulence;

		fak = samplerad * 50.0;
		xf = floor(len);
		len = smoothstep(xf - fak, xf + 1.0 + fak, len);

		fak = samplerad * 4.0;
		fak = fak * fak;

		len = mix( len, 0.5, min( fak, 1 ));
	}

	return len;
}


/*
** FIRE
**
** Recreates the pattern of the CINEMA 4D Fire Shader. It takes the
** Fire Parameters, the time and the position of the lookup as input,
** the output is a value between 0 and 1 as a lookup into the
** color Gradient.
*/
float patternFire( point p_in; float scaleX; float scaleY; float frequency; float turbulence; float seconds_in; )
{
	float octaves = 5, chaos, h, seconds = seconds_in;
	point p = p_in;

	seconds *= frequency;
	setxcomp( p,  xcomp( p ) * scaleX * 4 );
	if ( ycomp( p ) < 0.0) {
		setycomp( p, 0 );
		}
	setycomp( p, exp( -ycomp( p ) ) );

	chaos = abs( c4dfbm3d( point( comp( p, 0 ), comp( p, 1 ) + seconds, 0 ), 665, octaves, 1.87, 1.0 ) );
	h = 1.0 - 0.8 * comp( p, 1 );

	return 1.0 - clamp( 0.85 * chaos * turbulence + h, 0, 1);
}


/*
** FLAME
**
** Recreates the pattern of the CINEMA 4D Flame Shader.
*/
float patternFlame( point p_in; float frequency; float turbulence; float seconds_in; )
{
	float chaos, x, val, octaves = 6, seconds = seconds_in;
	point p = p_in;
	point pp;

	setxcomp( p, xcomp( p ) - 0.5 );
	setycomp( p, 1.0 - ycomp( p ) );

		seconds *= frequency * 0.5;

		pp = point( xcomp( p ) * 6.4, ( ycomp( p ) - seconds ) * 3.2, 0 );

		chaos = c4dturbulence3d( pp, 665, octaves, 1) * 0.3 * turbulence;

		x = 1.0 - 2.0 * abs( xcomp( p ) );
		val = x * x * ( 1 - ycomp( p ) + chaos );

		return clamp( val, 0, 1 );
}


/*
** METAL
**
** Recreates the pattern of the CINEMA 4D Metal Shader.
*/
float patternMetal( point p; float frequency; )
{
	float octaves = 6, fak = 0;

	fak = clamp(octaves - 0.4, 0.0, 1.0);
	fak *= c4dturbulence3d(p * ( frequency * 2 ), 665, octaves, 0 ) * .25 + .5;

	return fak;
}


/*
** SATURNRING
**
** Recreates the pattern of the CINEMA 4D Saturn Ring Shader.
*/
float integral( float x; float alpha )
{
	float d, value = 0;
	float i;

	color ringtab[6] =
	{
		color( 0.186, 0.021,  0.10 ), // D-Ring
		color( 0.207, 0.048,  0.25 ), // C-Ring
		color( 0.255, 0.071,  0.65 ), // B-Ring
		color( 0.339, 0.040,  0.60 ), // A-Ring
		color( 0.389, 0.0014, 0.15 ), // F-Ring
		color( 0.460, 0.022,  0.05 )  // G-Ring
	};

	for ( i = 0; i < 6; i += 1 ) {
		if ( x < comp( ringtab[i], 0 ) )  { break; }

		d = comp( ringtab[i], 0 ) + comp( ringtab[i], 1 );
		if ( x < d ) d = x;

		if ( alpha > 0.5 ) {
			value += ( d - comp( ringtab[i], 0 ) );
		} else {
			value += comp( ringtab[i], 2 ) * ( d - comp( ringtab[i], 0 ) );
		}
	}
	return value;
}

color patternSaturnRing( float ss_in; float tt_in; color col_in; float alpha )
{
	float r, mmin, mmax, rad;
	float ss = ss_in, tt = tt_in;
	color result = 0;

	ss  -= 0.5;
	tt  -= 0.5;
	r   = sqrt( sqr( ss ) + sqr( tt ) );

	rad = 0.0001;

	mmin = r - rad;
	mmax = r + rad;

	if ( mmin > 0.5 || mmax < 0 ) {
		result = 0;
	} else {
		if ( mmin < 0 ) mmin = 0;

		if ( alpha > 0.5 ) {
			result = ( integral( mmax, 1) - integral( mmin, 1 ) ) / ( mmax - mmin );
		} else {
			result = ( ( integral( mmax, 0 ) - integral( mmin, 0 ) ) / ( mmax - mmin ) ) * col_in;//color( 0.730, 0.598, 0.266 );
		}
	}
	return result;
}


/*
** NEPTUNE
**
** Recreates the pattern of the CINEMA 4D Neptune Shader.
*/
color patternNeptune( float ss_in; float tt_in; color col_in )
{
	float octaves = 6, h, f;
	float ss = ss_in, tt = tt_in;

	f = c4dturbulence3d( point( ss * 20, tt * 20, 0 ), 665, octaves, 0) * PI * 2;

	ss += 0.003 * cos( f );
	tt += 0.003 * sin( f );

	h = c4dturbulence3d( point( tt * 20, 0, 0 ), 665, octaves, 1) * 0.1 + 0.9;
	return h * col_in; //color( 0, 0.25, 0.25 );
}


/*
** SATURN
**
** Recreates the pattern of the CINEMA 4D Saturn Shader.
*/
color patternSaturn( float ss_in; float tt_in; color col_in; )
{
	float octaves = 6, h, f;
	float ss = ss_in, tt = tt_in;

	f = c4dturbulence3d( point( ss * 20, tt * 20, 0 ), 665, octaves, 0) * PI * 2;

	ss += 0.003 * cos( f );
	tt += 0.003 * sin( f );

	h = c4dturbulence3d( point( tt * 20, 0, 0 ), 665, octaves, 1) * 0.4 + 0.6;
	return h * col_in;//color( 0.730, 0.598, 0.266);
}


/*
** URANUS
**
** Recreates the pattern of the CINEMA 4D Uranus Shader.
*/
color patternUranus( float ss_in; float tt_in; color col_in )
{
	float octaves = 6, h, f;
	float ss = ss_in, tt = tt_in;

	f = c4dturbulence3d( point( ss * 20, tt * 20, 0 ), 665, octaves, 0) * PI * 2;

	ss += 0.003 * cos( f );
	tt += 0.003 * sin( f );

	h = c4dturbulence3d( point( tt * 20, 0, 0 ), 665, octaves, 1 ) * 0.3 + 0.7;
	return h * col_in;//color( 0, 0.2, 0.4 );
}


/*
** RUST
**
** Recreates the pattern of the CINEMA 4D Rust Shader.
*/
float patternRust( point p; float level; float frequency; )
{
	float octaves = 6;

	float fak = ( octaves - .4 ) *  .4;
	fak = clamp( fak, 0, 1);
	fak = fak * ( 2 - fak);

	return clamp( 2 * ( level * 2.5 - 1.03 + fak * c4dturbulence3d( p * ( frequency * 2.0 ), 665, octaves, 0 ) ), 0, 1 );
}


/*
** SUNBURST
**
** Recreates the pattern of the CINEMA 4D Sunburst Shader.
*/
float patternSunburst( float in_ss; float in_tt; float scaler; float scalea; float turbulence; float frequency; float radius; float height; float seconds_in; )
{
		float seconds = seconds_in;
	float r, w, chaos, drad, h = 0.0, fak, samplerad, octaves, rad = radius * 0.5, width = height;
	point pp;

		float ss = in_ss - 0.5;
		float tt = in_ss - 0.5;

	drad = 0.0001;

	r = sqrt( ss * ss + tt * tt );
	if ( r + drad > rad && r < rad + 3.0 * width ) { /* PROBLEM: Color 0 */

		if ( r - drad >= rad ) {
			fak = 1.0;
		} else {
			fak = ( r + drad - rad ) / ( 2 * drad );
		}

		if (r == 0) {
			w = 0;
		} else if ( tt > 0 ) {
			w = acos( ss / r );
		} else {
			w = PI * 2 - acos( ss / r );
		}

		if ( w > PI ) {
			w = PI * 2 - w;
		}

		w       *= scalea / ( PI * 2 );
		seconds *= frequency * 0.02;
		r       -= rad;

		octaves = 6;

		setxcomp( pp, w * 64 );
		setycomp( pp, ( ( r * scaler * 0.1 ) - seconds ) * 64 );
		setzcomp( pp, 0 );
		chaos = abs( c4dfbm3d( pp, 665, octaves, 1.87, 1 ) + tt);

		h = 1.0 - r * 3 * ( 1 + chaos * turbulence ) / width;
	}

	return clamp( h, 0, 1 );
}


/*
** BRICK
**
** Recreates the pattern of the CINEMA 4D Brick Shader.
*/
float patternBrickMidVal(float val; float in_delta; float joint; float ramp)
{
	float delta = in_delta, width, maxx, minx, minv, maxv, ig;

	delta = clamp( delta, 0.0001, 0.5 );

	width = joint + ramp;
	maxx  = val + delta;
	minx  = val - delta;

	/* positive red area */
	minv = minx;
	maxv = maxx;
	if ( minv < width ) minv = width;
	if ( maxv < width ) maxv = width;
	ig = maxv - minv;

	/* negative red area */
	minv = minx;
	maxv = maxx;
	if ( minv > -width ) minv = -width;
	if ( maxv > -width ) maxv = -width;
	ig += maxv - minv;

	/* mortar */
	if (ramp > 0.0)
	{
		minv = clamp( minx, joint, width );
		maxv = clamp( maxx, joint, width );
		ig += ( ( maxv * maxv - minv * minv ) * 0.5 + joint * ( minv - maxv ) ) / ramp;

		maxv = -clamp( minx, -width, -joint );
		minv = -clamp( maxx, -width, -joint );
		ig += ( ( maxv * maxv - minv * minv ) * 0.5 + joint * ( minv - maxv ) ) / ramp;
	}

	return ig / (2.0 * delta);
}


#endif /* #ifndef C4DPATTERN_H */
