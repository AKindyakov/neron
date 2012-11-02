/**
 ** autor   Alexander K
 ** date    08.07.2012.13.33
 ** BRIEF   contour implementation
*/

//==============================================================================
#include <iostream>
#include <iterator>
#include <cmath>
//==============================================================================
#include "include/geometry.h"
#include "include/math.h"
//==============================================================================

namespace G = Geometry;

G::Contour::Contour(const G::Point2f& center, 
                    const std::list<G::Reduced_vector>& polinom)
{

}

G::Contour::Contour(const std::list<G::Point2f>& polinom)
{}

void G::Contour::push_back(const G::Reduced_vector& pt)
{
   
}

void G::Contour::turn(float angle)
{
   
}

bool G::Contour::belongingPoint(const G::Point2f& point)const
{
   throw G::Geometry_error(
      "functoion Contour::belongingPoint not defined" );
   return false;
}

bool G::Contour::shapeIntersection(const G::I_Shape& inputShape,
                                   std::list<G::Point2f>* genPoint)
{
   return G::shapeIntersection(*this, inputShape, genPoint);
}

float G::Contour::getDistance(const Point2f& pt, Point2f* close_pt)const
{
   return -1;
}

void G::Contour::getX(float y, std::vector<float>* x)const
{}

void G::Contour::getY(float x, std::vector<float>* y)const
{}

int G::Contour::convertToContour()
{
   std::list<G::Reduced_vector>::const_iterator firstIt = m_vec.begin();
   std::list<G::Reduced_vector>::const_iterator secondIt = m_vec.begin();
   ++secondIt;
   
   /*
    * If prev pair of vectours and current pair of vectors turn 
    * to the equal directions (clockwise or conterclockwise)
    * both prev vector proud and current vector proud will be equal sign.
    * If sign chege we will write current convex_contour to contours list
    * and crete new convex_contour and start to fiil one.
    */
   
   float last_vector_proud = firstIt->vector_proud(*secondIt);
   float now_vector_proud = 0;
   
   //prepare runing start pt
   G::Point2f runingStartPt(m_center);
   runingStartPt = runingStartPt + *firstIt;
   
   // prepare first contour
   G::Convex_contour curr_cont(m_center);
   curr_cont.push_back(*firstIt);
   
   for ( ; secondIt != m_vec.end(); ++firstIt, ++secondIt )
   {
      now_vector_proud = firstIt->vector_proud(*secondIt);
      if ( now_vector_proud * last_vector_proud > 0)
      {
         curr_cont.push_back(*secondIt);
         
      }
      else /* sign have been changed */
      {
         m_contours.push_back(curr_cont);
         curr_cont.m_vec.resize(0);
         curr_cont.m_center = runingStartPt;
         curr_cont.push_back(*secondIt);
      }
      runingStartPt = runingStartPt + *secondIt;
      last_vector_proud = now_vector_proud;
   }
   m_contours.push_back(curr_cont); // write last convex_contour
}

bool G::Contour::id_converted()
{}

