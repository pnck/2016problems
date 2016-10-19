

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Oct 12 07:20:38 2016
 */
/* Compiler settings for ServerInjector.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ServerInjector_h_h__
#define __ServerInjector_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IServerInjector_FWD_DEFINED__
#define __IServerInjector_FWD_DEFINED__
typedef interface IServerInjector IServerInjector;

#endif 	/* __IServerInjector_FWD_DEFINED__ */


#ifndef __ServerInjector_FWD_DEFINED__
#define __ServerInjector_FWD_DEFINED__

#ifdef __cplusplus
typedef class ServerInjector ServerInjector;
#else
typedef struct ServerInjector ServerInjector;
#endif /* __cplusplus */

#endif 	/* __ServerInjector_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ServerInjector_LIBRARY_DEFINED__
#define __ServerInjector_LIBRARY_DEFINED__

/* library ServerInjector */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ServerInjector;

#ifndef __IServerInjector_DISPINTERFACE_DEFINED__
#define __IServerInjector_DISPINTERFACE_DEFINED__

/* dispinterface IServerInjector */
/* [uuid] */ 


EXTERN_C const IID DIID_IServerInjector;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("037FE15E-C13A-4D45-AF7E-13BF1A20B43A")
    IServerInjector : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IServerInjectorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IServerInjector * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IServerInjector * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IServerInjector * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IServerInjector * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IServerInjector * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IServerInjector * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IServerInjector * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IServerInjectorVtbl;

    interface IServerInjector
    {
        CONST_VTBL struct IServerInjectorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IServerInjector_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IServerInjector_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IServerInjector_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IServerInjector_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IServerInjector_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IServerInjector_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IServerInjector_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IServerInjector_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ServerInjector;

#ifdef __cplusplus

class DECLSPEC_UUID("83493616-0F02-4DA9-A429-1D3F1EF9FD08")
ServerInjector;
#endif
#endif /* __ServerInjector_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


