#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

#include <unistd.h>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
 
const int MAX_FEATURES = 1000;
const float GOOD_MATCH_PERCENT = 0.15f;
 
void alignImages(Mat &im1, Mat &im2, Mat &im1Reg, Mat &h)
{
  // Convert images to grayscale
  Mat im1Gray, im2Gray;
  cvtColor(im1, im1Gray, cv::COLOR_BGR2GRAY);
  cvtColor(im2, im2Gray, cv::COLOR_BGR2GRAY);
 
  // Variables to store keypoints and descriptors
  std::vector<KeyPoint> keypoints1, keypoints2;
  Mat descriptors1, descriptors2;
 
  // Detect ORB features and compute descriptors.
  Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
  orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
  orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);
 
  // Match features.
  std::vector<DMatch> matches;
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
  matcher->match(descriptors1, descriptors2, matches, Mat());
 
  // Sort matches by score
  std::sort(matches.begin(), matches.end());
 
  // Remove not so good matches
  const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
  matches.erase(matches.begin()+numGoodMatches, matches.end());
 
  // Draw top matches
  Mat imMatches;
  drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);
  imwrite("matches.jpg", imMatches);
 
  // Extract location of good matches
  std::vector<Point2f> points1, points2;
 
  for( size_t i = 0; i < matches.size(); i++ )
  {
    points1.push_back( keypoints1[ matches[i].queryIdx ].pt );
    points2.push_back( keypoints2[ matches[i].trainIdx ].pt );
  }
 
  // Find homography
  h = findHomography( points1, points2, RANSAC );
 
  // Use homography to warp image
  warpPerspective(im1, im1Reg, h, im2.size());
}



bool check_format(std::string path, std::string format)
{
  FILE* file = fopen((path+format).c_str(), "r");
  if (file == nullptr)
    return false;
  else
  {
    fclose(file);
    return true;
  }
}



int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "input image name only\n";
    return 1;
  }
  // Read reference
  string refFilename("../images/ref_normalized.jpg"); 
  cout << "Reading reference image : " << refFilename << "\n\n"; 
  Mat imReference = imread(refFilename);
  // Registered image will be resotred in imReg. 
  // The estimated homography will be stored in h. 
  Mat imReg, h;
 

  
  for (size_t i = 1; i < argc; i++)
  {
    std::string file = "../images/";
    file.append(argv[i]);

    Mat im;
    if(check_format(file, ".jpg"))
      im = imread(file+ ".jpg");
    else if(check_format(file, ".png"))
      im = imread(file+ ".png");
    else
    {
      std::cout << "Invalid format file " << file << std::endl;
      return 1;
    }

    // Align images
    cout << "Aligning images " << file << "-align.jpg\n"; 
    alignImages(im, imReference, imReg, h);
  
  
    imwrite(file+"-align.jpg", imReg);

    // Print estimated homography
    cout << "Estimated homography : \n" << h << "\n\n"; 
  }
}
