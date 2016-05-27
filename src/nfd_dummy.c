#include "nfd.h"

nfdresult_t NFD_OpenDialog( const char *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
	return NFD_CANCEL;
}

nfdresult_t NFD_OpenDialogMultiple( const nfdchar_t *filterList,
                                    const nfdchar_t *defaultPath,
                                    nfdpathset_t *outPaths )
{
	return NFD_CANCEL;
}

nfdresult_t NFD_SaveDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
	return NFD_CANCEL;
}

bool NFD_IsAvailable()
{
	return false;
}

