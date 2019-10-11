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

// Alternative:
//1) 
// #include "opencv2/core/core.hpp"
// ...
// cv::Mat H = cv::findHomography(points1, points2, CV_RANSAC, 5);
// ...
//
// or
// 2) 
// #include "opencv2/core/core.hpp"
// using namespace cv;
// ...
// Mat H = findHomography(points1, points2, CV_RANSAC, 5 );
//...


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
  img= imread("/afs/cern.ch/user/a/azzolini/scratch0/test_CplusplusCode/OpenCV/CloudyGoldenGate.jpg");


  // ?? R U sure you find the image?
  if( img.empty() )
    {
      cout <<  "image not findable or openable... please double check it " << std::endl ;
      return -1;
    }

  // print number of channels in image
  cout << "image rows x columns: " << img.rows() << "x " img.cols() <<  endl;
  cout << "image channels: " << img.channels() <<  endl;

  // -- ex 1 ----------------------
  // ..... Convert our image from BGR to Grayscale format
  Mat gray_img;
  cvtColor( img, gray_img, CV_BGR2GRAY );
  // cvtColor function converts an input image from one color space to another.
  // cvtColor(source image (image), destination image (gray_image)in which we will save the converted image, additional parameter=kind of transformation will be performed)
  // here : CV_BGR2GRAY (because of imread has BGR default channel order in case of color images).

  //...... Write image: img -> location Image2.jpg
  imwrite("/afs/cern.ch/user/a/azzolini/scratch0/test_CplusplusCode/OpenCV/CloudyGoldenGate_transformed_grey.jpg", gray_img);



  // -- ex 2 -----------------------
  // ..... Read single pixel
  int x=img.cols/2; // select a specific pixel(x,y) in the center of the image
  int y=img.rows/2;
  // ... get pixel intensity value

  // ... for a single channel grey scale image (type 8UC1) and pixel coordinates x and y:
  // Scalar intensity = img.at<uchar>(y, x); // the 0-based row index (or y-coordinate) goes first and the 0-based column index (or x-coordinate) follows it.
  // or 
  // Scalar intensity = img.at<uchar>(Point(x, y)); // note the x,y inverted
  // ... for 3 channel image with BGR color ordering (the default format returned by imread):
  // Vec3b intensity = img.at<Vec3b>(y, x);
  // uchar blue = intensity.val[0];
  // uchar green = intensity.val[1];
  // uchar red = intensity.val[2];
  Vec3b p= img.at<Vec3b>(x,y);
  cout << "Pixel value in x="<<x<<", y="<<y<<" is (B,G,R): (" << (int)p[0] << "," <<(int)p[1] << "," << (int)p[2] << ")" << endl;
  float blue =  p.val[0];
  float green = p.val[1];
  float red =   p.val[2];
  cout << "Pixel value in x="<<x<<", y="<<y<<" is (B,G,R): (" << (int)p.val[0] << "," << (int)p.val[1] << "," << (int)p.val[2] << ")" << endl;
  cout << "Pixel value in x="<<x<<", y="<<y<<" is (B,G,R): (" << blue << "," << green << "," << red << ")" << endl;


  // -- ex 3 -----------------------
  // ..... plot histograms of Red, Green, and Blue components.
  int HistR[257] = {0};
  int HistG[257] = {0};
  int HistB[257] = {0};

  for (int i = 0; i < img.rows; i++)
    for (int j = 0; j < img.cols; j++)
      {
	Vec3b intensity = img.at<Vec3b>(Point(j, i));
	int Red =   intensity.val[0];
	int Green = intensity.val[1];
	int Blue =  intensity.val[2];
	HistR[Red]   = HistR[Red]+1;
	HistB[Blue]  = HistB[Blue]+1;
	HistG[Green] = HistG[Green]+1;
      }
  Mat HistPlotR (500, 256, CV_8UC3, Scalar(0, 0, 0));
  Mat HistPlotG (500, 256, CV_8UC3, Scalar(0, 0, 0));
  Mat HistPlotB (500, 256, CV_8UC3, Scalar(0, 0, 0));
  for (int i = 0; i < 256; i=i+2)
    {
      line(HistPlotR, Point(i, 500), Point(i, 500-HistR[i]), Scalar(0, 0, 255),1,8,0);
      line(HistPlotG, Point(i, 500), Point(i, 500-HistG[i]), Scalar(0, 255, 0),1,8,0);
      line(HistPlotB, Point(i, 500), Point(i, 500-HistB[i]), Scalar(255, 0, 0),1,8,0);
    }

  // histogram = calcHist(images, channels, mask, bins, ranges)
  //    1. images   - is the image we want to calculate the histogram of wrapped as a list, so if our image is in variable image we will pass [image],
  //    2. channels - is the the index of the channels to consider wrapped as 
  //                  a list [0] for gray-scale images as there's only one channel or 
  //                  channel lists [0], [1] or [2] for color images if we want to consider the channel green, blue or red respectively)
  //    3. mask     - is a mask to be applied on the image if we want to consider only a specific region
  //                  Essentially, a mask is a uint8  image with the same shape as our original image, where pixels with a value of zero are ignored and pixels with a value greater than zero are included in the histogram computation. Using masks allow us to only compute a histogram for a particular region of an image. For now, we’ll just use a value of None for the mask.
  //    4. bins     - is a list containing the number of bins to use for each channel
  //    5. ranges   - is the range of the possible pixel values which is [0, 256] in case of RGB color space (where 256 is not inclusive).
  // e.g for a gray image: hist = cv2.calcHist([gray], [0], None, [256], [0, 256])
  // e.g color image:

  // -- ex 4 -----------------------
  // ..... plot histograms of Red, Green, and Blue components.
  /// Separate the image in 3 places ( B, G and R )
  //  vector<Mat> planes;
  Mat planes;
  split( img, planes );
  imshow("planes1",planes[0]);//b
  imshow("planes2",planes[1]);//g
  imshow("planes3",planes[2]);//r




  // -- check in real time ---------
  // .... Create a window for display 
  namedWindow( "Display win", WINDOW_AUTOSIZE ); 
  // .... show image in that windown
  imshow("Display win", img);

  namedWindow( "Display trans win", WINDOW_AUTOSIZE );  // window for transformed img
  imshow("Display transf win", gray_img); // show transformed img

  // .... window and display histograms
  //  namedWindow("Red Histogram"); // ? possibly add WINDOW_AUTOSIZE
  namedWindow("Red Histogram", WINDOW_AUTOSIZE);
  namedWindow("Green Histogram");
  namedWindow("Blue Histogram");
  imshow("Red Histogram", HistPlotR);
  imshow("Green Histogram", HistPlotG);
  imshow("Blue Histogram", HistPlotB);


  // -- ex 4 -----------------------
  // ..... select an area of interest( eg. rectangular)
  // Rect: Template class for 2D rectangles
  // Rect_ (_Tp _x, _Tp _y, _Tp _width, _Tp _height) == Coordinates of the top-left corner and width and height
  Rect r(240,212,70,55);
  Mat img_rectang = img(r);
  imshow("zooming into a specific area", img_rectang);




  //..... // Wait for a keystroke in the window. not practical if working offline
  int c = waitKey(0); // rem: show image. send key input -- it may take few ms. E.g.2000

  return 0;




}
