/*
  Native File Dialog

  User API

  http://www.frogtoss.com/labs
 */


#ifndef _NFD_H
#define _NFD_H

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
        /** @cond DOXYGEN_SHOULD_IGNORE_THIS */
        
        /* Note: For Doxygen to produce clean output, you should set the 
         * PREDEFINED option to remove DECLSPEC, CALLCONVENTION, and
         * the DOXYGEN_SHOULD_IGNORE_THIS blocks.
         * PREDEFINED = DOXYGEN_SHOULD_IGNORE_THIS=1 DECLSPEC= CALLCONVENTION=
         */
        
        /** Windows needs to know explicitly which functions to export in a DLL. */
	
#ifdef BUILD_NFD_AS_DLL
        #ifdef WIN32
                #define NFD_EXPORT __declspec(dllexport)
        #elif defined(__GNUC__) && __GNUC__ >= 4
                #define NFD_EXPORT __attribute__ ((visibility("default")))
        #else
                #define NFD_EXPORT
        #endif
#else
        #define NFD_EXPORT
#endif /* BUILD_NFD_AS_DLL */

/* For Windows, by default, use the C calling convention */
#if defined(_WIN32)
	#define NFD_CALL __cdecl
#else
	#define NFD_CALL
#endif


/** @endcond DOXYGEN_SHOULD_IGNORE_THIS */
#endif /* DOXYGEN_SHOULD_IGNORE_THIS */


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

/* denotes UTF-8 char */
typedef char nfdchar_t;

/* opaque data structure -- see NFD_PathSet_* */
typedef struct {
    nfdchar_t *buf;
    size_t *indices; /* byte offsets into buf */
    size_t count;    /* number of indices into buf */
}nfdpathset_t;

typedef enum {
    NFD_ERROR,       /* programmatic error */
    NFD_OKAY,        /* user pressed okay, or successful return */
    NFD_CANCEL       /* user pressed cancel */
}nfdresult_t;
    

/* nfd_<targetplatform>.c */

/* single file open dialog */    
NFD_EXPORT nfdresult_t NFD_CALL NFD_OpenDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath );

/* multiple file open dialog */    
NFD_EXPORT nfdresult_t NFD_CALL NFD_OpenDialogMultiple( const nfdchar_t *filterList,
                                    const nfdchar_t *defaultPath,
                                    nfdpathset_t *outPaths );

/* save dialog */
NFD_EXPORT nfdresult_t NFD_CALL NFD_SaveDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath );

/* nfd_common.c */

/* get last error -- set when nfdresult_t returns NFD_ERROR */
NFD_EXPORT const char * NFD_CALL NFD_GetError( void );
/* get the number of entries stored in pathSet */
NFD_EXPORT size_t NFD_CALL NFD_PathSet_GetCount( const nfdpathset_t *pathSet );
/* Get the UTF-8 path at offset index */
NFD_EXPORT nfdchar_t * NFD_CALL NFD_PathSet_GetPath( const nfdpathset_t *pathSet, size_t index );
/* Free the pathSet */    
NFD_EXPORT void NFD_CALL NFD_PathSet_Free( nfdpathset_t *pathSet );

/* Free the outPaths returned by the Dialog functions */
NFD_EXPORT void NFD_Path_Free(nfdchar_t* path);

NFD_EXPORT bool NFD_CALL NFD_IsAvailable( void );

#ifdef __cplusplus
}
#endif

#endif
