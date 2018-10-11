#include "wlsops.h"
#include "WLSINC.h"

struct ieee80211_local *wls_local = NULL;
struct ieee80211_vif *wls_vif = NULL;
struct ieee80211_hw *wls_hw = NULL;

int wls_hacker_init()
{
    struct net_device *dev;

    for_each_netdev(&init_net, dev)
    {
        if( dev->ieee80211_ptr ) // <net/cfg80211.h>
        {
            wls_vif = wdev_to_ieee80211_vif(dev->ieee80211_ptr);
            wls_local = (struct ieee80211_local *)wdev_priv(dev->ieee80211_ptr);
            wls_hw = &wls_local->hw;
            printh("find one driver: %s\n", dev->name);
            break;
        }
    }

    if (!wls_local)
        return -1;
    return 0;
}

int wls_conf_tx(struct tx_param param)
{
    struct ieee80211_tx_queue_params wls_params = 
    {
        .txop = param.txop,
	    .cw_min = param.cw_min,
	    .cw_max = param.cw_max,
	    .aifs = param.aifs,
        .acm = false,
        .uapsd = false
    };
    int ret;
    
    ret = wls_local->ops->conf_tx(wls_hw, wls_vif, param.ac, &wls_params);
    
    return ret;
}
