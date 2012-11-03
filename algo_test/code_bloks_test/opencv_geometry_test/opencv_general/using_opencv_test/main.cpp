#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "include/geometry.h"
#include "include/geometry_drow.h"
#include "tests/geometry/contour/createSamples.h"

int main()
{
   cv::Mat res(300, 300, CV_8UC1, cv::Scalar(1));

    Geometry::Contour cont = createStarContour(res);

    drowShape(cont,&res,cv::Scalar(233), 1,cv::Scalar(233), 3);

    cv::namedWindow("show", 0);

   while (1)
   {
      cv::imshow("show", res);
      char c = cv::waitKey(100);
      if (c == 27) break; // if was key "Esc" break;
   }
   cv::destroyWindow("show");
    std::cout << "Hello world!" << std::endl;
    return 0;
}
