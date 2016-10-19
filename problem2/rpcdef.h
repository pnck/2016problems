

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Oct 12 04:42:32 2016
 */
/* Compiler settings for rpcdef.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */
#pragma comment( lib,"rpcrt4.lib")

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"
#include <memory>

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __rpcdef_h__
#define __rpcdef_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IRpcCheck_INTERFACE_DEFINED__
#define __IRpcCheck_INTERFACE_DEFINED__

/* interface IRpcCheck */
/* [implicit_handle][version][uuid] */ 

int CheckFlag( 
    /* [string][in] */ const wchar_t *pflgstr);

void Shutdown(void);


extern handle_t Client_Binding;


extern RPC_IF_HANDLE IRpcCheck_v1_0_c_ifspec;
extern RPC_IF_HANDLE IRpcCheck_v1_0_s_ifspec;
#endif /* __IRpcCheck_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


