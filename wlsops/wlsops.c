#include "wlsops.h"
#include "WLSINC.h"

struct ieee80211_local *wls_local = NULL;
struct ieee80211_vif *wls_vif = NULL;
struct ieee80211_hw *wls_hw = NULL;

int wls_hack_init()
{
    struct net_device *dev;

    for_each_netdev(&init_net, dev)
    {
        if( dev->ieee80211_ptr ) // is a 802.11 device
        {
            wls_vif = wdev_to_ieee80211_vif(dev->ieee80211_ptr);
            if (wls_vif) {
                wls_local = (struct ieee80211_local *) wdev_priv(dev->ieee80211_ptr);
                wls_hw = &wls_local->hw;
                printh("find 802.11 device: %s\n", dev->name);
                printh("wls_vif: %p, wls_local: %p, wls_hw:%p\n", wls_vif, wls_local, wls_hw);
                break;
            }
        }
    }

    if (!wls_local)
    {
        printh("No 802.11 device found.\n");
        return -1;
    }
    
    return 0;
}

int wls_conf_tx(struct tx_param param)
{
    int ret;
    struct ieee80211_tx_queue_params wls_params = 
    {
        .txop = param.txop,
	    .cw_min = param.cw_min,
	    .cw_max = param.cw_max,
	    .aifs = param.aifs,
        // .acm = false,
        // .uapsd = false,
        // .mu_edca = false
    };
    
    ret = wls_local->ops->conf_tx(wls_hw, wls_vif, param.ac, &wls_params);
    return ret;
}
