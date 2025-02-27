// simplewall
// Copyright (c) 2016-2021 Henry++

#pragma once

// guids
DEFINE_GUID (GUID_WfpProvider, 0xb0d553e2, 0xc6a0, 0x4a9a, 0xae, 0xb8, 0xc7, 0x52, 0x48, 0x3e, 0xd6, 0x2f);
DEFINE_GUID (GUID_WfpSublayer, 0x9fee6f59, 0xb951, 0x4f9a, 0xb5, 0x2f, 0x13, 0x3d, 0xcf, 0x7a, 0x42, 0x79);

// deprecated and not used, but need for compatibility
DEFINE_GUID (GUID_WfpOutboundCallout4_DEPRECATED, 0xf1251f1a, 0xab09, 0x4ce7, 0xba, 0xe3, 0x6c, 0xcc, 0xce, 0xf2, 0xc8, 0xca);
DEFINE_GUID (GUID_WfpOutboundCallout6_DEPRECATED, 0xfd497f2e, 0x46f5, 0x486d, 0xb0, 0xc, 0x3f, 0x7f, 0xe0, 0x7a, 0x94, 0xa6);
DEFINE_GUID (GUID_WfpInboundCallout4_DEPRECATED, 0xefc879ce, 0x3066, 0x45bb, 0x8a, 0x70, 0x17, 0xfe, 0x29, 0x78, 0x53, 0xc0);
DEFINE_GUID (GUID_WfpInboundCallout6_DEPRECATED, 0xd0420299, 0x52d8, 0x4f18, 0xbc, 0x80, 0x47, 0x3a, 0x24, 0x93, 0xf2, 0x69);
DEFINE_GUID (GUID_WfpListenCallout4_DEPRECATED, 0x51fa679d, 0x578b, 0x4835, 0xa6, 0x3e, 0xca, 0xd7, 0x68, 0x7f, 0x74, 0x95);
DEFINE_GUID (GUID_WfpListenCallout6_DEPRECATED, 0xa02187ca, 0xe655, 0x4adb, 0xa1, 0xf2, 0x47, 0xa2, 0xc9, 0x78, 0xf9, 0xce);

// filter names
#define FW_NAME_BLOCK_CONNECTION L"BlockConnection"
#define FW_NAME_BLOCK_REDIRECT L"BlockRedirect"
#define FW_NAME_BLOCK_RECVACCEPT L"BlockRecvAccept"
#define FW_NAME_ICMP_ERROR L"BlockIcmpError"
#define FW_NAME_TCP_RST_ONCLOSE L"BlockTcpRstOnClose"
#define FW_NAME_BOOTTIME L"BlockBoottime"
#define FW_NAME_LOOPBACK L"AllowLoopback"

// sublayer weight
#define FW_SUBLAYER_WEIGHT 0xFFFF

// filter weights
#define FW_WEIGHT_HIGHEST_IMPORTANT 0x0F
#define FW_WEIGHT_HIGHEST 0x0E
#define FW_WEIGHT_RULE_BLOCKLIST 0x0D
#define FW_WEIGHT_RULE_USER_BLOCK 0x0C
#define FW_WEIGHT_RULE_USER 0x0B
#define FW_WEIGHT_RULE_SYSTEM 0x0A
#define FW_WEIGHT_APP 0x09
#define FW_WEIGHT_LOWEST 0x08

ENUM_INSTALL_TYPE _wfp_isproviderinstalled (_In_ HANDLE engine_handle);
ENUM_INSTALL_TYPE _wfp_issublayerinstalled (_In_ HANDLE engine_handle);

BOOLEAN _wfp_isfiltersapplying ();
ENUM_INSTALL_TYPE _wfp_isfiltersinstalled ();

HANDLE _wfp_getenginehandle ();
PR_STRING _wfp_getlayername (_In_ LPCGUID layer_guid);

BOOLEAN _wfp_initialize (_In_ HANDLE engine_handle);
VOID _wfp_uninitialize (_In_ HANDLE engine_handle, _In_ BOOLEAN is_full);

VOID _wfp_installfilters (_In_ HANDLE engine_handle);

BOOLEAN _wfp_transact_start (_In_ HANDLE engine_handle, _In_ UINT line);
BOOLEAN _wfp_transact_commit (_In_ HANDLE engine_handle, _In_ UINT line);

ULONG _wfp_createcallout (_In_ HANDLE engine_handle, _In_ LPCGUID layer_key, _In_ LPCGUID callout_key);

BOOLEAN _wfp_deletefilter (_In_ HANDLE engine_handle, _In_ LPCGUID filter_id);
ULONG _wfp_createfilter (_In_ HANDLE engine_handle, _In_ ENUM_TYPE_DATA type, _In_opt_ LPCWSTR filter_name, _In_reads_ (count) FWPM_FILTER_CONDITION *lpcond, _In_ UINT32 count, _In_ LPCGUID layer_id, _In_opt_ LPCGUID callout_id, _In_ UINT8 weight, _In_ FWP_ACTION_TYPE action, _In_ UINT32 flags, _Inout_opt_ PR_ARRAY guids);

VOID _wfp_clearfilter_ids ();
VOID _wfp_destroyfilters (_In_ HANDLE engine_handle);
BOOLEAN _wfp_destroyfilters_array (_In_ HANDLE engine_handle, _In_ PR_ARRAY guids, _In_ UINT line);

BOOLEAN _wfp_createrulefilter (_In_ HANDLE engine_handle, _In_ ENUM_TYPE_DATA filter_type, _In_opt_ LPCWSTR filter_name, _In_opt_ ULONG_PTR app_hash, _In_opt_ PITEM_FILTER_CONFIG filter_config, _In_opt_ PR_STRINGREF rule_remote, _In_opt_ PR_STRINGREF rule_local, _In_ UINT8 weight, _In_ FWP_ACTION_TYPE action, _In_ UINT32 flags, _Inout_opt_ PR_ARRAY guids);

BOOLEAN _wfp_create4filters (_In_ HANDLE engine_handle, _In_  PR_LIST rules, _In_ UINT line, _In_ BOOLEAN is_intransact);
BOOLEAN _wfp_create3filters (_In_ HANDLE engine_handle, _In_ PR_LIST rules, _In_ UINT line, _In_ BOOLEAN is_intransact);
BOOLEAN _wfp_create2filters (_In_ HANDLE engine_handle, _In_ UINT line, _In_ BOOLEAN is_intransact);

_Ret_maybenull_
PR_ARRAY _wfp_dumpfilters (_In_ HANDLE engine_handle, _In_ LPCGUID provider_id);

VOID NTAPI _wfp_applythread (_In_ PVOID arglist, _In_ ULONG busy_count);

VOID _wfp_firewallenable (_In_ BOOLEAN is_enable);
BOOLEAN _wfp_firewallisenabled ();

ULONG _FwpmGetAppIdFromFileName1 (_In_ PR_STRING path, _In_ ENUM_TYPE_DATA type, _Out_ PVOID_PTR byte_blob);

VOID ByteBlobAlloc (_In_ LPCVOID data, _In_ SIZE_T bytes_count, _Out_ PVOID_PTR byte_blob);
VOID ByteBlobFree (_Inout_ PVOID_PTR byte_blob);
