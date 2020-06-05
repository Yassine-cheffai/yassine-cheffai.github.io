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

#ifndef C4DNOISE_H
#define C4DNOISE_H


#ifndef boxstep
#define boxstep(a,b,x) (clamp(((x)-(a))/((b)-(a)),0,1))
#endif


point add( point p1; point p2; )
{
	point ret;
	setxcomp( ret, xcomp( p1 ) + xcomp( p2 ) );
	setycomp( ret, ycomp( p1 ) + ycomp( p2 ) );
	setzcomp( ret, zcomp( p1 ) + zcomp( p2 ) );
	return ret;
}

float c4dfbm3d(point p_in; float seed; float octaves; float lacunarity; float h; )
{
	float rt = 0.0, val;
	float table = lacunarity;
	float frequency = 1.0;
	float oct = octaves;
	point p = p_in;

	if ( oct > 12.0 ) oct = 12.0;

	while ( oct > 1.0 ) {
		table = pow( frequency, -h) ;
		frequency *= lacunarity;

		val  = c4dnoise( seed, p );
		rt  += val * table;
		oct -= 1.0;
		p   *= lacunarity;
	}

	if ( oct > 0.0 ) {
		table = pow( frequency, -h );
		frequency *= lacunarity;

		val  = c4dnoise( seed, p );
		val *= oct;
		rt  += val * table;
	}

	return rt;
}

float c4dfbm4d(point p_in; float seconds; float seed; float octaves; float lacunarity; float h; )
{

	float rt = 0.0, val;
	float table = lacunarity;
	float frequency = 1.0;
	float oct = octaves;
	float t = seconds;
	point p = p_in;

	if ( oct > 12.0 ) oct = 12.0;

	while ( oct > 1.0 ) {
		table = pow( frequency, -h );
		frequency *= lacunarity;

		val  = c4dnoise( seed, p, t );
		rt  += val * table;
		oct -= 1.0;
		p   *= lacunarity;
		t   *= lacunarity;
	}

	if ( oct > 0.0 ) {
		table = pow( frequency, -h );
		frequency *= lacunarity;

		val = c4dnoise( seed, p, t );
		val *= oct;
		rt  += val * table;
	}
	return rt;
}

float c4dridgedmultifractal( point p_in; float seed; float octaves; float offset; float gain; float lacunarity; float h; )
{
	float result = 0.0;
	float signal, weight;

	float table = lacunarity;
	float frequency = 1.0;

	float oct = octaves;
	point p = p_in;

	if ( oct > 12.0 ) oct = 12.0;

		// get first octave
		signal = abs( c4dnoise( seed, p ) );

	// invert and translate (note that "offset" should be ~= 1.0)
	signal = offset - signal;

		// square the signal, to increase "sharpness" of ridges
		signal *= signal;

	// assign initial values
		result = signal;
		weight = 1.0;

	if ( oct < 1.0 ) result *= oct;

	oct-=	1.0;
		while ( oct > 0.0 )	{
		table = pow( frequency, -h );
		frequency *= lacunarity;

				// increase the frequency
				p *= lacunarity;

				// weight successive contributions by previous signal
				weight = signal * gain;
				weight = clamp( weight, 0.0, 1.0 );

			signal = abs( c4dnoise( seed, p ) );
				signal = offset - signal;
				signal *= signal;

				// weight the contribution
				signal *= weight;

		if (oct < 1.0) { signal *= oct; }
				result += signal * table;
		oct -= 1.0;
	}

	return result;
}


float c4dturbulence3d( point p_in; float seed; float octaves; float abs; )
{
	float rt = 0, v = 1, val;
	float oct = octaves;
	point p = p_in;

	if ( abs != 0 ) {
		while ( oct > 1 ) {
			val = c4dnoise( seed, p );
			rt  += abs( val ) * v;
			oct -= 1.0;
			v *= 0.5;
			p = add( p, p ); /* p += p; */
		}
		if ( oct > 0 ) {
			val = c4dnoise( seed, p );
			val *= oct;
			rt  += abs( val ) * v;
		}
	} else {
		while ( oct > 1 ) {
			val = c4dnoise( seed, p );
			rt  += val * v;
			oct -= 1.0;
			v *= 0.5;
			p = add( p, p ); /* p += p; */
		}
		if ( oct > 0 ) {
			val = c4dnoise( seed, p );
			val *= oct;
			rt += val * v;
		}
	}
	return rt;
}

float c4dturbulence( point p_in; float seconds; float seed; float octaves; float abs; )
{
	float rt = 0, v = 1, val;
	float oct = octaves;
	point p = p_in;

	if ( abs != 0 ) {
		while ( oct > 1 ) {
			val  = c4dnoise( seed, p, seconds );
			rt  += abs( val ) * v;
			oct -= 1.0;
			v   *= 0.5;
			p    = add( p, p ); /* p += p; */
		}
		if ( oct > 0 ) {
			val  = c4dnoise( seed, p, seconds );
			val *= oct;
			rt  += abs( val )*v;
		}
	} else {
		while ( oct > 1 ) {
			val  = c4dnoise( seed, p, seconds );
			rt  += val*v;
			oct -= 1.0;
			v   *= 0.5;
			p    = add( p, p ); /* p += p; */
		}
		if ( oct > 0 ) {
			val  = c4dnoise( seed, p, seconds );
			val *= oct;
			rt  += val*v;
		}
	}
	return rt;
}


float slavlnoise( point p; float seconds; float seed; float dist )
{
		point vpoint;
		vector offset;
		point ptemp1, ptemp2, ptemp3;

		ptemp1 = p + vector( -2, 7, 8 );
		ptemp2 = p + vector( 13, -1, 4 );
		ptemp3 = p + vector( 17, 8, -4 );
		offset = vector( c4dnoise( seed, ptemp1, seconds ), c4dnoise( seed, ptemp2, seconds ), c4dnoise( seed, ptemp3, seconds ) );
		offset *= dist;
		vpoint = p + offset;
		return c4dnoise( seed, vpoint, seconds ) * 0.5 + 0.5;
}

float slavlnoise2d( float x; float y; float seconds; float seed; float dist )
{
		point vpoint;
		vector offset;
		point ptemp1, ptemp2;

		point p = point( x, y, 0.0 );
		ptemp1 = p + vector( -2, 7, 8 );
		ptemp2 = p + vector( 13, -1, 4 );
		offset = vector( c4dnoise( seed, ptemp1, seconds), c4dnoise( seed, ptemp2, seconds ), 0.0 );
		offset *= dist;
		vpoint = p + offset;
		return c4dnoise( seed, vpoint, seconds ) * 0.5 + 0.5;
}


point noisespace( float space; point p; string textureSpace; float ss; float tt; )
{
	point pnoise;
	if ( space >= 6 ) { /* Raster Space */
			pnoise = transform( "NDC", p ) * 0.1;
	} else if ( space >= 5) { /* Screen Space */
		pnoise = transform( "NDC", p ) * 0.1;
	} else if ( space >= 4) { /* Camera Space */
			pnoise = transform( "camera", p ) * 0.1;
		} else if ( space >= 3) { /* World Space */
			pnoise = transform( "world", p ) * 0.1;
	} else if ( space >= 2) { /* Object Space */
			pnoise = transform( "object", p ) * 0.1;
	} else if ( space >= 1) { /* UV Space */
			pnoise = point( ss * 12.8, tt * 12.8, 0 );
	} else { /* Texture Space */
			pnoise = transform( textureSpace, p ) * 10;
	}
	return pnoise;
}

void slavoronoi2d( float x; float y; float seed; float jitter; output float f1; output float f2; output point p1; output point p2 )
{
	float i, j;
	float rDistSq;
	point vTestCell, vPos;
	vector vOffset;
	point vCurrentCell = point( floor( x ) + 0.5, floor( y ) + 0.5, 0 );
	f1 = 1000;
	f2 = 1000;
	for ( i = -1; i < 2; i += 1 ) {
		for ( j = -1; j < 2; j += 1 ) {
			vTestCell  = vCurrentCell + vector( i, j, 0 );
			vPos = vTestCell + jitter * ( vector( c4dcellnoise( seed, xcomp( vTestCell ), ycomp( vTestCell ) ) - 0.5, c4dcellnoise( seed, xcomp( vTestCell ) + 20, ycomp( vTestCell ) - 10) - 0.5, 0 ) );
			vOffset = vPos - point( x, y, 0 );
			rDistSq  = vOffset . vOffset;
			if ( rDistSq < f1 ) {
				f2 = f1;
				p2 = p1;
				f1 = rDistSq;
				p1 = vPos;
			} else if ( rDistSq < f2 ) {
				f2 = rDistSq;
				p2 = vPos;
			}
		}
	}
	f1 = sqrt( f1 );
	f2 = sqrt( f2 );
}

void slavoronoi2dt( float x; float y; float seconds; float seed; float jitter; output float f1; output float f2; output point p1; output point p2 )
{
	float i, j;
	float rDistSq;
	point vTestCell, vPos;
	vector vOffset;
	point vCurrentCell = point( floor( x ) + 0.5, floor( y ) + 0.5, 0 );
	f1 = 1000;
	f2 = 1000;
	for ( i = -1; i < 2; i += 1 ) {
		for ( j = -1; j < 2; j += 1 ) {
			vTestCell  = vCurrentCell + vector( i, j, 0 );
									point ptmp  = point( floor( xcomp( vTestCell ) ), floor( ycomp( vTestCell ) ), 0 );
									point ptmp2 = point( floor( xcomp( vTestCell ) ) + 20, floor( ycomp( vTestCell ) ) - 10, 0);
			vPos = vTestCell + jitter * ( vector( boxstep( 0.3, 0.7, c4dnoise( seed, ptmp, seconds ) * 0.5 + 0.5 ) - 0.5, boxstep( 0.3, 0.7, c4dnoise( seed, ptmp2, seconds ) * 0.5 + 0.5 ) - 0.5, 0.0 ) );
			vOffset = vPos - point( x, y, 0 );
			rDistSq  = vOffset . vOffset;
			if ( rDistSq < f1 ) {
				f2 = f1;
				p2 = p1;
				f1 = rDistSq;
				p1 = vPos;
			} else if ( rDistSq < f2 ) {
				f2 = rDistSq;
				p2 = vPos;
			}
		}
	}
	f1 = sqrt( f1 );
	f2 = sqrt( f2 );
}

void slavoronoi( point p; float seconds; float seed; float jitter; output float f1; output float f2; output point p1; output point p2 )
{
	float i, j, k;
	float rDistanceSq;
	point vTestCell, vPosition;
	vector vOffset;
	point vCell = point( floor( xcomp( p ) ) + 0.5, floor( ycomp( p ) ) + 0.5, floor( zcomp( p ) ) + 0.5 );
	f1 = 1000;
	f2 = 1000;
	for ( i = -1; i < 2; i += 1 ) {
		for ( j = -1; j < 2; j += 1 ) {
			for ( k = -1; k < 2; k += 1 ) {
				point pTmp1, pTmp2, pTmp3;
				vTestCell  = vCell + vector( i, j, k );

				pTmp1 = point( floor( xcomp( vTestCell ) ),      floor( ycomp( vTestCell ) ),      floor( zcomp( vTestCell ) ) );
				pTmp2 = point( floor( xcomp( vTestCell ) ) + 20, floor( ycomp( vTestCell ) ) - 10, floor( zcomp( vTestCell ) ) + 20 );
				pTmp3 = point( floor( xcomp( vTestCell ) ) - 18, floor( ycomp( vTestCell ) ) - 16, floor( zcomp( vTestCell ) ) + 23 );
				vPosition  = vTestCell + jitter * vector( boxstep( 0.3, 0.7, ( c4dnoise( seed, pTmp1, seconds ) * 0.5 + 0.5 ) ) - 0.5,
										boxstep( 0.3, 0.7, ( c4dnoise( seed, pTmp2, seconds ) * 0.5 + 0.5 ) ) - 0.5,
										boxstep( 0.3, 0.7, ( c4dnoise( seed, pTmp3, seconds ) * 0.5 + 0.5 ) ) - 0.5 );
				vOffset = vPosition - p;
				rDistanceSq = vOffset . vOffset;
				if ( rDistanceSq < f1 ) {
					f2 = f1;
					p2 = p1;
					f1 = rDistanceSq;
					p1 = vPosition;
				} else if ( rDistanceSq < f2 ) {
					f2 = rDistanceSq;
					p2 = vPosition;
				}
			}
		}
	}
	f1 = sqrt( f1 );
	f2 = sqrt( f2 );
}


//
// Xnoise Noise Types
//
float c4dnoiseboxnoise( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0.0;
	vector vOffset;
	point vPoint;

	/* these cannot be passed directly to the DSO
	** noise function in AIR */
	point p0 = point( xcomp(p), 0, 0 );
	point p1 = point( 0, ycomp(p), 0 );
	point p2 = point( 0, 0, zcomp(p) );
	vOffset = vector( c4dnoise( seed, p0, seconds),
				c4dnoise( seed, p1, seconds),
				c4dnoise( seed, p2, seconds));

	vPoint = p + vOffset;
	value = c4dnoise( seed, vPoint, seconds ) * 0.5 + 0.5;

	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

float c4dnoiseblistturb( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0.0;
	float cutOff = 20.0;
	float temp;

	if ( cutOff < octaves ) {
		cutOff = clamp( mix( octaves, cutOff * 2.5, clamp( detailAtt, 0, 1 ) ), 0, 20 );
		temp   = boxstep( -1.5, 1.5, c4dturbulence( p * 0.4, seconds, seed, cutOff, 0 ) );
		cutOff = clamp( mix( octaves, cutOff, clamp( detailAtt, 0, 1 ) ), 0, 20 );
		value  = boxstep( 0.5 - temp, 0.5 + temp, boxstep( -1.5, 1.5, c4dturbulence( p, seconds, seed, cutOff, absolute ) ) );
	} else {
		temp  = boxstep( -1.5, 1.5, c4dturbulence( p * 0.4, seconds, seed, octaves, 0 ) );
		value = boxstep( 0.5 - temp, 0.5 + temp, boxstep( -1.5, 1.5, c4dturbulence( p, seconds, seed, octaves, absolute ) ) );
	}
	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}
	return value;
}

float c4dnoisebuya( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0.0;
	float cutOff = 20.0; /* TODO: use this value for antialiasing */
	float sq;

	if (cutOff < octaves) {
		cutOff = clamp(mix(octaves, cutOff, clamp(0.0, 1.0, detailAtt)), 0, 20);
	} else {
		cutOff = octaves;
	}

	sq = c4dfbm4d(p, seconds, seed, cutOff, 2.1, 0.5) * 0.5;

	value = boxstep(0.0, 0.35, sq * sq);

	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}
	return value;
}

float c4dnoisecellnoise( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	point vPoint;

	vPoint = p * 1.0001;
	value = c4dcellnoise( seed, vPoint, seconds );

	return value;
}

float c4dnoisecranal( point p_in; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	point p = p_in;
	float value = 0;
	float cutOff;
	float i;

	cutOff = 20.0;

	if ( cutOff < octaves ) {
		cutOff = clamp( 0.0, 20.0, mix( octaves, cutOff, clamp( 0.0, 1.0, detailAtt ) ) );
	} else {
		cutOff = octaves;
	}

	if ( cutOff > 0.0 ) {
		value = 0.0;
		for ( i = 1; i < cutOff; i += 1) {
			value += pow( abs( c4dnoise( seed, p, seconds ) ), 0.3219281);
			setxcomp( p, xcomp( p ) + 3.0 );
		}
		if ( cutOff != 1.0 ) {
			value += pow( abs( c4dnoise( seed, p, seconds ) ), 0.3219281 ) * ( cutOff - floor( cutOff ) );
		} else {
			value += pow( abs( c4dnoise( seed, p, seconds ) ), 0.3219281);
		}
		if ( cutOff >= 1.0 ) {
			value /= cutOff;
		} else {
			value = value - ( cutOff - floor( cutOff ) ) * 0.5 + 0.5;
		}
	} else {
		value = 0.5;
	}
	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisedents( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float i, temp;
	point ptemp;

	for ( i = 1; i <= octaves; i += 1 ) {
		ptemp = point( xcomp( p ) + i + 1, ycomp( p ) + i + 1, zcomp( p ) + i + 1 );
		temp = boxstep( 0.0, 0.65, abs( c4dnoise( seed, ptemp, seconds ) ) );
		if (temp > value) {
			value = temp;
		}
	}
	ptemp = point( xcomp( p ) + i + 1, ycomp( p ) + i + 1, zcomp( p ) + i + 1 );
	temp = ( octaves - floor( octaves ) ) * boxstep( 0.0, 0.65, abs( c4dnoise( seed, ptemp, seconds ) ) );
	if (temp > value) {
		value = temp;
	}

	value = boxstep( 0.25, 1.0, value ) * value;
	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

float c4dnoisedisplturb( point p_in; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float i;
	float displaceNoise;
	float temp;
	point pTemp;
	point p = p_in;

	float cutOff = 20.0;

	if (cutOff < octaves) {
		cutOff = clamp(mix(octaves, cutOff, clamp(detailAtt, 0, 1)), 0, 20);
	} else {
		cutOff = octaves;
	}

	if (cutOff > 0.0) {
		value = 0;
		for (i = 1; i <= cutOff; i += 1) {
			displaceNoise = c4dturbulence(p, seconds, seed, 2, 0);
			pTemp = point(xcomp(p) - displaceNoise, ycomp(p) + displaceNoise, zcomp(p) + displaceNoise);
			temp = c4dnoise( seed, pTemp, seconds ) * 0.5 + 0.5;
			setxcomp(p, xcomp(p) + 5.0);
			value += temp;
		}
		displaceNoise = c4dturbulence(p, seconds, seed, 2, 0);
		pTemp = point(xcomp(p) - displaceNoise, ycomp(p) + displaceNoise, zcomp(p) + displaceNoise);
		temp = c4dnoise( seed, pTemp, seconds ) * 0.5 + 0.5;
		value += (temp * (cutOff - floor(cutOff)));

		if (cutOff >= 1.0) {
			value /= cutOff;
		} else {
			value = value - ( cutOff - floor(cutOff) ) * 0.5 + 0.5;
		}
	} else {
		value = 0.5;
	}
	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

float c4dnoisefbm( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float cutOff = octaves;

	value = smoothstep( -1.75, 1.75, c4dfbm4d( p, seconds, seed, cutOff, 2.1, 0.5 ) );

	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

#if 0
	case NOISE_HAMA:
		{
			rValue = 0.0;
			for (i = 0; i < rOctaves; i++)
			{
				rTemp = Smoothstep(.65, 1.0, 1.0 - Abs(SNoise(Vector(p.x + i * 10.0, p.y  + i * 10.0, p.z + i * 10.0), rTime)));
				if (rTemp > rValue)
					rValue = rTemp;
			}
			rTemp = (rOctaves - (Int32)rOctaves) * Smoothstep(.65, 1.0, 1.0 - Abs(SNoise(Vector(p.x + i * 10.0, p.y  + i * 10.0, p.z + i * 10.0), rTime)));
			if (rTemp > rValue)
				rValue = rTemp;
			if (rSampleRad > 0.0 && rDetailAtt > 0.0)
			{
				rAverage = 0.0;
				rTemp = .5;
				for (i = 1; i <= rOctaves; i++)
				{
					rAverage += rTemp;
					rTemp *= .5;
				}
				rAverage2 = rAverage + rTemp;
				rAverage = Mix(rAverage, rAverage2, rOctaves - (Int32)rOctaves);
				rValue = Mix(rAverage, rValue, Boxstep(0.0, 1.0, .5 / (rSampleRad * rDetailAtt)));
			}
			rValue = 1.0 - rValue;
			if (bAbsolute)
				rValue = Abs(2.0 * rValue - 1.0);
		}
		break;
#endif

float c4dnoisehama( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float i, temp, average, average2;
	point ptemp;

	for ( i = 0; i < octaves; i += 1 ) {
		ptemp = point( xcomp( p ) + i * 10.0, ycomp( p ) + i * 10.0, zcomp( p ) + i * 10.0);
		temp = smoothstep( 0.65, 1.0, 1.0 - abs( c4dnoise( seed, ptemp, seconds ) ) );
		if ( temp > value ) {
			value = temp;
		}
	}
	ptemp = point( xcomp( p ) + i * 10.0, ycomp( p ) + i * 10.0, zcomp( p ) + i * 10.0);
	temp = ( octaves - floor( octaves ) ) * smoothstep( 0.65, 1.0, 1.0 - abs( c4dnoise( seed, ptemp, seconds ) ) );
	if ( temp > value ) {
		value = temp;
	}

	float sampleRad = 0.0001;
	if (sampleRad > 0.0 && detailAtt > 0.0) {
		average = 0.0;
		temp = 0.5;
		for ( i = 1; i <= octaves; i += 1 )
		{
			average += temp;
			temp *= 0.5;
		}
		average2 = average + temp;
		average = mix( average, average2, octaves - floor( octaves ) );
		value = mix( average, value, boxstep( 0.0, 1.0, 0.5 / (sampleRad * detailAtt)));
	}

	value = 1.0 - value;
	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0 );
	}

	return value;
}

float c4dnoiseluka( point p_in; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float cutOff = 20;
	float temp;
	float displaceNoise;
	point p = p_in;

	if ( cutOff < octaves ) {
		cutOff = clamp( mix( octaves, cutOff, clamp( detailAtt, 0, 1 ) ), 0, 20 );
	} else {
		cutOff = octaves;
	}
	temp = c4dturbulence( p, seconds, seed, octaves, 1 );
	displaceNoise = c4dnoise( seed, p, seconds ) * .5;
	value = boxstep( .5 - temp, .5 + temp, boxstep( -2, 2, c4dturbulence( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise ), seconds, seed, cutOff, 1 ) ) );
	value = boxstep( .5, .8, value );
	value = clamp( value, 0, 1 );
	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}
	return value;
}

float c4dnoisemodnoise( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	point pTemp = p;

	pTemp *= 1.0001;
	pTemp = point(floor(xcomp(pTemp)), floor(ycomp(pTemp)), floor(zcomp(pTemp)));
	value = c4dnoise( seed, pTemp, seconds) * 0.5 + 0.5;

	return value;
}

float c4dnoisenaki( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float displaceNoise;
	float temp;
	point pTemp;

	float cutOff = 20.0;

	if (cutOff < octaves) {
		cutOff = clamp( mix( octaves, cutOff, clamp( detailAtt, 0.0, 1.0 ) ), 0.0, 20.0);
	} else {
		cutOff = octaves;
	}
	displaceNoise = c4dturbulence( p, seconds, seed, cutOff, 1 ) * .5;
	pTemp = point( xcomp( p ) + displaceNoise, ycomp( p ) + displaceNoise, zcomp( p ) + displaceNoise );
	temp = c4dnoise( seed, pTemp, seconds ) * 0.5 + 0.5;
	value = boxstep( 0.4, 1.0, ( temp + smoothstep( .4, .6, displaceNoise * .25 + .5 ) ) * .5 );

	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}
	return value;
}

float c4dnoisenoise( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	point myP = p;
	float myTime = seconds;
	value = c4dnoise( seed, myP, myTime ) * 0.5 + 0.5;
	if ( absolute != 0 ) {
		value = abs( value );
	}
	return value;
}

float c4dnoisenutous( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float displaceNoise;
	float cutOff = 20.0;
	cutOff = octaves;

	displaceNoise = c4dturbulence( p, seconds, seed, octaves, 0 ) * 0.5;
	value = boxstep( 0.0, 0.5, abs( c4dturbulence( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise), seconds, seed, cutOff, absolute ) ) * 0.5 );
	return value;
}

float c4dnoiseober( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float temp = c4dturbulence( p, seconds, seed, octaves, 1) * .5;
	float displaceNoise = c4dnoise( seed, p, seconds );
	value = boxstep( 0.5 - temp, 0.5 + temp, boxstep( -0.5, 0.6, c4dridgedmultifractal( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise), seed, octaves, 0.25, 1.0, 2.1, 0.5 ) ) );
	value = smoothstep( 0.4, 0.9, 1.0 - value);
	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

#if 0
			if (rSampleRad > 0.0 && rDetailAtt > 0.0)
			{
				rCutOff = .5 / (rSampleRad * rDetailAtt);
				rTemp2 = rCutOff;
				rCutOff2 = rCutOff * .1;
			}
			else
			{
				rCutOff2 = rCutOff = 20.0;
				rTemp2 = .5 / (rSampleRad * rDetailAtt);
			}
			if (rCutOff < rOctaves)
			{
				rCutOff = Clamp(Mix(rOctaves, rCutOff, Clamp01(rDetailAtt)), 0.0, 20.0);
				rCutOff2 = Clamp(Mix(rOctaves, rCutOff2, Clamp01(rDetailAtt)), 0.0, 20.0);
			}
			else if (rCutOff2 < rOctaves)
			{
				rCutOff = rOctaves;
				rCutOff2 = Clamp(Mix(rOctaves, rCutOff2, Clamp01(rDetailAtt)), 0.0, 20.0);
			}
			else
			{
				rCutOff = rCutOff2 = rOctaves;
			}
			rValue = 1.0;
			rAverage = 1.0;
			for (i = 0; i < rCutOff; i++)
			{
				rTemp = 1.0 - Abs(SNoise(Vector(p.x + i * 20, p.y + i * 20, p.z + i * 20), rTime));
				rValue *= Smoothstep(.6, 1.0, rTemp);
				rAverage *= .5;
			}
			rTemp = 1.0 - Abs(SNoise(Vector(p.x + i * 20, p.y + i * 20, p.z + i * 20), rTime));
			rTemp = rValue * Smoothstep(.6, 1.0, rTemp);
			rAverage2 = rAverage * .5;
			rValue = Mix(rValue, rTemp, (rCutOff - (Int32)rCutOff));
			rAverage = Mix(rAverage, rAverage2, (rCutOff - (Int32)rCutOff));
			rValue = Mix(rAverage, rValue, Boxstep(0.0, 1.0, rTemp2));

			rTemp = Mix(.25, Smoothstep(-1.5, 1.5, Turbulence(p * 10.0, rTime, rCutOff2, TRUE)), Boxstep(0.0, 1.0, rTemp2 * .1));

			rValue = Mix(.25, 1.0, rValue) * rTemp;
			rValue = Smoothstep(0.0, .8, rValue);
			if (bAbsolute)
				rValue = Abs(2.0 * rValue - 1.0);
#endif

float c4dnoisepezo( point p; float seconds; float seed; float octaves; float detailAtt_in; float absolute; )
{
	float value = 0;
	float detailAtt = 0.00001;
	float sampleRad = 1;

	float cutOff2, cutOff, temp2;

	float i, temp;
	float average, average2;
	point ptemp;

	if (sampleRad > 0.0 && detailAtt > 0.0) {
		cutOff = .5 / ( sampleRad * detailAtt );
		temp2 = cutOff;
		cutOff2 = cutOff * .1;
	} else {
		cutOff2 = cutOff = 20.0;
		temp2 = .5 / ( sampleRad * detailAtt );
	}

	if ( cutOff < octaves ) {
		cutOff  = clamp( mix( octaves, cutOff,  clamp( detailAtt, 0, 1 ) ), 0, 20 );
		cutOff2 = clamp( mix( octaves, cutOff2, clamp( detailAtt, 0, 1 ) ), 0, 20 );
	} else if (cutOff2 < octaves ) {
		cutOff = octaves;
		cutOff2 = clamp( mix( octaves, cutOff2, clamp( detailAtt, 0, 1 ) ), 0, 20 );
	} else {
		cutOff = cutOff2 = octaves;
	}

	value = 1.0;
	average = 1.0;

	// TEMP!
	//cutOff = octaves;
/*
			rValue = 1.0;
			rAverage = 1.0;
			for (i = 0; i < rCutOff; i++)
			{
				rTemp = 1.0 - Abs(SNoise(Vector(p.x + i * 20, p.y + i * 20, p.z + i * 20), rTime));
				rValue *= Smoothstep(.6, 1.0, rTemp);
				rAverage *= .5;
			}
			rTemp = 1.0 - Abs(SNoise(Vector(p.x + i * 20, p.y + i * 20, p.z + i * 20), rTime));
			rTemp = rValue * Smoothstep(.6, 1.0, rTemp);
			rAverage2 = rAverage * .5;
			rValue = Mix(rValue, rTemp, (rCutOff - (Int32)rCutOff));
			rAverage = Mix(rAverage, rAverage2, (rCutOff - (Int32)rCutOff));
			rValue = Mix(rAverage, rValue, Boxstep(0.0, 1.0, rTemp2));

			rTemp = Mix(.25, Smoothstep(-1.5, 1.5, Turbulence(p * 10.0, rTime, rCutOff2, TRUE)), Boxstep(0.0, 1.0, rTemp2 * .1));

			rValue = Mix(.25, 1.0, rValue) * rTemp;
			rValue = Smoothstep(0.0, .8, rValue);
			if (bAbsolute)
				rValue = Abs(2.0 * rValue - 1.0);
*/

	for ( i = 0; i < cutOff; i += 1 ) {
		ptemp = point( xcomp( p ) + i * 20, ycomp( p ) + i * 20, zcomp( p ) + i * 20);
		temp = 1.0 - abs( c4dnoise( 665, ptemp, seconds ) );
		value *= smoothstep( 0.6, 1.0, temp );
		average *= .5;
	}
	ptemp = point( xcomp( p ) + i * 20, ycomp( p ) + i * 20, zcomp( p ) + i * 20 );
	temp = 1.0 - abs( c4dnoise( 665, ptemp, seconds ) );
	temp = value * smoothstep( 0.6, 1.0, temp );
	average2 = average * 0.5;
	value = mix( value, temp, ( cutOff - floor( cutOff ) ) );
	average = mix( average, average2, ( cutOff - floor( cutOff ) ) );
	value = mix( average, value, boxstep( 0.0, 1.0, temp2 ) );
	temp = mix( 0.25, smoothstep( -1.5, 1.5, c4dturbulence( p * 10.0, seconds, seed, cutOff2, 1 ) ), boxstep( 0.0, 1.0, temp2 * 0.1 ) );
	value = mix( 0.25, 1.0, value) * temp;
	value = smoothstep( 0.0, 0.8, value );
	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

float c4dnoisepoxo( point p; float seconds; float seed; float octaves; float detailAtt_in; float absolute; )
{
	float value = 0;
	float detailAtt = 0.00001;
	float sampleRad = 1;

	float size = 1.0;
	float i, temp;
	float average, average2;

	value = 0;

	for ( i = 0; i < octaves; i += 1 ) {
		temp = c4dturbulence( p * size, seconds, seed, 3.0, 1 );
		value += abs( temp - value ) * 0.5;
		size *= 2.0;
	}
	temp = c4dturbulence( p * size, seconds, seed, 3.0, 1 );
	value += abs( temp - value ) * 0.5 * ( octaves - floor( octaves ) );
	if ( octaves != 0.0 ) {
		value /= octaves;
	}
	value = clamp( pow( value, 0.7369656 ), 0, 1 );
	value = value * value * 2.0;
	value = boxstep( 0.0, 0.5, value );

	if ( sampleRad > 0.0 && detailAtt > 0.0) {
		average = 0.0;
		temp = 0.7;
		for ( i = 1; i <= octaves; i += 1 ) {
			average += temp;
			temp *= .3;
		}
		average2 = average + temp;
		average = mix( average, average2, ( octaves - floor( octaves ) ) );
		average = clamp( 1 - average, 0.0, 1.0 );
		value = mix( average, value, boxstep( 0.0, 1.0, 0.5 / ( sampleRad * detailAtt ) ) );
	}
	if ( absolute != 0 ) {
		value = abs(2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisesema( point p; float seconds; float seed; float octaves; float detailAtt_in; float absolute; )
{
	float value = 0;
	float detailAtt = 0.00001;
	float sampleRad = 1;

	float i, temp;
	point ptemp;
	float displaceNoise;
	float average, average2;

	value = 0.0;
	for ( i = 1; i <= octaves; i += 1 ) {
		ptemp = point( xcomp( p ) + i * 21.0, ycomp( p ) + i * 11.0, zcomp( p ) + i * 13.0);
		temp = 1.0 - abs( c4dnoise( seed, ptemp, seconds ) );
		displaceNoise = smoothstep(.875, .999, temp);
		if ( displaceNoise > value ) {
			value = displaceNoise;
		}
	}

	ptemp = point( xcomp( p ) + i * 21.0, ycomp( p ) + i * 11.0, zcomp( p ) + i * 13.0);
	temp = ( octaves - floor( octaves ) ) * ( 1.0 - abs( c4dnoise( seed, ptemp, seconds ) ) );
	displaceNoise = smoothstep(.875, .999, temp);
	if ( displaceNoise > value ) {
		value = temp;
	}
	value = boxstep( 0.95, 1.00, value ) + ( ( c4dturbulence( p * 75.0, seconds, seed, 1.0, absolute ) * 0.25 + 0.5 ) * 0.05 );
	if ( sampleRad > 0.0 && detailAtt > 0.0 ) {
		average = 0.0;
		temp = 0.15;
		for ( i = 1; i <= octaves; i += 1 ) {
			average += temp;
			temp *= .5;
		}
		average2 = average + temp;
		average = mix( average, average2, ( octaves - floor( octaves ) ) );
		value = mix( average, value, boxstep( 0.0, 1.0, 0.5 / ( sampleRad * detailAtt ) ) );
	}
	return value;
}

float c4dnoisestupl( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float cutOff = 20;
	float temp;
	float displaceNoise;

	if ( cutOff < octaves ) {
		cutOff = clamp( mix( octaves, cutOff, clamp( detailAtt, 0, 1 ) ), 0, 20 );
	} else {
		cutOff = octaves;
	}
	temp = c4dturbulence( p, seconds, seed, octaves, 1 );
	displaceNoise = c4dnoise( seed, p, seconds );
	value = boxstep( .5 - temp, .5 + temp, boxstep( -2, 2, c4dturbulence( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise ), seconds, seed, cutOff, 0 ) ) );
	value = smoothstep( 0, 1, value);
	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}
	return value;
}

float c4dnoiseturbulence( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float cutOff = 20;

	if ( cutOff < octaves ) {
		cutOff = clamp( mix( octaves, cutOff, clamp( detailAtt, 0, 1 ) ), 0, 20 );
	} else {
		cutOff = octaves;
	}
	value = boxstep( -1.75, 1.75, c4dturbulence( p, seconds, seed, cutOff, absolute ) );
	return value;
}

float c4dnoisevlnoise( point p; float seconds; float seed; float octaves; float detailAtt_in; float absolute; float b2dnoise)
{
	float value = 0;
	float detailAtt = 0.00001;
	float sampleRad = 1;

	if ( b2dnoise > 0.5 ) {
		value = slavlnoise2d( xcomp( p ), ycomp( p ), seconds, seed, 1.0 );
	} else {
				value = slavlnoise( p, seconds, seed, 1.0 );
	}

	if ( sampleRad > 0.0 && detailAtt > 0.0 ) {
		value = mix( 0.5, value, boxstep( 0.0, 1.0, 0.5 / ( sampleRad * detailAtt ) ) );
	}
	if ( absolute != 0 ) {
		value = abs( 2.0 * value - 1.0 );
	}
	return value;
}

float c4dnoisewavyturb( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float temp;
	point pTemp;

	float cutOff = 20.0;

	if (cutOff < octaves) {
		cutOff = clamp(mix(octaves, cutOff, clamp(detailAtt, 0.0, 1.0)), 0.0, 20.0);
	} else {
		cutOff = octaves;
	}

	temp = c4dturbulence( p + vector( 0.3 * c4dnoise( seed, p, seconds)  ), seconds * 0.2, seed, octaves, 1) * 0.5;
	pTemp = point(p) + vector(temp + temp);
	value = c4dnoise( seed, pTemp, seconds ) * 0.5 + 0.5;

	if ( absolute != 0 ) {
		value = abs( 2 * value - 1 );
	}

	return value;
}

float c4dnoisecellvoronoi( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	float f1 = 0;
	float f2 = 0;
	point p1 = 0;
	point p2 = 0;

	if ( b2dnoise > 0.5 ) {
		slavoronoi2dt( xcomp( p ), ycomp( p ), seconds, seed, 1.0, f1, f2, p1, p2 );
	} else {
		slavoronoi( p, seconds, seed, 1.0, f1, f2, p1, p2 );
	}

	p1 += vector( 10, 0, 0 );
	value = c4dnoise( 665, p1 ) * 0.5 + 0.5;

	return value;
}

float c4dnoisedisplvoronoi( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	float f1 = 0;
	float f2 = 0;
	point p1 = 0;
	point p2 = 0;

	if ( b2dnoise > 0.5 ) {
		slavoronoi2dt( xcomp( p ) + 0.15 * c4dfbm4d( p * 2.2, seconds, seed, octaves, 2.1, 0.5 ),
						ycomp( p ) + 0.15 * c4dfbm4d( ( p + vector( 7, 9, 4 ) ) * 2.2, seconds, seed, octaves, 2.1, 0.5 ), seconds, seed, 1, f1, f2, p1, p2 );
	}  else {
		point ptmp = point( xcomp( p ) + 0.15 * c4dfbm4d( p * 2.2, seconds, seed, octaves, 2.1, 0.5 ),
												ycomp( p ) + 0.15 * c4dfbm4d( (p + vector( 7, 9,  4 ) ) * 2.2, seconds, seed, octaves, 2.1, 0.5 ),
												zcomp( p ) + 0.15 * c4dfbm4d( (p + vector( 8, 1, 12 ) ) * 2.2, seconds, seed, octaves, 2.1, 0.5 ) );
		slavoronoi( ptmp, seconds, seed, 1.0, f1, f2, p1, p2 );
	}
	value = f1;

	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisesparseconv( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	if ( b2dnoise > 0.5 ) {
		value = 0.5 * ( c4dsparseconvolution( seed, xcomp( p ), ycomp( p ), seconds ) + 1 );
	} else {
		value = 0.5 * ( c4dsparseconvolution( seed, p, seconds ) + 1 );
		//value = 0.5 * ( c4dsparseconvolution( seed, xcomp( p ), ycomp( p ), zcomp( p ) ) + 1 );
	}

	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisevoronoi1( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	float f1 = 0;
	float f2 = 0;
	point p1 = 0;
	point p2 = 0;

	if ( b2dnoise > 0.5 ) {
		slavoronoi2dt( xcomp( p ), ycomp( p ), seconds, seed, 1, f1, f2, p1, p2 );
	} else {
		slavoronoi( p, seconds, seed, 1, f1, f2, p1, p2 );
	}
	value = f1;

	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisevoronoi2( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	float f1 = 0;
	float f2 = 0;
	point p1 = 0;
	point p2 = 0;

	if ( b2dnoise > 0.5 ) {
		slavoronoi2dt( xcomp( p ), ycomp( p ), seconds, seed, 1, f1, f2, p1, p2 );
	} else {
		slavoronoi( p, seconds, seed, 1, f1, f2, p1, p2 );
	}
	value = f2;

	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisevoronoi3( point p; float seconds; float seed; float octaves; float detailAtt; float absolute; float b2dnoise; )
{
	float value = 0;

	float f1 = 0;
	float f2 = 0;
	point p1 = 0;
	point p2 = 0;

	if ( b2dnoise > 0.5 ) {
		slavoronoi2dt( xcomp( p ), ycomp( p ), seconds, seed, 1, f1, f2, p1, p2 );
	} else {
		slavoronoi( p, seconds, seed, 1, f1, f2, p1, p2 );
	}
	value = f2 - f1;

	if ( absolute > 0.5 ) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

float c4dnoisezada( point p_in; float seconds; float seed; float octaves; float detailAtt; float absolute; )
{
	float value = 0;
	float i, temp;
	point p = p_in;
	float displaceNoise = c4dnoise( seed, p, seconds );

	for ( i = 0; i < octaves; i += 1 ) {
		temp = smoothstep( 0.0, 0.25, abs( c4dridgedmultifractal( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise ), seed, octaves, 0.25, 1.0, 2.1, 0.5 ) ) );
		temp = boxstep( 0.6, 1.0, pow( temp, 0.0740006 ) );
		setxcomp( p, xcomp( p ) + 5.0 );
		if (temp > value) {
			value = temp;
		}
	}
	temp = smoothstep( 0.0, 0.25, abs( c4dridgedmultifractal( point( xcomp( p ) + displaceNoise, ycomp( p ) - displaceNoise, zcomp( p ) + displaceNoise ), seed, octaves, 0.25, 1.0, 2.1, 0.5 ) ) );
	temp = boxstep( 0.6, 1.0, pow( temp, 0.0740006 ) ) * ( octaves - floor( octaves ) );
	if (temp > value) {
		value = temp;
	}

	if ( absolute != 0) {
		value = abs( 2.0 * value - 1.0);
	}
	return value;
}

color c4dnoisemap(
	float funcSample_in;
	color color1;
	color color2;
	float cycles;
	float lowClip;
	float highClip;
	float brightness;
	float contrast;
	float isBumpChannel;
)
{
	float funcSample = funcSample_in;
	color value;
	if ( cycles > 0 && cycles < 1 ) {
		funcSample = mix( funcSample, 0.5 - 0.5 * cos( funcSample * ( PI * 2 ) ), cycles );
	} else if ( cycles >= 1 ) {
		funcSample = 0.5 - 0.5 * cos( cycles * funcSample * ( PI * 2 ) );
	}

	funcSample = boxstep( lowClip, highClip, funcSample );
	funcSample = clamp( funcSample + brightness, 0, 1 );

	if ( contrast >= 0 ) {
		if ( contrast == 1 ) {
			funcSample = contrast * 0.5 > funcSample ? 0 : 1;
		} else {
			funcSample = boxstep( contrast * 0.5, 1 - ( contrast * 0.5 ), funcSample );
		}
	} else {
		funcSample = mix( funcSample, 0.5, -contrast );
	}
	value = mix( color1, color2, funcSample );

	if ( isBumpChannel > 0.5 ) {
		value /= 5;
	}
	return value;
}

#endif
