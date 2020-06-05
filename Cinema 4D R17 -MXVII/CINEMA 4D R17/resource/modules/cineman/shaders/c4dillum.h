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

#ifndef C4DILLUM_H
#define C4DILLUM_H

/**
 *
 * Oren-Nayar based on Larry Gritz' implementation at:
 * http://www.renderman.org/RMR/Shaders/LGShaders/LG_orennayar.sl
 *
 * References:
 * Cook, Robert L., Kenneth E. Torrance, "A Reflectance Model for Computer Graphics," ACM Transactions on Graphics, 1(1), January 1982, 7-27. http://doi.acm.org/10.1145/357290.357293
 * Oren, Michael, Shree K. Nayar, "Generalization of Lambert's reflectance model," Proceedings of the 21st annual conference on Computer graphics and interactive techniques, July 1994, 239-246. http://doi.acm.org/10.1145/192161.192213
 *
 */


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

color diffuseOrenNayar(
		normal Nf;
		vector V;
		float illuminationDiffuseFalloff;
		float illuminationDiffuseLevel;
		float illuminationRoughness;
		output color C_unshadowed;
		output color C_shadow;
)
{
		float sigma = illuminationRoughness;
		float falloff = illuminationDiffuseFalloff * .65;
		vector Ln;
		color C = 0;
		float L1, L2;
		float C1, C2, C3;
		float theta_r, theta_i, cos_theta_i;
		float alpha, beta, sigma2, cos_phi_diff;
		extern point P;

		theta_r = acos( V.Nf );
		sigma2 = sigma * sigma;

		C_unshadowed = 0;
		C_shadow = 1;

		illuminance( "-foglight", P, Nf, PI/2 ) {
				float nondiff = 0;
				lightsource ("__nondiffuse", nondiff);
				if (nondiff < 1) {
			color unshadowedCl;
			color shadowC;
			color Lt;
			float contrast = 0;

			lightsource( "__contrast", contrast );

						Ln = normalize( L );

						cos_theta_i = Ln.Nf;
						cos_phi_diff = normalize( V - Nf * ( V.Nf ) ).normalize( Ln - Nf *( Ln.Nf ) );

						theta_i = acos( cos_theta_i );

						alpha = max( theta_i, theta_r );
						beta = min( theta_i, theta_r );
						C1 = 1 - 0.5 * sigma2 / ( sigma2 + 0.33 );
						C2 = 0.45 * sigma2 / ( sigma2 + 0.09 );
						if ( cos_phi_diff >= 0 ) {
								C2 *= sin( alpha );
						} else {
								C2 *= ( sin( alpha ) - pow( 2 * beta / PI, 3 ) );
						}
						C3 = 0.125 * sigma2 / ( sigma2 + 0.09 ) * pow( ( 4 * alpha * beta ) / ( PI * PI ), 2 );


						L1 =  ( cos_theta_i * (C1 + cos_phi_diff * C2 * tan( beta ) + (1 - abs( cos_phi_diff ) ) * C3 * tan( ( alpha + beta ) / 2 ) ) );
						L2 =  ( 0.17 * cos_theta_i * sigma2 / ( sigma2 + 0.13 ) * (1 - cos_phi_diff * ( 4 * beta * beta ) / ( PI * PI ) ) );

						Lt = pow( L1 + L2, 1 - ( falloff + contrast ) ) * ( ( ( 1 - ( illuminationDiffuseFalloff + contrast ) ) / 5 ) + 0.8 );


						/* avoid rounding artifacts */
			Lt = clamp(Lt, color( 0 ), color( 1 ));

						C += Cl * Lt * illuminationDiffuseLevel;

			lightsource ("__unshadowed_Cl", unshadowedCl);
						C_unshadowed += unshadowedCl * Lt * illuminationDiffuseLevel;

						lightsource ("__shadow", shadowC);
						C_shadow *= color(1) - shadowC;
				}
		}
		return C;
}


color diffuseLambert(
		normal N;
		float illuminationDiffuseFalloff;
		output color C_unshadowed;
		output color C_shadow;
)
{
		color C = 0;
		float falloff = illuminationDiffuseFalloff * .65;
		extern point P;

		C_unshadowed = 0;
		C_shadow = 1;

		illuminance( "-foglight", P, N, PI/2 ) {
				float nondiff = 0;
				lightsource( "__nondiffuse", nondiff );
				if (nondiff < 1) {
			float atten;
						color unshadowedCl;
						color shadowC;
			float contrast = 0;

			lightsource( "__contrast", contrast );

			atten = pow( N.normalize( L ), 1 - ( falloff + contrast ) );
						C += Cl * atten;

			lightsource ("__unshadowed_Cl", unshadowedCl);
			C_unshadowed += unshadowedCl * atten;

						lightsource ("__shadow", shadowC);
						C_shadow *= color(1) - shadowC;
				}
		}
		return C;
}

float highlight(
		float angle;
		float width;
		float height;
		float falloff;
		float innerw;
)
{
		float expo = pow( 100.0, falloff + 0.125);
		float fact = 1.0 / width / (1.0001 - width * innerw);
		float subt = innerw / (1.0001 - width * innerw);
		float twbw = 2.0 / width;

		return pow(1.0 - pow(clamp(angle*fact - subt, 0.0, 1.0), expo), twbw)*height;
}

color phongSpecular(
		normal N;
		vector V;
		float specularWidth;
		float specularHeight;
		float specularFalloff;
		float specularInnerWidth;
		output color C_unshadowed;
)
{
		color C = 0;
		vector R = reflect( -normalize(V), normalize(N) );
		extern point P;

		C_unshadowed = 0;

		illuminance( "-foglight", P, N, PI/2 ) {
				float nonspec = 0;
				lightsource ("__nonspecular", nonspec);
				if (nonspec < 1) {
						vector Ln = normalize(L);
						float cos_theta = R.Ln;
						float theta = acos( cos_theta );
						float atten = highlight( theta/(PI/2), specularWidth, specularHeight, specularFalloff, specularInnerWidth );
						C += Cl * atten;

						color unshadowedCl;
			lightsource ("__unshadowed_Cl", unshadowedCl);
			C_unshadowed += unshadowedCl * atten;
				}
		}
		return C;
}

color blinnSpecular(
		normal N;
		vector V;
		float specularWidth;
		float specularHeight;
		float specularFalloff;
		float specularInnerWidth;
		output color C_unshadowed;
)
{
		color C = 0;
		vector Vn = normalize( V );
		normal Nn = normalize( N );
		extern point P;

		C_unshadowed = 0;

		illuminance( "-foglight", P, N, PI/2 ) {
				float nonspec = 0;
				lightsource ("__nonspecular", nonspec);
				if (nonspec < 1) {
						vector Ln = normalize(L);
						vector H = normalize( Vn + Ln );
						float cos_theta = Nn.H;
						float theta = acos( cos_theta );
						float atten = highlight( theta/(PI/2), specularWidth, specularHeight, specularFalloff, specularInnerWidth );
						C += Cl * atten;

						color unshadowedCl;
			lightsource ("__unshadowed_Cl", unshadowedCl);
			C_unshadowed += unshadowedCl * atten;
				}
		}
		return C;
}

color cookTorranceSpecular(
		normal N;
		vector V;
		float specularWidth;
		float specularHeight;
		float specularFalloff;
		float specularInnerWidth;
)
{
		vector R = reflect( -normalize(V), normalize(N) );
		color C = 0;
		normal Nn = normalize( N );
		extern point P;
		extern vector I;

		illuminance( "-foglight", P, N, PI/2 ) {
				float nonspec = 0;
				lightsource ("__nonspecular", nonspec);
				if (nonspec < 1) {
						vector Ln = normalize(L);
						vector H = normalize( Ln + normalize(V) );
						float c3 = pow( .75, 2 );
						float D = c3 / ( ( ( R.Ln ) * ( c3 - 1 ) + 1 ) );
						D = D * D;
						float Ga = 1;
						float Gb = 2 * ( Nn.H ) * ( Nn.V ) / ( V.H );
						float Gc = 2 * ( Nn.H ) * ( Nn.Ln ) / ( V.H );
						float G = min( Ga, Gb, Gc );

						float n = 1.8;
						float c = V.H;
						float g = sqrt( n*n + c*c - 1 );
						float F = pow( g - c, 2 ) / pow( g + c, 2 );
						F *= 1 + pow( c * ( g + c ) - 1, 2 ) / pow( c * ( g - c ) + 1, 2 );

						float Kr=0, Kt=0;
						normal Nf = faceforward( normalize( N ), I );
						fresnel( normalize(I), Nf, 1.8, Kr, Kt );
						C += Cl * ( G*D*(1-F) / ( Nn.V ) );
				}
		}
		return C;
}


#endif // #ifndef C4DILLUM_H
