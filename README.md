# panorama-construction

This repository contains a C++ program for creating a panorama by stitching two images together. The code uses the Imagine++ library for graphics and image processing. The program allows the user to select corresponding points in two images and computes a homography matrix to align and merge the images into a single panoramic image.

## Project Overview

The code consists of several functions that work together to create a panorama:

#### `getClicks`
- This function records user clicks on two images until a right-click is detected, storing the clicked points in two separate vectors.

#### `getHomography`
- This function calculates a homography matrix that aligns the corresponding points from the two images. It uses a minimum of four point matches to compute the matrix.

#### `panorama`
- The `panorama` function creates a panoramic image by stitching the two input images together using the computed homography. It calculates the bounding box of the panorama and blends the overlapping regions to create a smooth transition between the images.

#### `growTo`
- A utility function to update the minimum and maximum coordinates of a rectangle to include a given point.

#### `main`
- The main function loads two images, displays them in separate windows, and allows the user to select corresponding points. It then computes the homography and creates the panoramic image.


![Alt Text](path/to/image.png)
