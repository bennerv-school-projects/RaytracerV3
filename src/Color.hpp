#pragma once

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
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
		Color() {}

		Color(std::string fileName) {
			tinyxml2::XMLDocument doc;
			doc.LoadFile(fileName.c_str());

			if(doc.Error()) {
				cout << "There was an error parsing " << fileName << endl;
				doc.PrintError();
				exit(1);
			}

			// Iterate through the xml elements until we find "object" section
			tinyxml2::XMLElement * colorParent = doc.FirstChildElement();
			while( colorParent && strncmp(colorParent->Value(), "objects", 7) ) {
				//cout << "Element text " << colorParent->Value() << endl;
				colorParent = colorParent->NextSiblingElement();
			}

			if( !colorParent ) {
				cout << "Failed to find the object portion in the xml file.  Exiting" << endl;
				exit(10);
			}

			// Iterate through the sml elements until we find "colors" section
			colorParent = colorParent->FirstChildElement();
			while( colorParent && strncmp(colorParent->Value(), "colors", 6) ) {
				//cout << "Inner element text " << colorParent->Value() << endl;
				colorParent = colorParent->NextSiblingElement();
			}

			if( !colorParent ) {
				cout << "Failed to find the colors portion int he xml file.  Exiting." << endl;
				exit(10);
			}

			tinyxml2::XMLElement * color = colorParent->FirstChildElement();

			// Go through all the custom colors and map them
			while( color ) {
				int r = 0, g = 0, b = 0;
				bool addColor = true;
				char * colorName;

				// Query the attributes
				color->QueryIntAttribute("r", &r);
				color->QueryIntAttribute("g", &g);
				color->QueryIntAttribute("b", &b);

				colorName = (char *)color->Attribute("name");
				if(colorName == 0) {
					cout << "There is not a name attribute for one of the colors." << endl;
					addColor = false;
				}

				color = color->NextSiblingElement();

				// Add the color to the map
				if( addColor ) {
					std::string str(colorName);
					_colorMap[str] = Vec3<unsigned char>::vec3(r, g, b);
				}
			}
		}

		/* 
		 * Date: 1/7/16
		 * Function Name: GetColor
		 * Arguments: 
		 *     std::string - the name of the color
		 * Purpose: Gets the corresponding mapped color as described in Objects.xml
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
			float S = color.y / 100.f;
			float L = color.z / 100.f;

			float C = (1.0f - abs(2 * L - 1)) * S;
			float X = C * (1 - abs(fmodf(H / 60.f, 2.f) - 1));
			float m = L - (C / 2.f);

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

			color.setValues(255.f * (r + m), 255.f * (g + m), 255.f * (b + m));
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
			float r = color.x / 255.f;
			float g = color.y / 255.f;
			float b = color.z / 255.f;

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

			L = (cmax + cmin) / 2.f;

			if(delta == 0) {
				H = 0.f;
				S = 0.f;
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
				S = 100.f * delta / (1.f - temp);
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
