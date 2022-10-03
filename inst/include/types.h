#ifndef TYPES_H
#define TYPES_H

#include <boost/date_time.hpp>

typedef boost::gregorian::date bdate;
typedef boost::gregorian::month_iterator month_iterator;
typedef boost::gregorian::date_duration dd;
typedef std::vector<boost::gregorian::date> bdate_vector;

#endif
