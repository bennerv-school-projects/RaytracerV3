#pragma once

#include <cassert>

#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 3/4/17
 * Classname: ImagePlane
 * Purpose: An ImagePlane used to define the plane the image is projected onto or where the rays are shot from the camera to the image plane
 */
class ImagePlane {
    
    public :
    
    /*
     * Date: 3/4/17
     * Function Name: Point
     * Arguments:
     *      Vec3<float> - the top left corner of the ImagePlane
     *      float       - the length of the ImagePlane
     *      float       - the height of the ImagePlane
     * Purpose: Constructor
     * Return Value: void (Constructor)
     */
    ImagePlane(Vec3<float> corner, float length, float height) {
        _corner = corner;
        _length = length;
        _height = height;
        
        // Make sure the length/width/heights are greater than zero (otherwise we will have negative units for pixel length/height)
        assert(_length > 0);
        assert(_height > 0);
        
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetCorner
     * Arguments:
     *      void
     * Purpose: Gets the upper lefthand corner of the ImagePlane
     * Return Value: Vec3<float>
     */
    Vec3<float> GetCorner() {
        return _corner;
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetHeight
     * Arguments:
     *      void
     * Purpose: Gets the height of the ImagePlane
     * Return Value: float
     */
    float GetHeight() {
        return _height;
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetLength
     * Arguments:
     *      void
     * Purpose: Gets the length of the ImagePlane
     * Return Value: float
     */
    float GetLength() {
        return _length;
    }

    

    private :
    
    Vec3<float> _corner; // upper left corner
    float _length; // +/- x value
    float _height; // +/- y value
};
