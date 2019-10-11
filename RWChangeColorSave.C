// -- include the iostream to facilitate console line output and input. 
#include <iostream>
using namespace std;

//#include <string> 

// -- core section,   as here are defined the basic building blocks of the library
// -- highgui module, as this contains the functions for input and output operations
// -- imgproc module for image processing, it contains the cvtColor function 
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// To avoid data structure and function name conflicts with other libraries, OpenCV has its own namespace: cv.
using namespace cv;


//=========================
int main()
{

  //..... Read images -> img
  //// .. create a Mat object that will store the data of the loaded image
  ////    create the matrix header (containing information such as the size of the matrix, the method used for storing, at which address is the matrix stored, ....)
  ////    The matrix header size is constant
  Mat img;

  //// .. create a pointer to the matrix containing the pixel values (taking any dimensionality depending on the method chosen for storing). 
  ////    The size of the matrix itself may vary from image to image. can become rather large.
  //  Mat img= imread("/afs/cern.ch/user/a/azzolini/scratch0/test_CplusplusCode/CloudyGoldenGate_grayscale.jpg");
  img= imread("/afs/cern.ch/user/a/azzolini/scratch0/test_CplusplusCode/CloudyGoldenGate.jpg");


  // ?? R U sure you find the image?
  if( img.empty() )
    {
      cout <<  "image not findable or openable... please double check it " << std::endl ;
      return -1;
    }


  // -- ex 1 ----------------------
  // ..... Convert our image from BGR to Grayscale format
  Mat gray_img;
  cvtColor( img, gray_img, CV_BGR2GRAY );
  // cvtColor function converts an input image from one color space to another.
  // cvtColor(source image (image), destination image (gray_image)in which we will save the converted image, additional parameter=kind of transformation will be performed)
  // here : CV_BGR2GRAY (because of imread has BGR default channel order in case of color images).

  //...... Write image: img -> location Image2.jpg
  imwrite("/afs/cern.ch/user/a/azzolini/scratch0/test_CplusplusCode/CloudyGoldenGate_transformed_grey.jpg", gray_img);



  // -- ex 2 -----------------------
  //..... Read single pixel
  int x=img.cols/2; // select a specific pixel(x,y) in the center of the image
  int y=img.rows/2;
  // ... get pixel intensity value

  // ... for a single channel grey scale image (type 8UC1) and pixel coordinates x and y:
  // Scalar intensity = img.at<uchar>(y, x);
  // ... for 3 channel image with BGR color ordering (the default format returned by imread):
  // Vec3b intensity = img.at<Vec3b>(y, x);
  // uchar blue = intensity.val[0];
  // uchar green = intensity.val[1];
  // uchar red = intensity.val[2];
  Vec3b p= img.at<Vec3b>(x,y);
  cout << "Pixel value in x="<<x<<", y="<<y<<" is (B,G,R): (" << (int)p[0] << "," <<(int)p[1] << "," << (int)p[2] << ")" << endl;



  // -- check in real time ---------

  // .... Create a window for display 
  namedWindow( "Display win", WINDOW_AUTOSIZE ); 
  //..... show image in that windown
  imshow("Display win", img);

  namedWindow( "Display trans win", WINDOW_AUTOSIZE );  // window for transformed img
  imshow("Display transf win", gray_img); // show transformed img


  //..... // Wait for a keystroke in the window. not practical if working offline
  int c = waitKey(0); // rem: show image. send key input -- it may take few ms. E.g.2000


}
