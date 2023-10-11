// Imagine++ project
// Project:  Panorama
// Author:   Pascal Monasse & Mahdi Ranjbar
// Date:     2023/10/11

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <Imagine/LinAlg.h>
#include <vector>
#include <sstream>
using namespace Imagine;
using namespace std;

// Record clicks in two images, until right button click
void getClicks(Window w1, Window w2, vector<IntPoint2>& pts1, vector<IntPoint2>& pts2) {
    
    // ------------- TODO/A completer ----------
    
    Window const WINDOW1 = w1;
    Window const WINDOW2 = w2;
    
    int button;
    
    int sw;
    IntPoint2 p;
    Window w;
    
    while(true){
        button = anyGetMouse(p, w, sw);
        
        if (button==3) break; // If user right clicks
        
        if (WINDOW1 == w){
            pts1.push_back(std::move(p));
            cout << "Point added to window 1" << endl;
        }
        if (WINDOW2 == w){
            pts2.push_back(std::move(p));
            cout << "Point added to window 2" << endl;
        }
        if (pts1.size()==4 && pts2.size()==4){
            cout<<"\n"<<std::string(10,'-')
            << " Please right click in order to generate the panorama! "
            << std::string(10,'-')<<"\n"<<endl;
        }
    }
}


// Return homography compatible with point matches
Matrix<float> getHomography(const vector<IntPoint2>& pts1, const vector<IntPoint2>& pts2) {
    int n = (int) min(pts1.size(), pts2.size());
    if(n<4) {
        cout << "Not enough correspondences: " << n << endl;
        return Matrix<float>::Identity(3);
    }
    Matrix<double> A(2*n,8);
    Vector<double> B(2*n);
    
    
    // ------------- TODO ------------
    
    // Loop through correspondences to build the A matrix and B vector
    for (int i = 0; i < n; i++) {
        double x1 = pts1[i].x();
        double y1 = pts1[i].y();
        double x2 = pts2[i].x();
        double y2 = pts2[i].y();
        
        
        // Populate the A matrix
        A(2 * i, 0) = -x1;
        A(2 * i, 1) = -y1;
        A(2 * i, 2) = -1;
        A(2 * i, 3) = 0;
        A(2 * i, 4) = 0;
        A(2 * i, 5) = 0;
        A(2 * i, 6) = x1 * x2;
        A(2 * i, 7) = y1 * x2;
        
        A(2 * i + 1, 0) = 0;
        A(2 * i + 1, 1) = 0;
        A(2 * i + 1, 2) = 0;
        A(2 * i + 1, 3) = -x1;
        A(2 * i + 1, 4) = -y1;
        A(2 * i + 1, 5) = -1;
        A(2 * i + 1, 6) = x1 * y2;
        A(2 * i + 1, 7) = y1 * y2;
        
        
        // Populate the B vector
        B[2 * i] = -x2;
        B[2 * i + 1] = -y2;
    }
    
    // Solve the linear system A * x = B
    B = linSolve(A, B);
    
    // Create the 3x3 homography matrix H
    Matrix<float> H(3, 3);
    H(0,0)=B[0]; H(0,1)=B[1]; H(0,2)=B[2];
    H(1,0)=B[3]; H(1,1)=B[4]; H(1,2)=B[5];
    H(2,0)=B[6]; H(2,1)=B[7]; H(2,2)=1;
    
    // Sanity check
    for(size_t i=0; i<n; i++) {
        float v1[]={(float)pts1[i].x(), (float)pts1[i].y(), 1.0f};
        float v2[]={(float)pts2[i].x(), (float)pts2[i].y(), 1.0f};
        Vector<float> x1(v1,3);
        Vector<float> x2(v2,3);
        
        x1 = H*x1;
        
        cout << x1[1]*x2[2]-x1[2]*x2[1] << ' '
        << x1[2]*x2[0]-x1[0]*x2[2] << ' '
        << x1[0]*x2[1]-x1[1]*x2[0] << endl;
    }
    return H;
}


// Grow rectangle of corners (x0,y0) and (x1,y1) to include (x,y)
void growTo(float& x0, float& y0, float& x1, float& y1, float x, float y) {
    if(x<x0) x0=x;
    if(x>x1) x1=x;
    if(y<y0) y0=y;
    if(y>y1) y1=y;
}


// Panorama construction
void panorama(const Image<Color,2>& I1, const Image<Color,2>& I2, Matrix<float> H) {
    Vector<float> v(3);
    float x0=0, y0=0, x1=I2.width(), y1=I2.height();
    
    v[0]=0; v[1]=0; v[2]=1;
    v=H*v; v/=v[2];
    growTo(x0, y0, x1, y1, v[0], v[1]);
    
    v[0]=I1.width(); v[1]=0; v[2]=1;
    v=H*v; v/=v[2];
    
    growTo(x0, y0, x1, y1, v[0], v[1]);
    
    v[0]=I1.width(); v[1]=I1.height(); v[2]=1;
    v=H*v; v/=v[2];
    growTo(x0, y0, x1, y1, v[0], v[1]);
    
    v[0]=0; v[1]=I1.height(); v[2]=1;
    v=H*v; v/=v[2];
    growTo(x0, y0, x1, y1, v[0], v[1]);
    
    cout << "x0 x1 y0 y1=" << x0 << ' ' << x1 << ' ' << y0 << ' ' << y1<<endl;
    
    // Panorama bounding box
    int panoramaWidth = int(x1 - x0);
    int panoramaHeight = int(y1 - y0);
    
    Image<Color> panoramaImage(panoramaWidth, panoramaHeight);
    setActiveWindow( openWindow(panoramaImage.width(), panoramaImage.height()) );
    panoramaImage.fill(WHITE);
    
    
    // ------------- TODO -------------
    
    
    // Copy I2 to the panorama (it remains fixed as it is considered as the reference image)
    for (int y = 0; y < I2.height(); y++) {
        for (int x = 0; x < I2.width(); x++) {
            panoramaImage(x - static_cast<int>(x0), y - static_cast<int>(y0)) = I2(x, y);
        }
    }
    
    
    // Stitch I1 onto the panorama using the homography
    for (int y = 0; y < panoramaHeight; y++) {
        for (int x = 0; x < panoramaWidth; x++) {
            
            // Calculate the inverse transformation
            Vector<float> v(3);
            v[0] = float(x) + x0;
            v[1] = float(y) + y0;
            v[2] = 1;
            Vector<float> transformed = inverse(H) * v;
            
            int x1 = static_cast<int>(transformed[0] / transformed[2]);
            int y1 = static_cast<int>(transformed[1] / transformed[2]);
            
            Color color;
            
            if (x1 >= 0 && x1 < I1.width() && y1 >= 0 && y1 < I1.height()) {
                
                if(panoramaImage(x, y) == Color(255,255,255)){ // The case which is not overlapping
                    
                    color = I1(x1, y1);
                    panoramaImage(x, y) = color;
                    
                } else { // The case which is overlapping: blend the colors if the pixel
                    Color c1 = panoramaImage(x, y);
                    Color c2 = I1(x1, y1);
                    
                    
                    color = Color((int(c1.r())+int(c2.r()))/2,(int(c1.g())+int(c2.g()))/2,(int(c1.b())+int(c2.b()))/2);
                    panoramaImage(x, y) = color;
                }
            }
        }
    }
    
    display(panoramaImage,0,0);
}


// Main function
int main(int argc, char* argv[]) {
    const char* s1 = argc>1? argv[1]: srcPath("image0006.jpg");
    const char* s2 = argc>2? argv[2]: srcPath("image0007.jpg");
    
    // Load and display images
    Image<Color> I1, I2;
    if( ! load(I1, s1) ||
       ! load(I2, s2) ) {
        cerr<< "Unable to load the images" << endl;
        return 1;
    }
    Window w1 = openWindow(I1.width(), I1.height(), s1);
    display(I1,0,0);
    Window w2 = openWindow(I2.width(), I2.height(), s2);
    setActiveWindow(w2);
    display(I2,0,0);
    
    // Get user's clicks in images
    vector<IntPoint2> pts1, pts2;
    
    getClicks(w1, w2, pts1, pts2);
    
    vector<IntPoint2>::const_iterator it;
    cout << "pts1="<<endl;
    for(it=pts1.begin(); it != pts1.end(); it++)
        cout << *it << endl;
    cout << "\n pts2="<<endl;
    for(it=pts2.begin(); it != pts2.end(); it++)
        cout << *it << endl;
    
    // Compute homography
    Matrix<float> H = getHomography(pts1, pts2);
    cout << "H=" << H/H(2,2);
    
    //Apply homography
    panorama(I1, I2, H);
    
    endGraphics();
    return 0;
}
