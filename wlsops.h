#ifndef __WLSOPS_H__
#define __WLSOPS_H__

#include <linux/string.h>
#include <linux/netdevice.h>
#include <net/cfg80211.h>
#include <net/mac80211.h>
#include "include/ieee80211_i.h"

extern struct ieee80211_local *wls_local;
extern struct ieee80211_vif *wls_vif;
extern struct ieee80211_hw *wls_hw;

int wls_hacker_init(void);
int wls_conf_tx(u16 ac, u8 aifs, u16 cw_min, u16 cw_max, u16 txop);

#endif