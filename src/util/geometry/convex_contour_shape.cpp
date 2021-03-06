/**
 ** autor   Alexander K
 ** date    25.08.2012.10.47
 ** BRIEF   convex contour implementation
*/

//=============================================================================
#include <iostream>
#include <iterator>
#include <cmath>
#include <cfloat>
//=============================================================================
#include "include/util/geometry/geometry.h"
#include "include/util/math/math.h"
//=============================================================================

namespace G = Geometry;
G::Convex_contour::Convex_contour(const G::Point2f& center)
{
   m_center = center;
}

G::Convex_contour::Convex_contour(const G::Point2f& center,
                                  const std::list<G::Reduced_vector>& polinom)
                  : m_vec(polinom)
//                    I_Shape(center)
{
   m_center = center;
}

G::Convex_contour::Convex_contour(const std::list<G::Point2f>& polinom)
{
   std::list<G::Point2f>::const_iterator polIt = polinom.begin();
   m_center = *polIt;
   ++polIt;
   std::cout << "create convex contour:\n   "
             << m_center.x << " " << m_center.y << std::endl;
   for (std::list<G::Point2f>::const_iterator prevPolIt = polinom.begin();
                   polIt != polinom.end(); ++polIt, ++prevPolIt)
   {
      m_vec.push_back(Geometry::Reduced_vector( polIt->x - prevPolIt->x,
                                                polIt->y - prevPolIt->y) );
      std::cout << "pt: " << polIt->x - prevPolIt->x << " "
                          << polIt->y - prevPolIt->y << std::endl;
   }
}


/*G::Convex_contour::Convex_contour(const G::Contour& cont)
                    : m_vec(cont.getVectorList())
{
    m_center = cont.getCenter();
}*/

void G::Convex_contour::push_back(float x, float y)
{
   m_vec.push_back(G::Reduced_vector(x,y));
}

void G::Convex_contour::push_back(const G::Reduced_vector& pt)
{
   m_vec.push_back(pt);
}

void G::Convex_contour::turn(float angle)
{
   float sinPhi = std::sin(angle);
   float cosPhi = std::cos(angle);
   float x, y;
   for (std::list<G::Reduced_vector>::iterator vecIt = m_vec.begin();
            vecIt != m_vec.end(); ++vecIt)
   {
      x = vecIt->x*cosPhi - vecIt->y*sinPhi;
      y = vecIt->x*sinPhi + vecIt->y*cosPhi;
      vecIt->x = x;
      vecIt->y = y;
   }
}

G::Point2f G::Convex_contour::getEnd()
{
   G::Point2f endPt(m_center);
   for (std::list<G::Reduced_vector>::iterator vecIt = m_vec.begin();
            vecIt != m_vec.end(); ++vecIt)
   {
      endPt = endPt + *vecIt;
   }
   return endPt;
}

G::constReducedVectorIterator G::Convex_contour::cbeginIt()const
{
   return this->m_vec.begin();
}

G::reducedVectorIterator G::Convex_contour::beginIt()
{
   return this->m_vec.begin();
}

G::constReducedVectorIterator G::Convex_contour::cendIt()const
{
   return this->m_vec.end();
}

G::reducedVectorIterator G::Convex_contour::endIt()
{
   return this->m_vec.end();
}

bool G::Convex_contour::shapeIntersection(const G::I_Shape& inputShape,
                                         std::list<G::Point2f>* genPoint)
{
   return G::shapeIntersection(*this, inputShape, genPoint);
}

bool G::Convex_contour::insidePoint(const G::Point2f& point)const
{
   return false;
}
bool G::Convex_contour::belongingPoint(const G::Point2f& point)const
{
    return false;
}

float G::Convex_contour::getDistance(const G::Point2f& pt, Point2f* close_pt)const
{
   std::list<G::Reduced_vector>::const_iterator vecIt = m_vec.begin();
   float minDist = FLT_MAX;
   float dist = 0;
   std::vector<G::Point2f> closePt;
   // runing buffer for searching
      // init
   std::vector<G::Point2f> runPts(3,m_center);
   runPts[1] = m_center + *(vecIt);

   std::vector<G::Point2f>::iterator runIt = runPts.begin();
   std::vector<G::Point2f>::iterator nextIt = runPts.begin();
   ++nextIt;

   /*
    * m_center   m_center+m_vec[0]    <not significance>
    *    ^               ^                   ^
    * runPts[0]       runPts[1]           runPts[2]
    *    ^               ^                   ^
    *    |               |                   |
    *  runIt           nextIt               prev
    */

   for ( ; vecIt != m_vec.end(); ++vecIt, ++runIt, ++nextIt)
   {
      if ( runIt == runPts.end() )
         runIt = runPts.begin();
      if ( nextIt == runPts.end() )
         nextIt = runPts.begin();

      *(nextIt) = *runIt + *vecIt;
      float dist = G::distance(*runIt, pt);
      if ( dist < minDist )
      {
         minDist = dist;
         if ( nextIt + 1 == runPts.end() )
            closePt[0] = runPts[0];
         else
            closePt[0] = *(nextIt + 1);

         closePt[1] = *runIt;
         closePt[2] = *nextIt;
      }
   }
   // and now are needed to check end point and m_center point because
   // we missed it in loop
   ++runIt;
   ++nextIt;
   // ----- carry out it to function
   if ( runIt == runPts.end() )
      runIt = runPts.begin();
   if ( nextIt == runPts.end() )
      nextIt = runPts.begin();
   // -----
   *(nextIt) = m_center;
   dist = G::distance(*runIt, pt);
   if ( dist < minDist )
   {
      minDist = dist;
      if ( nextIt + 1 == runPts.end() )
         closePt[0] = runPts[0];
      else
         closePt[0] = *(nextIt + 1);

         closePt[1] = *runIt;
         closePt[2] = *nextIt;
   }
   ++runIt;
   ++nextIt;
   // ----- carry out it to function
   if ( runIt == runPts.end() )
      runIt = runPts.begin();
   if ( nextIt == runPts.end() )
      nextIt = runPts.begin();
   // -----
   *(nextIt) = m_center + m_vec.front();
   dist = G::distance(*runIt, pt);
   if ( dist < minDist )
   {
      minDist = dist;
      if ( nextIt + 1 == runPts.end() )
         closePt[0] = runPts[0];
      else
         closePt[0] = *(nextIt + 1);

         closePt[1] = *runIt;
         closePt[2] = *nextIt;
   }
   // and then are needed to find close point in 2 intervals
   G::Interval inter1(closePt[0], closePt[1]);
   G::Interval inter2(closePt[1], closePt[2]);
   G::Point2f pt1;
   G::Point2f pt2;
   dist = inter1.getDistance(pt, &pt1);
   minDist = inter2.getDistance(pt, &pt2);
   if ( dist < minDist )
   {
      minDist = dist;
      *close_pt = pt1;
   }
   else
   {
      *close_pt = pt2;
   }
   return minDist;
}

void G::Convex_contour::getX(float y, std::vector<float>* x)const
{
}
void G::Convex_contour::getY(float x, std::vector<float>* y)const
{
}
