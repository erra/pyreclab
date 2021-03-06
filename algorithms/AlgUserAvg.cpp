#include "AlgUserAvg.h"

using namespace std;


AlgUserAvg::AlgUserAvg( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos )
{
}

int AlgUserAvg::train()
{
   for( size_t u = 0 ; u < m_ratingMatrix.users() ; ++u )
   {
      string userId = m_ratingMatrix.userId( u );
      double sumbyrow = 0;
      int countbyrow = m_ratingMatrix.sumRow( u, sumbyrow );
      if( 0 < countbyrow )
      {
         m_meanRatingByUser[userId] = sumbyrow/countbyrow;
      }

      if( !m_running )
      {
         return STOPPED;
      }
   }

   return FINISHED;
}

double AlgUserAvg::predict( string& userId, string& itemId )
{
   double p = -1;
   if( m_meanRatingByUser.find( userId ) != m_meanRatingByUser.end() )
   {
      p = m_meanRatingByUser[userId];
   }
   return p > 0 ? p : m_globalMean;
}

double AlgUserAvg::predict( size_t userrow, size_t itemcol )
{
   double p = -1;
   string userId = m_ratingMatrix.userId( userrow );
   if( !userId.empty() )
   {
      p = m_meanRatingByUser[userId];
   }
   return p > 0 ? p : m_globalMean;
}


