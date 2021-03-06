#include "cw.h"
#include "capwap_items.h"
#include "dbg.h"
#include "mbag.h"


int cw_in_ac_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{

	return cw_read_ac_descriptor(conn->config,data,len,NULL);

	struct cw_ac_status *status = malloc(sizeof(struct cw_ac_status));
	if (!status)
		return 0;

	status->stations = cw_get_word(data);
	status->limit = cw_get_word(data + 2);
	status->active_wtps = cw_get_word(data + 4);
	status->max_wtps = cw_get_word(data + 6);
	status->security = cw_get_byte(data + 8);
	status->rmac_field = cw_get_byte(data + 9);
	status->dtls_policy = cw_get_byte(data + 11);

	cw_dbg(DBG_SUBELEM,
	       "AC Descriptor: WTPs:%d/%d, Stations:%d/%d, Security:%d, Rmac:%d, DTLS-Policy:%d",
	       status->active_wtps, status->max_wtps, status->stations, status->limit,
	       status->security, status->rmac_field, status->dtls_policy);


	mbag_set_ptr(conn->incomming, CW_ITEM_AC_STATUS, status);

	static struct cw_descriptor_subelem_def allowed[] = {
		{0,CW_SUBELEM_AC_HARDWARE_VERSION, CW_ITEM_WTP_HARDWARE_VERSION, 1024,1},
		{0,CW_SUBELEM_AC_SOFTWARE_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024,1},
		{0,0, NULL,0, 0}
	};

	cw_read_descriptor_subelems(conn->config, data + 12, len - 12, allowed);

	return 1;
}
