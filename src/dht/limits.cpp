// Copyright (c) 2019 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "dht/limits.h"

#include "bdap/domainentrydb.h"
#include "bdap/linkingdb.h"
#include "chain.h"
#include "utilstrencodings.h"
#include "tinyformat.h"

#include <boost/algorithm/string.hpp>

#include <map>
#include <vector>

//  Default accepted DHT record types:
std::multimap<std::string, CAllowDataCode> mapAllowedData = {
    //                             salt,       slots,  start, expire
    {"info",        CAllowDataCode("info",     32,     0,     0)}, 
    {"denylink",    CAllowDataCode("denylink", 32,     0,     0)},
    {"ignore",      CAllowDataCode("ignore",   32,     0,     0)},
    {"index",       CAllowDataCode("index",    32,     0,     0)},
    {"avatar",      CAllowDataCode("avatar",    4,     0,     0)},
    {"ldap",        CAllowDataCode("ldap",     32,     0,     0)},
    {"oauth",       CAllowDataCode("oauth",    16,     0,     0)},
    {"pshare",      CAllowDataCode("pshare",   48,     0,     0)},
    {"pconsult",    CAllowDataCode("pconsult", 48,     0,     0)},
    {"noid",        CAllowDataCode("noid",     48,     0,     0)},
    {"whispers",    CAllowDataCode("whispers", 48,     0,     0)},
    {"spam",        CAllowDataCode("spam",     64,     0,     0)},
    {"groups",      CAllowDataCode("groups",   48,     0,     0)},
    {"chat",        CAllowDataCode("chat",     32,     0,     0)},
    {"message",     CAllowDataCode("message",  32,     0,     0)},
    {"data",        CAllowDataCode("data",    128,     0,     0)},
    {"keys",        CAllowDataCode("keys",     32,     0,     0)},
    {"test",        CAllowDataCode("test",      8,     0,     0)},
};


bool CheckSalt(const std::string& strSalt, const unsigned int nHeight, std::string& strErrorMessage)
{
    strErrorMessage = "";
    std::vector<std::string> vSplit;
    boost::split(vSplit, strSalt, boost::is_any_of(":"));
    if (vSplit.size() != 2) {
        strErrorMessage = strprintf("Invalid salt (%s). Could not find ':' delimiter", strSalt);
        return false;
    }
    uint32_t nSlots;
    if (ParseUInt32(vSplit[1], &nSlots)) {
        strErrorMessage = strprintf("Invalid salt (%s). Could not parse slot number after :", strSalt);
        return false;
    }
    std::multimap<std::string, CAllowDataCode>::iterator iAllowed = mapAllowedData.find(vSplit[0]);
    while (iAllowed != mapAllowedData.end()) {
        if (nHeight > iAllowed->second.nStartHeight) {
            strErrorMessage = strprintf("%s, Allow data type found but height is greater than allowed data start height %d. ", strErrorMessage, iAllowed->second.nStartHeight);
            iAllowed++;
            continue;
        }
        if (iAllowed->second.nExpireTime >= nHeight && iAllowed->second.nExpireTime != 0) {
            strErrorMessage = strprintf("%s, Allow data type found but expired %d. ", strErrorMessage, iAllowed->second.nExpireTime);
            iAllowed++;
            continue;
        }
        if (nSlots <= iAllowed->second.nMaximumSlots) {
            strErrorMessage = strprintf("%s, Allow data type found but too many slots (%d) used. Max slots = %d", strErrorMessage, nSlots, iAllowed->second.nMaximumSlots);
            iAllowed++;
            continue;
        }
        // Passes all checks so it is a valid data record salt.
        return true;
    }
    strErrorMessage = strprintf("%s, Invalid salt. Allow data type salt not found in allowed data map.", strErrorMessage);
    return false;
}

bool CheckPubKey(const std::vector<unsigned char>& vchPubKey)
{
    bool fAccountPubkey = AccountPubKeyExists(vchPubKey);
    bool fLinkPubkey = LinkPubKeyExists(vchPubKey);
    if (fAccountPubkey || fLinkPubkey)
        return true;

    return false;
}