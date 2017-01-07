#pragma once

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <map>

#include "tinyxml2.h"
#include "Vector.hpp"

using namespace std;

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Color
 * Purpose: A class that contains colors for objects
 */
class Color {
	public: 
		Color();

		Color(std::string fileName) {
			tinyxml2::XMLDocument xml;
			xml.LoadFile(fileName.c_str());

			if(xml.Error()) {
				cout << "There was an error parsing " << fileName << endl;
				exit(1);
			}

			tinyxml2::XMLNode * colorParent = xml.FirstChild()->NextSibling();
			if( !colorParent) {
				cout << "There was an error parsing " << fileName << endl;
				exit(1);	
			}

			tinyxml2::XMLElement * color = colorParent->FirstChildElement();

			// Go through all the custom colors and map them
			while( color ) {
				int r, g, b;
				char * colorName;

				color->QueryIntAttribute("r", &r);
				color->QueryIntAttribute("g", &g);
				color->QueryIntAttribute("b", &b);

				colorName = (char *)color->FirstAttribute()->Value();


				color = color->NextSiblingElement();

//TODO <BMV> Check this for error checking on xml so we don't just seg fault by accidnet
				std::string str(colorName);
				_colorMap[str] = Vec3<unsigned char>::vec3(r, g, b);
			}
		}

		/* 
		 * Date: 1/7/16
		 * Function Name: GetColor
		 * Arguments: 
		 *     std::string - the name of the color
		 * Purpose: Gets the corresponding mapped color as described in colors.xml
		 * Return Value: Vec3<unsigned char>
		 */
		Vec3<unsigned char> GetColor(std::string str) {
			
			// Check that a value exists
			if( _colorMap.find(str) == _colorMap.end() ) {
				return Vec3<unsigned char>::vec3(0, 0, 0);
			}
			return _colorMap[str];

		}

		/* 
		 * Date: 1/7/16
		 * Function Name: HSLToRGB
		 * Arguments: 
		 *     Vec3 - the color to conver
		 * Purpose: Converts the HSL defined color into an RGB color space
		 * Return Value: void
		 */
		static void HSLToRGB(Vec3<float> &color) {

			float H = color.x;
			float S = color.y / 100.;
			float L = color.z / 100.;

			float C = (1.0 - abs(2 * L - 1)) * S;
			float X = C * (1 - abs(fmodf(H / 60, 2) - 1));
			float m = L - (C / 2.);

			float r, g, b;
			if(H >= 300) {
				r = C;
				g = 0;
				b = X;
			} else if(H >= 240) {
				r = X;
				g = 0;
				b = C;
			} else if(H >= 180) {
				r = 0;
				g = X;
				b = C;
			} else if(H >= 120) {
				r = 0;
				g = C;
				b = X;
			} else if(H >= 60) {
				r = X;
				g = C;
				b = 0;
			} else {
				r = C;
				g = X;
				b = 0;
			}

			color.setValues(255. * (r + m), 255. * (g + m), 255. * (b + m));
			//printf("HSL: %f %f %f to RGB %f %f %f\n", H, S*100, L*100, color.x, color.y, color.z);
		}

		/* 
		 * Date: 1/7/16
		 * Function Name: RGBToHSL
		 * Arguments: 
		 *     Vec3 - the color to conver
		 * Purpose: Converts the RGB defined color into an HSL color space (hue, saturation as a percent, light as a percent)
		 * Return Value: void
		 */
		static void RGBToHSL(Vec3<float> &color) {
			float r = color.x / 255.;
			float g = color.y / 255.;
			float b = color.z / 255.;

			char maxSel;

			float cmax, cmin, delta;

			float H, S, L;

			if( r >= g && r >= b) {
				maxSel = 'r';
				cmax = r;
			} else if( g >= r && g >= b) {
				maxSel = 'g';
				cmax = g;
			} else {
				maxSel = 'b';
				cmax = b;
			}

			cmin = fminf(fminf(r, g), b);
			delta = cmax - cmin;

			L = (cmax + cmin) / 2.;

			if(delta == 0) {
				H = 0.;
				S = 0.;
			} else {
				switch(maxSel) {
					case 'r':
						H = 60 * fmodf((g - b) / delta, 6);
						break;
					case 'g':
						H = 60 * (((b - r) / delta) + 2);
						break;
					case 'b' :
						H = 60 * (((r - g) / delta) + 4);
						break;
					default :
						H = 0;
						break;
				}
				float temp = 2 * L - 1;
				if( temp < 0 ) {
					temp *= -1;
				}
				S = 100. * delta / (1. - temp);
			}

			if( H < 0 ){
				H += 360;
			}

			//printf("RGB: %f %f %f to HSL %f %f %f\n", color.x, color.y, color.z, H, S, L*100);
			color.setValues(H, S, 100*L);
		}

	private:
		std::map<std::string, Vec3<unsigned char> > _colorMap;
		
};
