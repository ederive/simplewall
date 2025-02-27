// simplewall
// Copyright (c) 2016-2021 Henry++

#include "global.h"

VOID _app_timer_set (_In_opt_ HWND hwnd, _Inout_ PITEM_APP ptr_app, _In_ LONG64 seconds)
{
	if (seconds <= 0)
	{
		ptr_app->is_enabled = FALSE;
		ptr_app->is_haveerrors = FALSE;

		ptr_app->timer = 0;

		if (_app_istimerset (ptr_app->htimer))
		{
			_app_timer_remove (&ptr_app->htimer);
		}
	}
	else
	{
		FILETIME file_time;
		LONG64 current_time = _r_unixtime_now ();
		BOOLEAN is_created = FALSE;

		_r_unixtime_to_filetime (current_time + seconds, &file_time);

		if (_app_istimerset (ptr_app->htimer))
		{
			SetThreadpoolTimer (ptr_app->htimer, &file_time, 0, 0);
			is_created = TRUE;
		}
		else
		{
			PTP_TIMER timer = CreateThreadpoolTimer (&_app_timer_callback, (PVOID)ptr_app->app_hash, NULL);

			if (timer)
			{
				SetThreadpoolTimer (timer, &file_time, 0, 0);
				ptr_app->htimer = timer;
				is_created = TRUE;
			}
		}

		if (is_created)
		{
			ptr_app->is_enabled = TRUE;
			ptr_app->timer = current_time + seconds;
		}
		else
		{
			ptr_app->is_enabled = FALSE;
			ptr_app->is_haveerrors = FALSE;
			ptr_app->timer = 0;

			if (_app_istimerset (ptr_app->htimer))
			{
				_app_timer_remove (&ptr_app->htimer);
			}
		}
	}

	if (hwnd)
	{
		_app_updateitembylparam (hwnd, ptr_app->app_hash, TRUE);
	}
}

VOID _app_timer_reset (_In_opt_ HWND hwnd, _Inout_ PITEM_APP ptr_app)
{
	ptr_app->is_enabled = FALSE;
	ptr_app->is_haveerrors = FALSE;

	ptr_app->timer = 0;

	if (_app_istimerset (ptr_app->htimer))
	{
		_app_timer_remove (&ptr_app->htimer);
	}

	if (hwnd)
	{
		_app_updateitembylparam (hwnd, ptr_app->app_hash, TRUE);
	}
}

VOID _app_timer_remove (_Inout_ PTP_TIMER *timer)
{
	PTP_TIMER current_timer = *timer;

	*timer = NULL;

	CloseThreadpoolTimer (current_timer);
}

BOOLEAN _app_istimersactive ()
{
	PITEM_APP ptr_app;
	SIZE_T enum_key = 0;

	_r_queuedlock_acquireshared (&lock_apps);

	while (_r_obj_enumhashtablepointer (apps_table, &ptr_app, NULL, &enum_key))
	{
		if (_app_istimerset (ptr_app->htimer))
		{
			_r_queuedlock_releaseshared (&lock_apps);

			return TRUE;
		}
	}

	_r_queuedlock_releaseshared (&lock_apps);

	return FALSE;
}

VOID CALLBACK _app_timer_callback (_Inout_ PTP_CALLBACK_INSTANCE instance, _Inout_opt_ PVOID context, _Inout_ PTP_TIMER timer)
{
	HANDLE hengine;
	HWND hwnd;
	PITEM_APP ptr_app;
	PR_LIST rules;
	HRESULT hr;

	ptr_app = _app_getappitem ((ULONG_PTR)context);

	if (!ptr_app)
		return;

	hr = CoInitializeEx (NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	hwnd = _r_app_gethwnd ();

	_app_timer_reset (hwnd, ptr_app);

	hengine = _wfp_getenginehandle ();

	if (hengine)
	{
		rules = _r_obj_createlist (NULL);

		_r_obj_addlistitem (rules, ptr_app);

		_wfp_create3filters (hengine, rules, __LINE__, FALSE);

		_r_obj_dereference (rules);
	}

	_app_updatelistviewbylparam (hwnd, ptr_app->type, PR_UPDATE_TYPE | PR_UPDATE_FORCE);

	_r_obj_dereference (ptr_app);

	_app_profile_save ();

	if (_r_config_getboolean (L"IsNotificationsTimer", TRUE))
	{
		WCHAR buffer[256];
		ULONG icon_id;

		icon_id = NIIF_INFO;

		if (!_r_config_getboolean (L"IsNotificationsSound", TRUE))
			icon_id |= NIIF_NOSOUND;

		_r_str_printf (buffer, RTL_NUMBER_OF (buffer), L"%s - %s", _r_app_getname (), _app_getappdisplayname (ptr_app, TRUE));

		_r_tray_popup (hwnd, &GUID_TrayIcon, icon_id, buffer, _r_locale_getstring (IDS_STATUS_TIMER_DONE));
	}

	if (hr == S_OK || hr == S_FALSE)
		CoUninitialize ();
}
