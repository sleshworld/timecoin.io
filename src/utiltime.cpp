// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/time-config.h"
#endif

#include "tinyformat.h"
#include "utiltime.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

using namespace std;

static int64_t nMockTIMECoin = 0;  //! For unit testing

int64_t GetTIMECoin()
{
    if (nMockTIMECoin) return nMockTIMECoin;

    time_t now = time(NULL);
    assert(now > 0);
    return now;
}

void SetMockTIMECoin(int64_t nMockTIMECoinIn)
{
    nMockTIMECoin = nMockTIMECoinIn;
}

int64_t GetTIMECoinMillis()
{
    int64_t now = (boost::posix_time::microsec_clock::universal_time() -
                   boost::posix_time::ptime(boost::gregorian::date(1970,1,1))).total_milliseconds();
    assert(now > 0);
    return now;
}

int64_t GetTIMECoinMicros()
{
    int64_t now = (boost::posix_time::microsec_clock::universal_time() -
                   boost::posix_time::ptime(boost::gregorian::date(1970,1,1))).total_microseconds();
    assert(now > 0);
    return now;
}

int64_t GetSystemTIMECoinInSeconds()
{
    return GetTIMECoinMicros()/1000000;
}

/** Return a time useful for the debug log */
int64_t GetLogTIMECoinMicros()
{
    if (nMockTIMECoin) return nMockTIMECoin*1000000;

    return GetTIMECoinMicros();
}

void MilliSleep(int64_t n)
{

/**
 * Boost's sleep_for was uninterruptable when backed by nanosleep from 1.50
 * until fixed in 1.52. Use the deprecated sleep method for the broken case.
 * See: https://svn.boost.org/trac/boost/ticket/7238
 */
#if defined(HAVE_WORKING_BOOST_SLEEP_FOR)
    boost::this_thread::sleep_for(boost::chrono::milliseconds(n));
#elif defined(HAVE_WORKING_BOOST_SLEEP)
    boost::this_thread::sleep(boost::posix_time::milliseconds(n));
#else
//should never get here
#error missing boost sleep implementation
#endif
}

std::string DateTIMECoinStrFormat(const char* pszFormat, int64_t nTIMECoin)
{
    // std::locale takes ownership of the pointer
    std::locale loc(std::locale::classic(), new boost::posix_time::time_facet(pszFormat));
    std::stringstream ss;
    ss.imbue(loc);
    ss << boost::posix_time::from_time_t(nTIMECoin);
    return ss.str();
}

std::string DurationToDHMS(int64_t nDurationTIMECoin)
{
    int seconds = nDurationTIMECoin % 60;
    nDurationTIMECoin /= 60;
    int minutes = nDurationTIMECoin % 60;
    nDurationTIMECoin /= 60;
    int hours = nDurationTIMECoin % 24;
    int days = nDurationTIMECoin / 24;
    if(days)
        return strprintf("%dd %02dh:%02dm:%02ds", days, hours, minutes, seconds);
    if(hours)
        return strprintf("%02dh:%02dm:%02ds", hours, minutes, seconds);
    return strprintf("%02dm:%02ds", minutes, seconds);
}
