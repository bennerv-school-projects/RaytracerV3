#pragma once

#include "tinyxml2.h"

//TODO <BMV> Parse gradient information

/*
* Author: Ben Vesel
* Date: 4/12/17
* Classname: Config
* Purpose: A class that contains configuration for raytracing
*/
class Config {

	public:
		Config(std::string fileName) {
			tinyxml2::XMLDocument doc;
			doc.LoadFile(fileName.c_str());

			// Parse the xml document
			if (doc.Error()) {
				std::cout << "There was an error parsing " << fileName << std::endl;
				doc.PrintError();
				exit(1);
			}

			// Iterate through the xml elements for the configuration section
			tinyxml2::XMLElement * rootElement = doc.FirstChildElement();
			while (rootElement && strncmp(rootElement->Value(), "configuration", 13)) {
                rootElement = rootElement->NextSiblingElement();
            }
            
            
            if(rootElement) {
            
				// Go through each of the configuration sections
				tinyxml2::XMLElement * configElement = rootElement->FirstChildElement();
				while (configElement) {

					std::string str(configElement->GetText());
					std::transform(str.begin(), str.end(), str.begin(), ::toupper);

					// Set default values
					if (!strncmp(configElement->Value(), "anti_aliasing", 13)) {
						if (!strncmp(str.c_str(), "TRUE", 4)) {
							_antiAliasing = true;
						}
					}
					else if (!strncmp(configElement->Value(), "ambient_light", 13)) {
						_ambientLight = (float)atof(str.c_str());
						if (_ambientLight == 0) {
							_ambientLight = 0.2f;
						}
					}
					else if (!strncmp(configElement->Value(), "image_length", 12)) {
						_imageLength = atoi(str.c_str());
						if (_imageLength < 512) {
							std::cout << "Image length must be at least 512" << std::endl;
							_imageLength = 512;
						}
					}
					else if (!strncmp(configElement->Value(), "image_height", 12)) {
						_imageHeight = atoi(str.c_str());
						if (_imageHeight < 512) {
							std::cout << "Image height must be at least 512" << std::endl;
							_imageHeight = 512;
						}
					}
					else if (!strncmp(configElement->Value(), "gamma_correction", 16)) {
						if (!strncmp(str.c_str(), "TRUE", 4)) {
							_gammaCorrect = true;
						}
					} 
					else if (!strncmp(configElement->Value(), "anaglyph", 8)) {
						if (!strncmp(str.c_str(), "TRUE", 4)) {
							_isAnaglyph = true;
						}
					} else if(!strncmp(configElement->Value(), "normal_correction", 17)) {
						if(!strncmp(str.c_str(), "TRUE", 4)) {
							_normalCorrection = true;
						} else {
							_normalCorrection = false;
						}
					}

					// Get the next sibling element
					configElement = configElement->NextSiblingElement();
				}

				// Get the next sibling element
				rootElement = rootElement->NextSiblingElement();
			}
		}

		/*
		* Date: 4/12/17
		* Function Name: IsAnaglyph
		* Arguments:
		*     void
		* Purpose: Returns true if anaglyph mode is activated
		* Return Value: bool
		*/
		bool IsAnaglyph() {
			return _isAnaglyph;
		}

		/*
		* Date: 4/12/17
		* Function Name: IsAntialiased
		* Arguments:
		*     void
		* Purpose: Returns true if image is antialiased
		* Return Value: bool
		*/
		bool IsAntialiased() {
			return _antiAliasing;
		}

		/*
		* Date: 4/12/17
		* Function Name: GammaCorrect
		* Arguments:
		*     void
		* Purpose: Returns true if gamma correction is activated
		* Return Value: bool
		*/
		bool GammaCorrect() {
			return _gammaCorrect;
		}

	   /*
		* Date: 4/14/17
		* Function Name: NormalCorrect
		* Arguments:
		*     void
		* Purpose: Returns true if normal correction is activated
		* Return Value: bool
		*/
		bool NormalCorrect() {
			return _normalCorrection;
		}

		/*
		* Date: 4/12/17
		* Function Name: GetPixelLength
		* Arguments:
		*     void
		* Purpose: Returns the pixel length of the image
		* Return Value: int
		*/
		int GetPixelLength() {
			return _imageLength;
		}

		/*
		* Date: 4/12/17
		* Function Name: GetPixelHeight
		* Arguments:
		*     void
		* Purpose: Returns the pixel height of the image
		* Return Value: int
		*/
		int GetPixelHeight() {
			return _imageHeight;
		}

		/*
		* Date: 4/12/17
		* Function Name: GetAmbientLight
		* Arguments:
		*     void
		* Purpose: Returns the ambient light value of the image
		* Return Value: float
		*/
		float GetAmbientLight() {
			return _ambientLight;
		}


	private:
		bool _antiAliasing = false;
		bool _gammaCorrect = false;
		bool _isAnaglyph = false;
		bool _normalCorrection = true;
		float _ambientLight = 0.2f;
		int _imageLength = 512;
		int _imageHeight = 512;


};
